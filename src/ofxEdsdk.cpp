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
#define OFX_EDSDK_LIVE_DELAY 100
#ifdef TARGET_OSX
#include <Cocoa/Cocoa.h>
#elif TARGET_WIN32
#define _WIN32_DCOM
#include <objbase.h>
#endif
namespace ofxEdsdk {
	
	EdsError EDSCALLBACK Camera::handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context) {
		ofLogVerbose() << "object event " << Eds::getObjectEventString(event);
		if(object) {
			if(event == kEdsObjectEvent_DirItemCreated) {
				((Camera*) context)->setDownloadImage(object);
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
			((Camera*) context)->setSendKeepAlive();
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
	photoDataReady(false),
	needToSendKeepAlive(false),
	needToDownloadImage(false),
#ifdef  TARGET_OSX
	bTryInitLiveView(false),
#endif
	resetIntervalMinutes(15) {
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
				ofLogVerbose("ofxEdsdk::setup") << "connected camera model: " <<  info.szDeviceDescription << " " << info.szPortName << endl;
				
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
		if(connected){
#ifdef TARGET_OSX
			if (bTryInitLiveView) {
				if (ofGetElapsedTimeMillis() - initTime > OFX_EDSDK_LIVE_DELAY) {
					bTryInitLiveView = false;
					resetLiveView();
				}
			}
#endif
		lock();
		if(liveBufferMiddle.size() > 0) {
			// decoding the jpeg in the main thread allows the capture thread to run in a tighter loop.
			ofBuffer* middleFront = liveBufferMiddle.front();
			liveBufferFront.set(middleFront->getBinaryBuffer(), middleFront->size());
			liveBufferMiddle.pop();
			unlock();
			ofLoadImage(livePixels, liveBufferFront);
			if(liveTexture.getWidth() != livePixels.getWidth() ||
				 liveTexture.getHeight() != livePixels.getHeight()) {
				liveTexture.allocate(livePixels.getWidth(), livePixels.getHeight(), GL_RGB8);
			}
			liveTexture.loadData(livePixels);
			lock();
			liveDataReady = true;
			frameNew = true;
			unlock();
		} else {
			unlock();
		}
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
		lock();
		frameRate = fps.getFrameRate();
		unlock();
		return frameRate;
	}
	
	void Camera::takePhoto(bool blocking) {
		lock();
		needToTakePhoto = true;
		unlock();
		if(blocking) {
			while(!photoNew) {
				ofSleepMillis(10);
			}
		}
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
	
	ofTexture& Camera::getLiveTexture() {
		return liveTexture;
	}
	
	void Camera::drawPhoto(float x, float y) {
		if(photoDataReady) {
			ofPixels& photoPixels = getPhotoPixels();
			draw(x, y, getWidth(), getHeight());
		}
	}
	
	void Camera::drawPhoto(float x, float y, float width, float height) {
		if(photoDataReady) {
			photoTexture.draw(x, y, width, height);
		}
	}
	
	ofTexture& Camera::getPhotoTexture() {
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
		}
		return photoTexture;
	}
	
	bool Camera::isLiveReady() const {
		return liveDataReady;
	}
	
	void Camera::setLiveReady(bool liveReady) {
		// this is done before the threaded loop starts
		this->liveReady = liveReady;
	}
	
	void Camera::setDownloadImage(EdsDirectoryItemRef directoryItem) {
		lock();
		this->directoryItem = directoryItem;
		needToDownloadImage = true;
		unlock();
	}
	
	void Camera::setSendKeepAlive() {
		lock();
		needToSendKeepAlive = true;
		unlock();
	}
	
	bool Camera::savePhoto(string filename) {
		return ofBufferToFile(filename, photoBuffer, true);
	}
	
	void Camera::resetLiveView() {
		lock();
		if(connected) {
			Eds::StartLiveview(camera);
			lastResetTime = ofGetElapsedTimef();
		}
		unlock();
	}
	
	void Camera::threadedFunction() {
#ifdef TARGET_OSX
		
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#elif TARGET_WIN32
		CoInitializeEx( NULL, 0x0);// OINIT_MULTITHREADED );
#endif	
		lock();
		try {
			Eds::OpenSession(camera);
			connected = true;
#ifdef TARGET_OSX
			bTryInitLiveView = true;
			initTime = ofGetElapsedTimeMillis();
#else
			Eds::StartLiveview(camera);
#endif
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error opening the camera, or starting live view: " << e.what();
			return;
		}
		lastResetTime = ofGetElapsedTimef();
		unlock();
		
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
			
			if(needToTakePhoto) {
				try {
					Eds::SendCommand(camera, kEdsCameraCommand_TakePicture, 0);
					lock();
					needToTakePhoto = false;
					unlock();
				} catch (Eds::Exception& e) {
					ofLogError() << "Error while taking a picture: " << e.what();
				}
			}
			
			if(needToSendKeepAlive) {
				try {
					// always causes EDS_ERR_DEVICE_BUSY, even with live view disabled or a delay
					// but if it's not here, then the camera shuts down after 5 minutes.
					Eds::SendStatusCommand(camera, kEdsCameraCommand_ExtendShutDownTimer, 0);
				} catch (Eds::Exception& e) {
					ofLogError() << "Error while sending kEdsCameraCommand_ExtendShutDownTimer with Eds::SendStatusCommand: " << e.what();
				}
				lock();
				needToSendKeepAlive = false;
				unlock();
			}
			
			if(needToDownloadImage) {
				try {
					Eds::DownloadImage(directoryItem, photoBuffer);
					ofLogVerbose() << "Downloaded image: " << (int) (photoBuffer.size() / 1024) << " KB";
					lock();
					photoDataReady = true;
					photoNew = true;
					needToDecodePhoto = true;
					needToUpdatePhoto = true;
					needToDownloadImage = false;
					unlock();
				} catch (Eds::Exception& e) {
					ofLogError() << "Error while downloading image: " << e.what();
				}
			}
			
			float timeSinceLastReset = ofGetElapsedTimef() - lastResetTime;
			if(timeSinceLastReset > resetIntervalMinutes * 60) {
				resetLiveView();
			}
			
			// the t2i can run at 30 fps = 33 ms, so this might cause frame drops
			ofSleepMillis(5);
		}
#ifdef TARGET_OSX
		[pool drain];
#elif TARGET_WIN32
		CoUninitialize();
#endif
	}
}