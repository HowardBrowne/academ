#include "ofxEdsdk.h"

/*
 This controls the size of liveBufferMiddle. If you are running at a low fps
 (lower than 30 fps), then it will effectively correspond to the latency of the
 camera. If you're running higher than 30 fps, it will determine how many frames
 you can miss without dropping one. For example, if you are running at 60 fps
 but one frame happens to last 200 ms, and your buffer size is 4, you will drop
 2 frames.
 */
#define OFX_EDSDK_BUFFER_SIZE 4

namespace ofxEdsdk {
	
	EdsError EDSCALLBACK Camera::handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context) {
		ofLogVerbose() << "object event " << Eds::getObjectEventString(event);
		if(object) {
			if(event == kEdsObjectEvent_DirItemCreated) {
				((Camera*) context)->downloadImage(object);
			} else if(event == kEdsObjectEvent_DirItemRemoved) {
				// no need to release a removed item
			} else {
				try {
					Eds::SafeRelease(object);
				} catch (Eds::Exception& e) {
					ofLogError() << "Error while releasing EdsBaseRef inside handleObjectEvent()";
				}
			}
		}
		return EDS_ERR_OK;
	}
	
	EdsError EDSCALLBACK Camera::handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID propertyId, EdsUInt32 param, EdsVoid* context) {
		ofLogVerbose() << "property event " << Eds::getPropertyEventString(event) << ": " << Eds::getPropertyIDString(propertyId) << " / " << param;
		if(propertyId == kEdsPropID_Evf_OutputDevice) {
			((Camera*) context)->setLiveReady(true);
		}
		return EDS_ERR_OK;
	}
	
	EdsError EDSCALLBACK Camera::handleCameraStateEvent(EdsStateEvent event, EdsUInt32 param, EdsVoid* context) {
		ofLogVerbose() << "camera state event " << Eds::getStateEventString(event) << ": " << param;
		if(event == kEdsStateEvent_WillSoonShutDown) {
			((Camera*) context)->sendKeepAlive();
		}
		return EDS_ERR_OK;
	}
	
	Camera::Camera() :
	connected(false),
	liveReady(false),
	liveDataReady(false),
	frameNew(false),
	needToTakePhoto(false),
	photoNew(false),
	needToDecodePhoto(false),
	needToUpdatePhoto(false),
	photoDataReady(false) {
		liveBufferMiddle.resize(OFX_EDSDK_BUFFER_SIZE);
		for(int i = 0; i < liveBufferMiddle.maxSize(); i++) {
			liveBufferMiddle[i] = new ofBuffer();
		}
	}
	
	Camera::~Camera() {
		waitForThread();
		lock();
		if(connected) {
			if(liveReady) {
				Eds::EndLiveview(camera);
			}
			try {
				Eds::CloseSession(camera);
				Eds::TerminateSDK();
			} catch (Eds::Exception& e) {
				ofLogError() << "There was an error destroying ofxEds::Camera: " << e.what();
			}
		}
		unlock();
		for(int i = 0; i < liveBufferMiddle.maxSize(); i++) {
			delete liveBufferMiddle[i];
		}
	}
	
	bool Camera::setup(int deviceId) {
		try {
			Eds::InitializeSDK();
			
			EdsCameraListRef cameraList;
			Eds::GetCameraList(&cameraList);
			
			UInt32 cameraCount;
			Eds::GetChildCount(cameraList, &cameraCount);
			
			if(cameraCount > 0) {				
				EdsInt32 cameraIndex = deviceId;
				Eds::GetChildAtIndex(cameraList, cameraIndex, &camera);
				Eds::SetObjectEventHandler(camera, kEdsObjectEvent_All, handleObjectEvent, this);
				Eds::SetPropertyEventHandler(camera, kEdsPropertyEvent_All, handlePropertyEvent, this);
				Eds::SetCameraStateEventHandler(camera, kEdsStateEvent_All, handleCameraStateEvent, this);
				
				EdsDeviceInfo info;
				Eds::GetDeviceInfo(camera, &info);
				Eds::SafeRelease(cameraList);
				
				startThread(true, false);
				return true;
			} else {
				ofLogError() << "No cameras are connected for ofxEds::Camera::setup().";
			}
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error during Camera::setup(): " << e.what();
		}
		return false;
	}
	
	void Camera::update() {
		lock();
		cout << "size:" << liveBufferMiddle.size() << endl;
		if(liveBufferMiddle.size() > 0) {
			// decoding the jpeg in the main thread allows the capture thread to run in a tighter loop.
			ofBuffer* middleFront = liveBufferMiddle.front();
			liveBufferFront.set(middleFront->getBinaryBuffer(), middleFront->size());
			liveBufferMiddle.pop();
			unlock();
			ofLoadImage(livePixels, liveBufferFront);
			frameNew = true;
			if(liveTexture.getWidth() != livePixels.getWidth() ||
				 liveTexture.getHeight() != livePixels.getHeight()) {
				liveTexture.allocate(livePixels.getWidth(), livePixels.getHeight(), GL_RGB8);
			}
			liveTexture.loadData(livePixels);
			lock();
			liveDataReady = true;
			unlock();
		} else {
			unlock();
		}
	}
	
	bool Camera::isFrameNew() {
		if(frameNew) {
			frameNew = false;
			return true;
		} else {
			return false;
		}
	}
	
	bool Camera::isPhotoNew() {
		if(photoNew) {
			photoNew = false;
			return true;
		} else {
			return false;
		}
	}
	
	float Camera::getFrameRate() {
		float frameRate;
		if(lock()) {
			frameRate = fps.getFrameRate();
			unlock();
		}
		return frameRate;
	}
	
	void Camera::takePhoto() {
		lock();
		needToTakePhoto = true;
		unlock();
	}
	
	ofPixels& Camera::getLivePixels() {
		return livePixels;
	}
	
	ofPixels& Camera::getPhotoPixels() {
		if(needToDecodePhoto) {
			ofLoadImage(photoPixels, photoBuffer);
			needToDecodePhoto = false;
		}
		return photoPixels;
	}
	
	unsigned int Camera::getWidth() const {
		return livePixels.getWidth();
	}
	
	unsigned int Camera::getHeight() const {
		return livePixels.getHeight();
	}
	
	void Camera::draw(float x, float y) {
		draw(x, y, getWidth(), getHeight());
	}
	
	void Camera::draw(float x, float y, float width, float height) {
		if(liveDataReady) {
			liveTexture.draw(x, y, width, height);
		}
	}
	
	void Camera::drawPhoto(float x, float y) {
		if(photoDataReady) {
			ofPixels& photoPixels = getPhotoPixels();
			draw(x, y, getWidth(), getHeight());
		}
	}
	
	void Camera::drawPhoto(float x, float y, float width, float height) {
		if(photoDataReady) {
			ofPixels& photoPixels = getPhotoPixels();
			if(needToUpdatePhoto) {
				if(photoTexture.getWidth() != photoPixels.getWidth() ||
					 photoTexture.getHeight() != photoPixels.getHeight()) {
					photoTexture.allocate(photoPixels.getWidth(), photoPixels.getHeight(), GL_RGB8);
				}
				photoTexture.loadData(photoPixels);
				needToUpdatePhoto = false;
			}
			photoTexture.draw(x, y, width, height);
		}
	}
	
	bool Camera::isLiveReady() const {
		return liveDataReady;
	}
	
	void Camera::setLiveReady(bool liveReady) {
		// this is done before the threaded loop starts
		this->liveReady = liveReady;
	}
	
	void Camera::sendKeepAlive() {
		if(connected) {
			try {
				Eds::SendStatusCommand(camera, kEdsCameraCommand_ExtendShutDownTimer, 0);
			} catch (Eds::Exception& e) {
				ofLogError() << "Error while sending kEdsCameraCommand_ExtendShutDownTimer with Eds::SendStatusCommand: " << e.what();
			}
		}
	}
	
	void Camera::downloadImage(EdsDirectoryItemRef directoryItem) {
		Eds::DownloadImage(directoryItem, photoBuffer);
		ofLogVerbose() << "Downloaded image: " << (int) (photoBuffer.size() / 1024) << " KB";
		photoDataReady = true;
		photoNew = true;
		needToDecodePhoto = true;
		needToUpdatePhoto = true;
	}
	
	void Camera::savePhoto(string filename) {
		ofLogVerbose() << "status of needToDecodePhoto: " << needToDecodePhoto << endl;
		ofBufferToFile(filename, photoBuffer, true);
	}
	
	void Camera::threadedFunction() {
		if(lock()) {
			try {
				Eds::OpenSession(camera);
				connected = true;
				Eds::StartLiveview(camera);
			} catch (Eds::Exception& e) {
				ofLogError() << "There was an error opening the camera, or starting live view: " << e.what();
			}
			unlock();
		}
		
		// threaded variables:
		// liveReady, liveBufferMiddle, liveBufferBack, fps, needToTakePhoto
		while(isThreadRunning()) {
			if(liveReady) {
				if(Eds::DownloadEvfData(camera, liveBufferBack)) {
					lock();
					ofBuffer* middleBack = liveBufferMiddle.back();
					middleBack->set(liveBufferBack.getBinaryBuffer(), liveBufferBack.size()); // liveBufferMiddle = liveBufferBack;
					liveBufferMiddle.push();
					fps.tick();
					unlock();
				}
			}
			
			lock();
			if(needToTakePhoto) {
				unlock();
				try {
					Eds::SendCommand(camera, kEdsCameraCommand_TakePicture, 0);
					lock();
					needToTakePhoto = false;
					unlock();
				} catch (Eds::Exception& e) {
					ofLogError() << "Error while taking a picture: " << e.what();
				}
			} else {
				unlock();
			}
		}
	}
}