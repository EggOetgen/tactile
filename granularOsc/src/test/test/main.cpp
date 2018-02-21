//
//  main.cpp
//  test
//
//  Created by Edmund Oetgen on 14/03/2017.
//  Copyright © 2017 Edmund Oetgen. All rights reserved.
//

#include <iostream>

#include "maximilian.h"

maxiOsc myCounter,mySwitchableOsc;//
int CurrentCount;//
double myOscOutput,myCurrentVolume;//
maxiEnv myEnvelope;


void setup() {//some inits
    
    //Timing is in ms
    
    myEnvelope.setAttack(0);
    myEnvelope.setDecay(1);  // Needs to be at least 1
    myEnvelope.setSustain(1);
    myEnvelope.setRelease(1000);
    
}

void play(double *output) {
    
    //notice that we feed in a value of 1. to create an envelope shape we can apply later.
    myCurrentVolume=myEnvelope.adsr(1.,myEnvelope.trigger);
    
    CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
    
    // You'll notice that these 'if' statements don't require curly braces "{}".
    // This is because there is only one outcome if the statement is true.
    
    if (CurrentCount==1) myEnvelope.trigger=1; //trigger the envelope
    
    else myEnvelope.trigger=0;//release the envelope to make it fade out only if it's been triggered
    
    if (CurrentCount<5)
        
        myOscOutput=mySwitchableOsc.sawn(CurrentCount*100);
    
    else if (CurrentCount>=5)//and the 'else' bit.
        
        myOscOutput=mySwitchableOsc.sinewave(CurrentCount*50);//one osc object can produce whichever waveform you want.
    
    
    output[0]=myOscOutput*myCurrentVolume;//left speaker
    output[1]=output[0];
    
}
