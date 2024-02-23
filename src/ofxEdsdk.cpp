#include "ofxEdsdk.h"

namespace ofxEdsdk {
	
	// from EDSDK API sample 6.3.6
	void downloadImage(EdsDirectoryItemRef directoryItem) {
		EdsStreamRef stream = NULL;
		
		// Get directory item information
		EdsDirectoryItemInfo dirItemInfo;
		try {
			Eds::GetDirectoryItemInfo(directoryItem, &dirItemInfo);
			Eds::CreateFileStream(dirItemInfo.szFileName, kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
			Eds::Download(directoryItem, dirItemInfo.size, stream);
			Eds::DownloadComplete(directoryItem);
			Eds::Release(stream);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error downloading the image: " << e.what();
		}
	}
	
	// from EDSDK API sample 6.3.7
	void getVolume(EdsCameraRef camera, EdsVolumeRef* volume) {
		
	}
	
	// from EDSDK API sample 6.3.10
	void startLiveview(EdsCameraRef camera) {
		try {
			// Get the output device for the live view image
			EdsUInt32 device;
			Eds::GetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// PC live view starts by setting the PC as the output device for the live view image.
			device |= kEdsEvfOutputDevice_PC;
			Eds::SetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// A property change event notification is issued from the camera if property settings are made successfully.
			// Start downloading of the live view image once the property change notification arrives.
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error starting live view: " << e.what();
		}
	}
	
	// from EDSDK API sample 6.3.10
	bool downloadEvfData(EdsCameraRef camera, ofBuffer& imageBuffer) {
		EdsStreamRef stream = NULL;
		EdsEvfImageRef evfImage = NULL;
		bool frameNew = false;
		
		try {
			//	Create memory stream.
			// This automatically allocates the stream if it's unallocated.
			// If you want to save some time, avoid reallocation by keeping the EdsStreamRef around.
			// Alternatively, you can prepare the memory yourself and use EdsCreateMemoryStreamFromPointer.
			Eds::CreateMemoryStream(0, &stream);
			
			//	Create EvfImageRef.
			Eds::CreateEvfImageRef(stream, &evfImage);
			
			// Download live view image data.
			Eds::DownloadEvfImage(camera, evfImage);
			
			// Get the image data.
			EdsUInt32 length;
			Eds::GetLength(stream, &length);
			
			char* streamPointer;
			Eds::GetPointer(stream, (EdsVoid**) &streamPointer);
			
			imageBuffer.set(streamPointer, length);
			
			frameNew = true;
		} catch (Eds::Exception& e) {
			if(e != EDS_ERR_OBJECT_NOTREADY) {
				ofLogError() << "There was an error downloading the live view data:" << e.what();
			}
		}
		
		try {
			// Release stream
			if(stream != NULL) {
				Eds::Release(stream);
				stream = NULL;
			}
			
			// Release evfImage
			if(evfImage != NULL) {
				Eds::Release(evfImage);
				evfImage = NULL;
			}
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error releasing the live view data: " << e.what();
		}
		
		return frameNew;
	}
	
	// from EDSDK API sample 6.3.10
	void endLiveview(EdsCameraRef camera) {
		try {		
			// Get the output device for the live view image
			EdsUInt32 device;
			Eds::GetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// PC live view ends if the PC is disconnected from the live view image output device.
			device &= ~kEdsEvfOutputDevice_PC;
			Eds::SetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
		} catch (Eds::Exception& e) {
			ofLogError() << "There was an error closing the live view stream: " << e.what();
		}
	}
	
	EdsError EDSCALLBACK Camera::handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context) {
		ofLogVerbose() << "object event " << Eds::getObjectEventString(event);
		if(object) {
			try {
				Eds::Release(object);
			} catch (Eds::Exception& e) {
				ofLogError() << "Error while releasing EdsBaseRef* inside handleObjectEvent()";
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
			((Camera*) context)->sendKeepAlive();
		}
		return EDS_ERR_OK;
	}
	
	Camera::Camera() :
	connected(false),
	liveViewReady(false),
	liveViewDataReady(false),
	frameNew(false),
	needToUpdate(false),
	needToTakePicture(false) {
	}
	
	Camera::~Camera() {
		waitForThread();
		lock();
		if(connected) {
			if(liveViewReady) {
				endLiveview(camera);
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
				Eds::Release(cameraList);
				
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
			liveViewDataReady = true;
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
	
	float Camera::getFrameRate() {
		float frameRate;
		if(lock()) {
			frameRate = fps.getFrameRate();
			unlock();
		}
		return frameRate;
	}
	
	void Camera::takePicture() {
		lock();
		needToTakePicture = true;
		unlock();
	}
	
	const ofPixels& Camera::getPixelsRef() const {
		return livePixels;
	}
	
	ofPixels& Camera::getPixelsRef() {
		return livePixels;
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
		if(liveViewDataReady) {
			liveTexture.draw(x, y, width, height);
		}
	}
	
	bool Camera::isReady() const {
		return liveViewDataReady;
	}
	
	void Camera::setLiveViewReady(bool liveViewReady) {
		// this is done before the threaded loop starts
		this->liveViewReady = liveViewReady;
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
	
	void Camera::threadedFunction() {
		if(lock()) {
			try {
				Eds::OpenSession(camera);
				connected = true;
				startLiveview(camera);
			} catch (Eds::Exception& e) {
				ofLogError() << "There was an error opening the camera, or starting live view: " << e.what();
			}
			unlock();
		}
		
		// threaded variables:
		// liveViewReady, needToUpdate, liveBufferMiddle, liveBufferBack, fps
		while(isThreadRunning()) {
		
			if(liveViewReady) {
				if(downloadEvfData(camera, liveBufferBack)) {
					lock();
					needToUpdate = true;
					liveBufferMiddle.set(liveBufferBack.getBinaryBuffer(), liveBufferBack.size()); // liveBufferMiddle = liveBufferBack;
					fps.tick();
					unlock();
				}
				
				lock();
				if(needToTakePicture) {
					unlock();
					try {
						Eds::SendCommand(camera, kEdsCameraCommand_TakePicture, 0);
						lock();
						needToTakePicture = false;
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