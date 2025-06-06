#pragma once

#include "ofMain.h"

#include "ofxEdsdk.h"

class ofApp : public ofBaseApp {
public:
	void setup();
    void exit();
	void update();
	void draw();
	void keyPressed(int key);
	
	ofxEdsdk::Camera camera;
    
    bool bIsRecordingMovie;
};
