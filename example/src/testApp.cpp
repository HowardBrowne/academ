#include "testApp.h"

void testApp::setup() {	
	ofSetVerticalSync(true);
	camera.setup();
}

void testApp::update() {
	camera.update();
	if(camera.isFrameNew()) {
	}
}

void testApp::draw() {
	camera.draw(0, 0);
}
