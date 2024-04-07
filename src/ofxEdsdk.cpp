#include "ofxEdsdk.h"

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
	needToUpdate(false),
	needToTakePhoto(false),
	photoNew(false),
	needToDecodePhoto(false),
	needToUpdatePhoto(false),
	photoDataReady(false) {
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
	}
	
	void Camera::setup(int deviceId) {
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
			} else {
				ofLogError() << "No cameras are connected for ofxEds::Camera::setup().";
			}
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error during Camera::setup(): " << e.what();
		}
	}
	
	void Camera::update() {
		lock();
		if(needToUpdate) {
			// decoding the jpeg in the main thread allows the capture thread to run in a tighter loop.
			liveBufferFront.set(liveBufferMiddle.getBinaryBuffer(), liveBufferMiddle.size());
			unlock();
			ofLoadImage(livePixels, liveBufferFront);
			frameNew = true;
			if(liveTexture.getWidth() != livePixels.getWidth() ||
				 liveTexture.getHeight() != livePixels.getHeight()) {
				liveTexture.allocate(livePixels.getWidth(), livePixels.getHeight(), GL_RGB8);
			}
			liveTexture.loadData(livePixels);
			lock();
			needToUpdate = false;
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
		// liveReady, needToUpdate, liveBufferMiddle, liveBufferBack, fps, needToTakePhoto
		while(isThreadRunning()) {
			if(liveReady) {
				if(Eds::DownloadEvfData(camera, liveBufferBack)) {
					lock();
					needToUpdate = true;
					liveBufferMiddle.set(liveBufferBack.getBinaryBuffer(), liveBufferBack.size()); // liveBufferMiddle = liveBufferBack;
					fps.tick();
					unlock();
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
}