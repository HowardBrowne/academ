#include "ofxEdsdk.h"

namespace ofxEdsdk {
	
	void handleError(EdsError err, string msg) {
		if(err != EDS_ERR_OK) {
			cout << msg << " returned " << getEdsErrorString(err) << endl;
		}
	}
	
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
		cout << "property event: " << getEdsPropertyString(propertyId) << " / " << param << endl;
		
		if(propertyId == kEdsPropID_Evf_OutputDevice) {
			((Camera*) context)->setLiveViewReady(true);
		}
	}
	
	EdsError startLiveview(EdsCameraRef camera) {
		EdsError err = EDS_ERR_OK;
		
		// Get the output device for the live view image
		EdsUInt32 device;
		err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
		
		// PC live view starts by setting the PC as the output device for the live view image.
		if(err == EDS_ERR_OK) {
			device |= kEdsEvfOutputDevice_PC;
			err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
		}
		
		// A property change event notification is issued from the camera if property settings are made successfully.
		// Start downloading of the live view image once the property change notification arrives.
	}
	
	EdsError downloadEvfData(EdsCameraRef camera, ofPixels& pixels) {
		EdsError err = EDS_ERR_OK;
		EdsStreamRef stream = NULL;
		EdsEvfImageRef evfImage = NULL;
		
		//	Create memory stream.
		// This automatically allocates the stream if it's unallocated.
		// If you want to save some time, avoid reallocation by keeping the EdsStreamRef around.
		// Alternatively, you can prepare the memory yourself and use EdsCreateMemoryStreamFromPointer.
		if(err == EDS_ERR_OK) {
			err = EdsCreateMemoryStream(0, &stream);
			handleError(err, "EdsCreateMemoryStream");
		}
		
		//	Create EvfImageRef.
		if(err == EDS_ERR_OK) {
			err = EdsCreateEvfImageRef(stream, &evfImage);
			handleError(err, "EdsCreateEvfImageRef");
		}
		
		// Download live view image data.
		if(err == EDS_ERR_OK) {
			err = EdsDownloadEvfImage(camera, evfImage);
			handleError(err, "EdsDownloadEvfImage");
		}
		
		// Get the image data.
		EdsUInt32 length;
		if(err == EDS_ERR_OK) {
			EdsGetLength(stream, &length);
			handleError(err, "EdsGetLength");
		}
		
		char* streamPointer;
		if(err == EDS_ERR_OK) {
			EdsGetPointer(stream, (EdsVoid**) &streamPointer);
			handleError(err, "EdsGetPointer");
		}
		
		if(err == EDS_ERR_OK) {
			cout << "Copying image (" << length << ") to ofBuffer" << endl;
			ofBuffer imageBuffer;
			imageBuffer.set(streamPointer, length);
			
			ofLoadImage(pixels, imageBuffer);
		}
		
		/*
		 // Get the metadata of the image.
		 // Get the zoom ratio
		 EdsUInt32 zoom;
		 EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0 , sizeof(zoom), &zoom);
		 
		 // Get the focus and zoom border position
		 EdsPoint point;
		 EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0 , sizeof(point), &point);
		 */
		
		// Release stream
		if(stream != NULL) {
			EdsRelease(stream);
			handleError(err, "EdsRelease");
			stream = NULL;
		}
		
		// Release evfImage
		if(evfImage != NULL) {
			EdsRelease(evfImage);
			handleError(err, "EdsRelease");
			evfImage = NULL;
		}
		
		return err;
	}
	
	EdsError endLiveview(EdsCameraRef camera) {
		EdsError err = EDS_ERR_OK;
		// Get the output device for the live view image
		EdsUInt32 device;
		err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device );
		// PC live view ends if the PC is disconnected from the live view image output device.
		if(err == EDS_ERR_OK) {
			device &= ~kEdsEvfOutputDevice_PC;
			err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
		}
	}
	
	Camera::Camera() :
	liveViewReady(false),
	frameNew(false) {
	}
	
	Camera::~Camera() {
		if(liveViewReady) {
			endLiveview(camera);
		}
		
		err = EdsCloseSession(camera);
		handleError(err, "EdsCloseSession");
		
		err = EdsTerminateSDK();
		handleError(err, "EdsTerminateSDK");
	}
	
	void Camera::setup() {
		cout << "EdsInitializeSDK()" << endl;
		err = EdsInitializeSDK();
		handleError(err, "EdsInitializeSDK");
		
		EdsCameraListRef cameraList;
		err = EdsGetCameraList(&cameraList);
		
		EdsUInt32 cameraCount;
		cout << "EdsGetChildCount()" << endl;
		err = EdsGetChildCount(cameraList, &cameraCount);
		handleError(err, "EdsGetChildCount");
		
		cout << "Camera count: " << cameraCount << endl;
		
		EdsInt32 cameraIndex = 0;
		cout << "EdsGetChildAtIndex()" << endl;
		err = EdsGetChildAtIndex(cameraList, cameraIndex, &camera);
		handleError(err, "EdsGetChildAtIndex");
		
		cout << "EdsSetPropertyEventHandler()" << endl;
		err = EdsSetPropertyEventHandler(camera,	kEdsPropertyEvent_All, handlePropertyEvent, this);
		handleError(err, "EdsSetPropertyEventHandler");
		
		cout << "EdsOpenSession()" << endl;
		err = EdsOpenSession(camera);
		handleError(err, "EdsOpenSession");
		
		cout << "starting live view" << endl;
		startLiveview(camera);
	}
	
	void Camera::update() {
		if(liveViewReady) {
			cout << "downloadEvfData()" << endl;
			err = downloadEvfData(camera, livePixels);
			if(err == EDS_ERR_OK) {
				frameNew = true;
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
	
	void Camera::setLiveViewReady(bool liveViewReady) {
		this->liveViewReady = liveViewReady;
	}
}