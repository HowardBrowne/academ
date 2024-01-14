#include "testApp.h"

void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	camera.setup();
}

void testApp::update() {
	if(camera.isConnected()) {
		camera.update();
		if(camera.isFrameNew()) {
		}
	}
}

void testApp::draw() {
	camera.draw(0, 0);
	stringstream status;
	status << camera.getWidth() << "x" << camera.getHeight() << " @ " << (int) ofGetFrameRate() << " fps";
	ofDrawBitmapString(status.str(), 10, 20);
}
