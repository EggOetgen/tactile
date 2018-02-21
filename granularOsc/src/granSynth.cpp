//
//  granSynth.cpp
//  granularOsc
//
//  Created by Edmund Oetgen on 12/03/2017.
//
//

#include "granSynth.hpp"


granSynth::granSynth(string sampFilePath, string name_){
    

    name = name_;
    
    vol = 1.0f;
    pos = 0.0f;
    speed = 1.0f;
    rate = 1.0f;
    grainLength = 0.2f;
    dlFeedback = 0.0;
    dlSize = 44100;
    onOff =  true;
    loop = random = false;


    samp.load(ofToDataPath(sampFilePath));
    
    granular = new maxiTimePitchStretch<hannWinFunctor, maxiSample>(&samp);
    
    
    
    ampTrigger = speedTrigger = 0;
  
    
    ampEnv.setAttack(5);
    ampEnv.setDecay(2);
    ampEnv.setSustain(0.7);
    ampEnv.setRelease(2.5);
    
    speedEnv.setAttack(5);
    speedEnv.setDecay(2);
    speedEnv.setSustain(0.5);
    speedEnv.setRelease(1);
    

    
}

float granSynth::play(){
    
    if(onOff)
    wave = granular->play(speed, rate, grainLength,4, 0);
    else wave = 0;
    
    wave += delay.dl(wave, dlSize, dlFeedback);
    wave *= vol2;
    wave *= vol;
    
    return wave;
}

void granSynth::update(){
    
    if(random){
       float nPos = ofRandom(0.001,1.0);
       float nSpeed = ofRandom(0.2f,2.0f);
       float nRate = ofRandom(-1.0f,1.0f);
       dlFeedback = ofRandom(0.f,1.f);
       dlSize = ofRandom(512, 88200);
        
        adjustStart(nPos);
        adjustSpeed(nSpeed);
        adjustRate(nRate);
        
        
        random = !random;
    }
    
    if(loop) adjustStart(pos, 1);
   
    
    
   
    
   vol2 = applyEnvelope(1, ampEnv,ampTrigger, 0, 2);


   
}

void granSynth::display(){
    
    //EACH SYNTH HAS ITS OWN GUI, BUT ACTUALLY ALL PART OF SAME ONE INITIALISED IN OFAPP

     ImGui::SetNextWindowSize(ofVec2f(200,350), ImGuiSetCond_FirstUseEver);
    bool t = true;
    ImGui::Begin(name.c_str(), &t);

    
    {
        ImGui::Text("Synth Controls");
        
        if(ImGui::Button("On"))
        {
            onOff = !onOff;
        }
        
        ImGui::SliderFloat("Volume", &vol, 0.1f, 1.0f);


        if(ImGui::Button("Random"))
        {
            random = !random;
        }
        
        if(ImGui::Button("Loop"))
        {
            loop = !loop;
        }
        ImGui::SliderFloat("Position", &pos, 0.001f, 1.0f);
        
        ImGui::SliderFloat("Pitch", &speed, 0.2f, 1.5f);
        
        
        ImGui::SliderFloat("Rate", &rate, -1.0f, 1.0f);


        ImGui::SliderFloat("grainLength", &grainLength, 0.2f, 4.f);
        
        
        ImGui::SliderFloat("Delay Feedback", &dlFeedback, 0.0f, 0.9f);
        
        
        ImGui::SliderInt("Delay Rate", &dlSize, 512, 88200);

        
        
    }
     ImGui::End();

    
}

//USE THESE TO SET THE PARMAETERS JUST TO MAKE SURE THEY DON'T GET TO WHEER THEY SHOULDN'T AND HURT YOUR EARS!

void granSynth::adjustVolume(float picker, float range){
    
    picker = picker/range;
    
    vol = picker;
    
}


void granSynth::adjustStart(float picker, float range){

    picker = picker/range;
  
    if (picker<0) picker = 0.0;
    if (picker>1) picker = 1.0;
    pos = picker;
    granular->setPosition(picker);

}

void granSynth::adjustSpeed(float picker, float range){

    picker = picker/range;
    if (picker<0.2) picker = 0.2;
    if (picker>1.5) picker = 1.5;
    speed = picker;

}

void granSynth::adjustRate(float picker, float range){

    picker = picker/range;
    if (picker<-1.0) picker = -1.0;
    if (picker>1.0) picker = 1.0;
    rate = picker;

}

void granSynth::adjustGrainLength(float picker, float range){

    picker = picker/range;
   
   // granular->setPosition(pos);
    if (picker<0.02) picker = 0.2;
    if (picker>0.4) picker = 0.4;
    
    grainLength = picker;
   

}

//pass the envelope by reference because we want to effect the actual object
float granSynth::applyEnvelope(float input, maxiEnv &env, int &trigger,const float floor, const float ceil){

    float out;
    
    float e = env.adsr(ceil, trigger);
    if(e<floor)e = floor;
    if(e>ceil) e = ceil;

    
   // trigger = 0;
    
    out = input * e;
    if(out<floor)e = floor;
    if(out>ceil) e = ceil;

    return out;

}

void granSynth::reset(){
    
    adjustSpeed(1);
    adjustRate(1);
   
    
}




