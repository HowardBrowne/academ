#include "ofxEdsdk.h"

namespace ofxEdsdk {
	
	/*
	 After calling startLiveView, we get:
	 property event: kEdsPropID_Evf_OutputDevice / 0
	 property event: kEdsPropID_Evf_DepthOfFieldPreview / 0
	 property event: kEdsPropID_MeteringMode / 0
	 property event: kEdsPropID_FocusInfo / 0
	 property event: kEdsPropID_AFMode / 0
	 property event: kEdsPropID_ExposureCompensation / 0
	 property event: kEdsPropID_AFMode / 0
	 property event: kEdsPropID_MeteringMode / 0
	 */
	EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID propertyId, EdsUInt32 param, EdsVoid* context) {
		cout << "property event: " << Eds::getPropertyString(propertyId) << " / " << param << endl;
		
		if(propertyId == kEdsPropID_Evf_OutputDevice) {
			((Camera*) context)->setLiveViewReady(true);
		}
	}
	
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
			stringstream err;
			err << "There was an error starting live view: " << e.what() << endl;
			ofLog(OF_LOG_ERROR, err.str());
		}
	}
	
	bool downloadEvfData(EdsCameraRef camera, ofPixels& pixels) {
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
			
			cout << "Copying image (" << length << ") to ofBuffer" << endl;
			ofBuffer imageBuffer;
			imageBuffer.set(streamPointer, length);
			
			ofLoadImage(pixels, imageBuffer);
			
			frameNew = true;
		} catch (Eds::Exception& e) {
			stringstream err;
			err << "There was an error downloading the live view data:" << e.what() << endl;
			ofLog(OF_LOG_ERROR, err.str());
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
			stringstream err;
			err << "There was an error releasing the live view data: " << e.what() << endl;
			ofLog(OF_LOG_ERROR, err.str());
		}
		
		return frameNew;
	}
	
	void endLiveview(EdsCameraRef camera) {
		try {
			// Get the output device for the live view image
			EdsUInt32 device;
			Eds::GetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
			
			// PC live view ends if the PC is disconnected from the live view image output device.
			device &= ~kEdsEvfOutputDevice_PC;
			Eds::SetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
		} catch (Eds::Exception& e) {
			stringstream err;
			err << "There was an error closing the live view stream: " << e.what() << endl;
			ofLog(OF_LOG_ERROR, err.str());
		}
	}
	
	Camera::Camera() :
	connected(false),
	liveViewReady(false),
	frameNew(false) {
	}
	
	Camera::~Camera() {
		if(connected) {
			if(liveViewReady) {
				endLiveview(camera);
			}
			
			try {
				Eds::CloseSession(camera);
				Eds::TerminateSDK();
			} catch (Eds::Exception& e) {
				stringstream err;
				err << "There was an error destroying ofxEds::Camera: " << e.what() << endl;
				ofLog(OF_LOG_ERROR, err.str());
			}
		}
	}
	
	void Camera::setup() {
		try {
			Eds::InitializeSDK();
			
			EdsCameraListRef cameraList;
			Eds::GetCameraList(&cameraList);
			
			UInt32 cameraCount;
			Eds::GetChildCount(cameraList, &cameraCount);
			
			if(cameraCount > 0) {				
				EdsInt32 cameraIndex = 0;
				Eds::GetChildAtIndex(cameraList, cameraIndex, &camera);
				Eds::SetPropertyEventHandler(camera,	kEdsPropertyEvent_All, handlePropertyEvent, this);
				Eds::OpenSession(camera);
				startLiveview(camera);
				
				connected = true;
			} else {
				ofLog(OF_LOG_ERROR, "No cameras are connected for ofxEds::Camera::setup().");
			}
		} catch (Eds::Exception& e) {
			stringstream errMsg;
			errMsg << "There was an error during Camera::setup(): " << e.what();
			ofLog(OF_LOG_ERROR, errMsg.str());
		}
	}
	
	void Camera::update() {
		if(liveViewReady) {
			cout << "downloadEvfData()" << endl;
			frameNew = downloadEvfData(camera, livePixels);
			if(frameNew) {
				if(liveTexture.getWidth() != livePixels.getWidth() ||
					 liveTexture.getHeight() != livePixels.getHeight()) {
					liveTexture.allocate(livePixels.getWidth(), livePixels.getHeight(), GL_RGB8);
				}
				liveTexture.loadData(livePixels);
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
		if(liveViewReady) {
			ofPushMatrix();
			ofTranslate(x, y);
			liveTexture.draw(0, 0, width, height);
			stringstream status;
			status << livePixels.getWidth() << "x" << livePixels.getHeight() << " @ " << (int) ofGetFrameRate() << " fps";
			ofDrawBitmapString(status.str(), 10, 20);
			ofPopMatrix();
		}
	}
	
	bool Camera::isConnected() const {
		return connected;
	}
	
	void Camera::setLiveViewReady(bool liveViewReady) {
		this->liveViewReady = liveViewReady;
	}
}