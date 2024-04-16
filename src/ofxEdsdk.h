#pragma once

/*
 ofxEdsdk lets OF talk to Canon cameras using a simple interface.
 */

#include "ofMain.h"
#include "EdsWrapper.h"
#include "RateTimer.h"
#include "FixedQueue.h"

namespace ofxEdsdk {
	
	class Camera : public ofThread {
	public:
		Camera();
		~Camera();
		void setup(int deviceId = 0);
		
		void update();
		bool isFrameNew();
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		bool isLiveReady() const;
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);
		ofPixels& getLivePixels();
		float getFrameRate();
		
		void takePhoto();
		bool isPhotoNew();
		void drawPhoto(float x, float y);
		void drawPhoto(float x, float y, float width, float height);
		void savePhoto(string filename);
		ofPixels& getPhotoPixels();
		
	protected:
		EdsCameraRef camera;
		
		RateTimer fps;
		
		ofBuffer liveBufferBack, liveBufferMiddle, liveBufferFront;
		ofPixels livePixels;
		ofTexture liveTexture;
		
		ofBuffer photoBuffer;
		ofPixels photoPixels;
		ofTexture photoTexture;
		
		// There are a few important state variables used for keeping track of what
		// is and isn't ready, and syncing data between different threads.
		bool connected; // camera is valid, OpenSession was successful, you can use Eds(camera) now.
		bool liveReady; // Live view is initialized and connected, ready for downloading.
		bool liveDataReady; // Live view data has been downloaded at least once by threadedFunction().
		bool frameNew; // There has been a new frame since the user last checked isFrameNew().
		bool needToUpdate; // There is new live view data available for uploading in update().
		bool needToTakePhoto; // threadedFunction() should take a picture next chance it gets.
		bool photoNew; // There is a new photo since the user last checked isPhotoNew().
		bool needToDecodePhoto; // The photo pixels needs to be decoded from photo buffer.
		bool needToUpdatePhoto; // The photo texture needs to be updated from photo pixels.
		bool photoDataReady; // Photo data has been downloaded at least once.
		
		void threadedFunction();
		
		static EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid* context);
		static EdsError EDSCALLBACK handlePropertyEvent(EdsPropertyEvent event, EdsPropertyID propertyId, EdsUInt32 param, EdsVoid* context);
		static EdsError EDSCALLBACK handleCameraStateEvent(EdsStateEvent event, EdsUInt32 param, EdsVoid* context);
		
		void setLiveReady(bool liveViewReady);
		void sendKeepAlive();
		void downloadImage(EdsDirectoryItemRef directoryItem);
	};
	
}