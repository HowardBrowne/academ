#pragma once

#include "ofMain.h"

#include "ofxEdsdk.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxEdsdk::Camera camera;
};
