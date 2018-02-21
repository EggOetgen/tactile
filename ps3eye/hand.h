#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
class hand {

public:

	vector<ofVec3f> fingers;

	float allowance = 5;

	ofVec3f pCent;
	ofVec3f cent;

	hand();

	void addFinger();

	ofVec3f getVel();

	void findFingers(ofxCvGrayscaleImage &img);

	ofVec3f getCentroid();
	
	float getAvgDistance();


};