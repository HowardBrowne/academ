#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	camera.setup();
}

void testApp::update() {
	camera.update();
	if(camera.isFrameNew()) {
		// process the live view with camera.getLivePixels()
	}
	if(camera.isPhotoNew()) {
		// process the photo with camera.getPhotoPixels()
		// or just save the photo to disk (jpg only):
		camera.savePhoto(ofToString(ofGetFrameNum()) + ".jpg");
	}
}

void testApp::draw() {
	camera.draw(0, 0);
	// camera.drawPhoto(0, 0, 432, 288);
	
	if(camera.isLiveReady()) {
		stringstream status;
			status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
			(int) ofGetFrameRate() << " app-fps " << " / " <<
			(int) camera.getFrameRate() << " cam-fps";
		ofDrawBitmapString(status.str(), 10, 20);
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		camera.takePhoto();
	}
}