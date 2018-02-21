#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxOsc.h"
#include "ofxImGui.h"

#include "granSynth.hpp"


//OSC:
#define HOST "localhost"
#define FPORT 12000
#define GPORT 13000


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    
    void audioOut(float * output, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    int sampleRate;
    int bufferSize;
    
    ofxImGui::Gui gui;
    
    float 	* lAudioOut; /* outputs */
    float   * rAudioOut;
    
    float * lAudioIn; /* inputs */
    float * rAudioIn;
    
    vector <float> lAudio;
    vector <float> rAudio;
    double outputs[2];
    
    float wave;

    
    //fingers
    int numFings;
    vector<ofVec2f> fingers, pfingers;
    ofPoint centroid;
    //gestures
    vector<float>gestures;
    
    

    maxiMix mix;
    maxiSample samp;

    
    //OSC sender:
    ofxOscReceiver freceiver;
    ofxOscReceiver greceiver;
    
    
    granSynth *padSynth, *sirenSynth, *vocalSynth, *noiseSynth;
    vector <granSynth*> synths;

    float sqrDist;
};
