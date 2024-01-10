#include "testApp.h"

void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	camera.setup();
}

void testApp::update() {
	if(camera.isConnected()) {
		cout << "update()" << endl;
		camera.update();
		if(camera.isFrameNew()) {
			cout << "new frame" << endl;	
		}
	}
}

void testApp::draw() {
	camera.draw(0, 0);
}
