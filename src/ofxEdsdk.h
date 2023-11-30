#pragma once

/*
	ofxEdsdk lets OF talk to Canon cameras using a simple interface.
*/

#include "ofMain.h"
#include "EdsdkWrapper.h"

#define safe(x, y) if(err == EDS_ERR_OK) {err = y; handleError(err, x);}

namespace ofxEdsdk {
	
	/*
		Camera is currently not threaded, so
		update() blocks and isFrameNew() is always true.
	*/
	
	class Camera {
	public:
		Camera();
		~Camera();
		void setup();
		void update();
		bool isFrameNew();
		
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);
		
		const ofPixels& getPixelsRef() const;
		ofPixels& getPixelsRef();
		
		bool isConnected() const;
		void setLiveViewReady(bool liveViewReady);
		
	protected:
	
		EdsError err;
		EdsCameraListRef cameraList;
		EdsCameraRef camera;
		
		ofPixels livePixels;
		ofTexture liveTexture;
		bool connected;
		bool liveViewReady;
		bool frameNew;
	};
	
}