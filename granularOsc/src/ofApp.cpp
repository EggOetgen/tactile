#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //--------------------------------------------------------------
    //GENERAL OPENFRAMEWORKS SETUP
    //--------------------------------------------------------------
    ofSetWindowShape(640, 640);
    ofSetFrameRate(60);
    
    //--------------------------------------------------------------
    //SET UP EVERYTHING FOR ofxOSC
    //--------------------------------------------------------------
    cout << "listening for osc messages on port " << FPORT << "\n";
    freceiver.setup(FPORT);
    greceiver.setup(GPORT);
    
    //--------------------------------------------------------------
    //SET UP EVERYTHING FOR ofxImGui
    //--------------------------------------------------------------
    // gui.setup();

    //--------------------------------------------------------------
    //SET UP EVERYTHING FOR ofxMAXIMILLIAN
    //--------------------------------------------------------------
    
    sampleRate 	= 44100; //set to 44100 according to nyquist
    bufferSize	= 512;
    
    lAudioOut			= new float[bufferSize];
    rAudioOut			= new float[bufferSize];
    lAudioIn			= new float[bufferSize];
    rAudioIn			= new float[bufferSize];
    
    
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    
    memset(lAudioOut, 0, bufferSize * sizeof(float));
    memset(rAudioOut, 0, bufferSize * sizeof(float));
    
    memset(lAudioIn,  0, bufferSize * sizeof(float));
    memset(rAudioIn,  0, bufferSize * sizeof(float));
    
   
    
        
    //--------------------------------------------------------------
    //LOAD UP ALL THE SYNTHS AND PUT THEM IN AN ARRAY
    //--------------------------------------------------------------

    string pad ("ice.wav");
    padSynth = new granSynth(pad, string(pad));
    synths.push_back(padSynth);
    
    string siren ("snare.wav");
    sirenSynth = new granSynth(siren, siren);
    synths.push_back(sirenSynth);
    
    string cage("cage.wav");
    vocalSynth = new granSynth(cage, cage);
    synths.push_back(vocalSynth);
  
    string noise("noise.wav");
    noiseSynth = new granSynth(noise, noise);
    synths.push_back(noiseSynth);
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //OSC CODE TAKEN FROM OSC EXAMPLES IN OFX
    // check for waiting messages
    while(freceiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        freceiver.getNextMessage(m);
        if(m.getAddress() == "/point"){
            
        
          //FIRST ARGUMENT TELLS US NUMBER OF FINGERS DETECTED
            numFings = m.getArgAsFloat(0);
          
         //IF THERE ISN'T ANY FINGERS TURN ALL THE SYNTHS OFF
            if (numFings ==0) {
                for (int i = 0; i < synths.size(); i++){
                    synths[i]->ampTrigger = 0;
                }
            }
            
            //AND IF THERE IS ANY FINGERS...
            else if (numFings !=0) {
               
                //GET RID OF DATA FROM LAST FRAME
                fingers.clear();
                sqrDist = 0;
                centroid.set(0,0);
                
                float dist;
                
                //LOAD THE POSTIONS OF ALL THE FINGERS INTO AN ARRAY
                for(int i = 1; i < numFings*2; i+=2){
                    ofPoint p(m.getArgAsFloat(i), m.getArgAsFloat(i+1));
                    fingers.push_back(p);
                }
             
                //FIND CENTROID BY AVERAGING ALL THE FINGER POSTIONS
                for(int i = 0; i < fingers.size(); i ++){
                    centroid += fingers[i];
                }
                centroid /= fingers.size();
               
                //FIND THE AVERAGE DISTANCE BETWEEN FINGERS AND CENTROID
                for(int i = 0; i < fingers.size(); i ++){
                    sqrDist +=centroid.squareDistance(fingers[i]);
                }
                
                dist = (sqrDist/numFings);

                //NUMBER OF FINGERS DETERMINES WHICH SYNTH TO OPERATE
                float picker = (numFings -1) % 4;
          
                //ADJUST PERIMETERS FOR CURRENT SYNTH, TURN OTHERS OFF
                for (int i = 0; i < synths.size(); i++){
               if(dist!=0) synths[i]->dlFeedback = dist*10;
                  
                    if(i == picker){
                synths[i]->ampTrigger=1;
                synths[i]->loop = true;
                synths[i]->adjustStart(centroid.x, 1);
                synths[i]->adjustSpeed(centroid.x*1.5, 1.5);
               
                    }else{
                        synths[i]->ampTrigger = 0;
                    }
                }
                
                
            }
            
        }
        
        
        
    }
    
    
 
    //SET TO LOAD ALL THE GESTURE DATA INTO AN ARRAY, BUT ONLY 2 WERE USED
    while(greceiver.hasWaitingMessages()){
        
        ofxOscMessage m;
        greceiver.getNextMessage(m);
        
        if(m.getAddress()=="/wek"){
            gestures.clear();
            gestures.push_back(m.getArgAsFloat(0));
            gestures.push_back(m.getArgAsFloat(1));
            gestures.push_back(m.getArgAsFloat(2));
            gestures.push_back(m.getArgAsFloat(3));
            gestures.push_back(m.getArgAsFloat(4));
            
           
            //NORMALISE THE VALUE SHOWING SIMILARITY TO GESTURE
            float circle = gestures[0];
            float zigzag = gestures[1];
            
           //WHEN A ZIGZAG IS DETECTED RANDOMISE THE PARAMETERS, WHEN THERE IS A CIRLCE SWITCH GRAI LOOPING ON AND OFF
            for (int i = 0; i < synths.size(); i++){
                if(zigzag < 35){
                    synths[i]->random = true;
                }
                if (circle < 68){
                    synths[i]->loop = true;
                }else{
                        synths[i]->loop = false;
                    }
            
                }
            }
        }
    
  
    for (int i = 0; i < synths.size(); i++){
        synths[i]->update();
    
          }
 
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    gui.begin();

    for (int i = 0; i < synths.size(); i++){
       
        synths[i]->display();
        
    }
    gui.end();
        

}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    for (int i = 0; i < bufferSize; i++){
    
             for (int j = 0; j < synths.size(); j++){
            wave += (synths[j]->play());
            wave*= 0.8;
             }

        mix.stereo(wave,outputs,0.5);
                lAudioOut[i] = lAudio[i] = output[i*nChannels    ] = outputs[0];
        rAudioOut[i] = rAudio[i] = output[i*nChannels + 1] = outputs[1];
    }
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    for(int i = 0; i < bufferSize; i++){
       
        
    }
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // YOU CAN USE THESE TO TRIGGER THE SAMPLES
    if(key =='a'){
        synths[0]->ampTrigger =1;
    }else if(key =='z'){
        synths[0]->ampTrigger =1;
        synths[0]->speedTrigger =1;
    }else if(key =='s'){
        synths[1]->ampTrigger =1;
    }else if(key =='x'){
        synths[1]->ampTrigger =1;
        synths[1]->speedTrigger =1;
    }else if(key =='c'){
        synths[2]->ampTrigger =1;
        synths[2]->speedTrigger =1;
    }else if(key =='d'){
        synths[2]->ampTrigger =1;
    }else if(key =='f'){
        synths[3]->ampTrigger =1;
    }else if(key =='v'){
        synths[3]->ampTrigger =1;
        synths[3]->speedTrigger =1;
    }

    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key =='a'||key =='z'){
        synths[0]->ampTrigger =0;
        synths[0]->speedTrigger =0;
    }else if(key =='s'||key =='x'){
        synths[1]->ampTrigger =0;
        synths[1]->speedTrigger =0;
    }else if(key =='d'||key =='c'){
        synths[2]->ampTrigger =0;
        synths[2]->speedTrigger =0;
    }else if(key =='f'||key =='v'){
        synths[3]->ampTrigger =0;
        synths[3]->speedTrigger =0;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
   
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    }

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


