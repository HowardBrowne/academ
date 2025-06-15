#include "ofxEdsdk.h"

/*
 This controls the size of liveBufferMiddle. If you are running at a low fps
 (lower than camera fps), then it will effectively correspond to the latency of the
 camera. If you're running higher than camera fps, it will determine how many frames
 you can miss without dropping one. For example, if you are running at 60 fps
 but one frame happens to last 200 ms, and your buffer size is 4, you will drop
 2 frames if your camera is running at 30 fps.
 */
#define OFX_EDSDK_BUFFER_SIZE 1

#define OFX_EDSDK_JPG_FORMAT 14337
#define OFX_EDSDK_MOV_FORMAT 45316

#if defined(TARGET_WIN32)
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
            ((Camera*) context)->setLiveViewReady(true);
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
    deviceId(0),
    orientationMode(0),
    bytesPerFrame(0),
    connected(false),
    liveViewReady(false),
    liveDataReady(false),
    frameNew(false),
    needToTakePhoto(false),
    photoNew(false),
    needToStartRecording(false),
    needToStopRecording(false),
    movieNew(false),
    needToDecodePhoto(false),
    needToUpdatePhoto(false),
    photoDataReady(false),
    needToSendKeepAlive(false),
    needToDownloadImage(false),
    resetIntervalMinutes(15) {
        liveBufferMiddle.resize(OFX_EDSDK_BUFFER_SIZE);
        for(int i = 0; i < liveBufferMiddle.maxSize(); i++) {
            liveBufferMiddle[i] = new ofBuffer();
        }
        liveBufferFront = new ofBuffer();
        liveBufferBack = new ofBuffer();
    }
    
    void Camera::setDeviceId(int deviceId) {
        this->deviceId = deviceId;
    }
    
    void Camera::setOrientationMode(int orientationMode) {
        this->orientationMode = orientationMode;
    }
    
    void Camera::setLiveView(bool useLiveView) {
        this->useLiveView = useLiveView;
    }
    
    void Camera::setup() {
        initialize();
        startCapture();
        startThread();
    }
    
    bool Camera::close() {
        stopThread();
        // for some reason waiting for the thread keeps it from
        // completing, but sleeping then stopping capture is ok.
        ofSleepMillis(100);
        stopCapture();
    }
    
    Camera::~Camera() {
        if(connected) {
            ofLogError() << "You must call close() before destroying the camera.";
        }
        for(int i = 0; i < liveBufferMiddle.maxSize(); i++) {
            delete liveBufferMiddle[i];
        }
        delete liveBufferFront;
        delete liveBufferBack;
    }
    
    void Camera::update() {
        if(connected){
            lock();
            if(liveBufferMiddle.size() > 0) {
                // decoding the jpeg in the main thread allows the capture thread to run in a tighter loop.
                swap(liveBufferFront, liveBufferMiddle.front());
                liveBufferMiddle.pop();
                unlock();
                ofLoadImage(livePixels, *liveBufferFront);
                livePixels.rotate90(orientationMode);
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
    
    bool Camera::isMovieNew() {
        if(movieNew) {
            movieNew = false;
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
    
    float Camera::getBandwidth() {
        lock();
        float bandwidth = bytesPerFrame * fps.getFrameRate();
        unlock();
        return bandwidth;
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
    
    void Camera::beginMovieRecording()
    {
        lock();
        needToStartRecording = true;
        unlock();
    }
    
    void Camera::endMovieRecording()
    {
        lock();
		needToStopRecording = true;
		unlock();
    }
    
	const ofPixels& Camera::getLivePixels() const {
		return livePixels;
    }
    
    const ofPixels& Camera::getPhotoPixels() const {
        if(needToDecodePhoto) {
            ofLoadImage(photoPixels, photoBuffer);
            photoPixels.rotate90(orientationMode);
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
    
	const ofTexture& Camera::getLiveTexture() const {
		return liveTexture;
	}
	
	void Camera::drawPhoto(float x, float y) {
		if(photoDataReady) {
			const ofPixels& photoPixels = getPhotoPixels();
			draw(x, y, getWidth(), getHeight());
		}
	}
	
	void Camera::drawPhoto(float x, float y, float width, float height) {
		if(photoDataReady) {
            getPhotoTexture().draw(x, y, width, height);
		}
	}
	
	const ofTexture& Camera::getPhotoTexture() const {
		if(photoDataReady) {
			const ofPixels& photoPixels = getPhotoPixels();
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
    
    bool Camera::isLiveDataReady() const {
        return liveDataReady;
    }
    
    // this is only used by the EDSDK callback
    void Camera::setLiveViewReady(bool liveViewReady) {
        this->liveViewReady = liveViewReady;
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
        if(connected) {
            Eds::StartLiveview(camera);
            lock();
            lastResetTime = ofGetElapsedTimef();
            unlock();
        }
    }
    
    void Camera::initialize() {
        try {
            Eds::InitializeSDK();
            
            EdsCameraListRef cameraList;
            Eds::GetCameraList(&cameraList);
            
            EdsUInt32 cameraCount;
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
            } else {
                ofLogError() << "No cameras are connected for ofxEds::Camera::setup().";
            }
        } catch (Eds::Exception& e) {
            ofLogError() << "There was an error during Camera::setup(): " << e.what();
        }
    }
    
    void Camera::startCapture() {
        try {
            Eds::OpenSession(camera);
            connected = true;
            if(useLiveView) {
                Eds::StartLiveview(camera);
            }
        } catch (Eds::Exception& e) {
            ofLogError() << "There was an error opening the camera, or starting live view: " << e.what();
            return;
        }
        lastResetTime = ofGetElapsedTimef();
    }
    
    void Camera::stopCapture() {
        if(connected) {
            try {
                if(liveViewReady) {
                    Eds::EndLiveview(camera);
                    liveViewReady = false;
                }
                Eds::CloseSession(camera);
                Eds::TerminateSDK();
                connected = false;
            } catch (Eds::Exception& e) {
                ofLogError() << "There was an error closing ofxEds::Camera: " << e.what();
            }
        }
    }
    
    void Camera::captureLoop() {
        if(liveViewReady) {
            if(Eds::DownloadEvfData(camera, *liveBufferBack)) {
                lock();
                fps.tick();
                bytesPerFrame = ofLerp(bytesPerFrame, liveBufferBack->size(), .01);
                swap(liveBufferBack, liveBufferMiddle.back());
                liveBufferMiddle.push();
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
        
        if(needToStartRecording) {
            try {
                EdsUInt32 saveTo = kEdsSaveTo_Camera;
                EdsSetPropertyData(camera, kEdsPropID_SaveTo, 0, sizeof(saveTo) , &saveTo);
                
                EdsUInt32 record_start = 4; // Begin movie shooting
                EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_start), &record_start);
                lock();
                needToStartRecording = false;
                unlock();
            } catch (Eds::Exception& e) {
                ofLogError() << "Error while beginning to record: " << e.what();
            }
        }
        
        if(needToStopRecording) {
            try {
                EdsUInt32 record_stop = 0; // End movie shooting
                EdsSetPropertyData(camera, kEdsPropID_Record, 0, sizeof(record_stop), &record_stop);
                lock();
                needToStopRecording = false;
                unlock();
            } catch (Eds::Exception& e) {
                ofLogError() << "Error while stopping to record: " << e.what();
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
                EdsDirectoryItemInfo dirItemInfo = Eds::DownloadImage(directoryItem, photoBuffer);
                ofLogVerbose() << "Downloaded item: " << (int) (photoBuffer.size() / 1024) << " KB";
                lock();
                photoDataReady = true;
                needToDecodePhoto = true;
                needToUpdatePhoto = true;
                needToDownloadImage = false;
                
                if (dirItemInfo.format == OFX_EDSDK_JPG_FORMAT) {
                    photoNew = true;
                } else if (dirItemInfo.format == OFX_EDSDK_MOV_FORMAT) {
                    movieNew = true;
                }
                
                unlock();
            } catch (Eds::Exception& e) {
                ofLogError() << "Error while downloading item: " << e.what();
            }
        }
        
        float timeSinceLastReset = ofGetElapsedTimef() - lastResetTime;
        if(timeSinceLastReset > resetIntervalMinutes * 60) {
            resetLiveView();
        }
        
    }
    
    void Camera::threadedFunction() {
#if defined(TARGET_WIN32)
        CoInitializeEx(NULL, 0x0); // COINIT_APARTMENTTHREADED in SDK docs
#endif
        while(isThreadRunning()) {
            captureLoop();
            ofSleepMillis(5);
        }
#if defined(TARGET_WIN32)
        CoUninitialize();
#endif
    }
}