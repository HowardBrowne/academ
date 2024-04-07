#include "testApp.h"

ofxEdsdk::RateTimer appFps, decodeFps;

void testApp::setup() {
	//ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	camera.setup();
}

void testApp::update() {
	camera.update();
	if(camera.isFrameNew()) {
		decodeFps.tick();
	}
	if(camera.isPhotoNew()) {
		camera.savePhoto(ofToString(ofGetFrameNum()) + ".jpg");
	}
	appFps.tick();
}

void testApp::draw() {
	camera.draw(0, 0);
	//camera.drawPhoto(0, 0, 432, 288);
	
	if(camera.isLiveReady()) {
		stringstream status;
		status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
			(int) appFps.getFrameRate() << " app-fps " << " / " <<
			(int) decodeFps.getFrameRate() << " decode-fps" << " / " <<
			(int) camera.getFrameRate() << " cam-fps";
		ofDrawBitmapString(status.str(), 10, 20);
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		camera.takePhoto();
	}
}