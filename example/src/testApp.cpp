#include "testApp.h"

ofxEdsdk::RateTimer camFps;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	camera.setup();
}

void testApp::update() {
	camera.update();
	if(camera.isFrameNew()) {
		camFps.tick();
	}
}

void testApp::draw() {
	camera.draw(0, 0);
	if(camera.isReady()) {
		stringstream status;
		status << camera.getWidth() << "x" << camera.getHeight() << " @ " <<
			(int) ofGetFrameRate() << " app-fps " << " / " <<
			(int) camFps.getFrameRate() << " app-cam-fps" << " / " <<
			(int) camera.getFrameRate() << " cam-cam-fps";
		ofDrawBitmapString(status.str(), 10, 20);
	}
}
