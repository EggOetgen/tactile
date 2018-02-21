#pragma once

#include "ofMain.h"
#include "ofVideoGrabber.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"
#include "ofxImGui.h"
#include "../hand.h"

//ip address of device that granularOsc is running on
#define HOST "10.100.26.13"
#define PORT 12000

//port for wekinator
#define MYPORT 12344


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	//variable to enable use of an external video (may not work anymore)
	bool useVideo = true;

	//video grabber for input
	ofVideoGrabber myInput;

	//opencv
	ofxCvColorImage colorImg;
	ofxCvColorImage colorImgDec;
	ofxCvGrayscaleImage grayImg;
	ofxCvContourFinder contourFinder;
	ofColor c;

	int var = 0;

	//senders to send osc messages. sender sends directly to granularOsc to generate sound, sender2 sends to wekinatorinputhelper
	ofxOscSender sender;
	ofxOscSender sender2;

	//gui object
	ofxImGui::Gui gui;

	//default values for image filtering.
	float brightness = 0.8916;
	float contrast = 0.8833;
	float blur = 4.823;
	float threshold = 244.341;

	//variables to enable and disable image filters via gui
	bool bcOn = true;
	bool bOn = true;
	bool tOn = true;

	//hand finder object
	hand handFinder;

	//shader
	ofShader shader;

	//variable that when enables shows shader. when false, gui is shown
	bool display = true;

};