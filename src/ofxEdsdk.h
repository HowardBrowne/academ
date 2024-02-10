#pragma once

/*
	ofxEdsdk lets OF talk to Canon cameras using a simple interface.
*/

#include "ofMain.h"
#include "EdsWrapper.h"
#include "RateTimer.h"

namespace ofxEdsdk {

	class Camera : public ofThread {
	public:
		Camera();
		~Camera();
		void setup(int deviceId = 0);
		void update();
		bool isFrameNew();
		float getFrameRate();
		
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);
		
		const ofPixels& getPixelsRef() const;
		ofPixels& getPixelsRef();
		
		bool isReady() const;
		
		void setLiveViewReady(bool liveViewReady);
		void keepAlive();
		
	protected:
		EdsCameraRef camera;
		
		RateTimer fps;
		
		ofBuffer liveBufferBack, liveBufferMiddle, liveBufferFront;
		ofPixels livePixels;
		ofTexture liveTexture;
		
		// There are a few important state variables used for keeping track of what
		// is and isn't ready, and syncing data in different threads.
		bool connected; // camera is valid, OpenSession was successful, you can use Eds(camera) now.
		bool liveViewReady; // Live view is initialized and connected, ready for downloading.
		bool liveViewDataReady; // Live view data has been downloaded at least once by threadedFunction().
		bool frameNew; // There has been a new frame since the user last checked isFrameNew().
		bool needToUpdate; // There is new live view data available for uploading in update().
		
		void threadedFunction();
	};
	
}