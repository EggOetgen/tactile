//
//  granSynth.hpp
//  granularOsc
//
//  Created by Edmund Oetgen on 12/03/2017.
//
//

#pragma once
#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxImGui.h"

class granSynth{
    
public:
    float play();
    void display();
    void update();
    void adjustStart(float picker, float range =1);
    void adjustVolume(float picker, float range = 1);
    void adjustSpeed(float picker, float range = 1);
    void adjustRate(float picker, float range = 1);
    void adjustGrainLength(float picker, float range = 1);
    float applyEnvelope(float input, maxiEnv &env, int &trigger, const float floor,const float ceil);
     void reset();
    
    float wave;
    float pos;
    float grainLength;
    float speed;
    float rate;
    
    maxiSample samp;
    maxiTimePitchStretch<hannWinFunctor, maxiSample>* granular;
    
    maxiDelayline delay;
    maxiEnv ampEnv;
    int ampTrigger;
    
    maxiEnv speedEnv;
    int speedTrigger;
    float vol, vol2;
    
    float dlFeedback;
    int dlSize;
    
    double cutoff, res;
    maxiFilter lp;
    maxiFilter hp;
    
    //granSynth(maxiSample samp);
     granSynth(string sampFilePath, string name_);
    string name;
    
       
    
    
   // ofxImGui::Gui gui;
    bool onOff, random, loop;


};
