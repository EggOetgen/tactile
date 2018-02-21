
#include "ofApp.h"
#include "cv.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//setting up the osc senders
	sender.setup(HOST, PORT);
	sender2.setup("localhost", MYPORT);

	ofSetFrameRate(60);	
	
	//setting the webcam input for the ps3eye camera. Id may be different depending on device
	myInput.setVerbose(true);
	myInput.setDeviceID(2);
	myInput.initGrabber(320, 240);

	//gui setup
	gui.setup();
	ImGui::GetIO().MouseDrawCursor = false;

	//allocating an image to take the input from the webcam
	colorImg.allocate(myInput.getWidth(), myInput.getWidth());

	//loading the folder with the shaders
	shader.load("shader/shader");
}
//--------------------------------------------------------------
void ofApp::update() {
	
	//updating the webcam
	myInput.update();

	if (myInput.isFrameNew()) {

		//setting the colorImg to the webcam image if there is a new frame
		colorImg.setFromPixels(myInput.getPixels(), myInput.getWidth(), myInput.getHeight());
		
		//creating a gray image to filter
		grayImg = colorImg;
		
		//dilating the image. Expands areas of color to improve detection by opencv
		grayImg.dilate_3x3();

		//settings to filter image, controlled by the gui, and switch on and off from the gui
		if (bOn) grayImg.blur(blur);
		if (bcOn) grayImg.brightnessContrast(brightness, contrast);
		if (tOn) grayImg.threshold(threshold);

		//using the handfinder object to find the fingers
		handFinder.findFingers(grayImg);

	}


}

//--------------------------------------------------------------
void ofApp::draw() {
	
	//drawing the color image and the filtered image
	ofSetHexColor(0xffffff);

	colorImg.draw(colorImg.getWidth(), 0);
	grayImg.draw(0, 0); 

	ofSetColor(ofColor(255, 0, 255));

	
	//drawing red circles where fingers are detected, and a green circle for the centroid of all points
	for (int i = 0; i< handFinder.fingers.size(); i++) {

		ofSetColor(255, 0, 0);
		ofDrawEllipse(handFinder.fingers[i], 10, 10);
		ofSetColor(0, 255, 0 );
		ofDrawEllipse(handFinder.getCentroid(), 10, 10);
	}
	

	
	ofVec2f sum;
	sum.set(0, 0);

	
	//osc message for granularOsc
	ofxOscMessage m;
	m.setAddress("/point");

	//adding first element as the number of fingers to send
	m.addFloatArg(handFinder.fingers.size());

	//adding each finger's x and y values
	for (int i = 0; i < handFinder.fingers.size(); i++) {
		
		cout <<  handFinder.fingers[i].x / myInput.getWidth() << " " << handFinder.fingers[i].y / myInput.getHeight() << "\n";

		m.addFloatArg(handFinder.fingers[i].x /myInput.getWidth());
		m.addFloatArg(handFinder.fingers[i].y /myInput.getHeight());

	}

	//sending the osc message
	sender.sendMessage(m, false);

	//creating a message to send to wekinator of the centroid x and y position
	ofxOscMessage m2;
	m2.setAddress("/cent");
	m2.addFloatArg(handFinder.getCentroid().x);
	m2.addFloatArg(handFinder.getCentroid().y);

	//sending centroid to wekinator
	sender2.sendMessage(m2, false);

	
	
	//creating the gui
	bool t = true;
	gui.begin();
	ImGui::SetNextWindowSize(ofVec2f(500, 700));
	ImGui::Begin("parameters", &t);

	//small button to toggle blur
	if (ImGui::SmallButton("Blur on")) {
		bOn = !bOn;
	}
	//slider to change blur ammount
	ImGui::SliderFloat("Blur", &blur, 0, 100);

	//button to toggle brigness/contrast
	if (ImGui::SmallButton("Brightness/Contrast on")) {
		bcOn = !bcOn;
	}
		
	//slider to change brigtness/contrast
	ImGui::SliderFloat("Brigntness", &brightness, -1, 1);
	ImGui::SliderFloat("Contrast", &contrast, -1, 1);

	//button to toggle threshold
	if (ImGui::SmallButton("Threshold on")) {
		tOn = !tOn;
	}
	//slider ot change threshold
	ImGui::SliderFloat("Threshold", &threshold, 0, 255);

	ImGui::End();
	gui.end();


	//when drawing the shader, it is drawn over the gui
	if (display) {
		//creating a background white rectangle
		ofSetColor(255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		
		//starting the shader.
		shader.begin();

		
		//passing the screen resolution and mouse position as vec2s to the shader
		shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		shader.setUniform2f("mouse", mouseX, mouseY);
		
		//passing the time in milliseconds as a float
		shader.setUniform1f("time", ofGetElapsedTimeMillis());

		//as C arrays can not have a dynamic size when they are created, i always send 5 vec2s to the program. if a finger does not exist in the program, a position off the screen is sent
		if (handFinder.fingers.size() > 0) {
			shader.setUniform2f("f1", handFinder.fingers[0].x, handFinder.fingers[0].y);
		}
		else {
			shader.setUniform2f("f1", -10.f, -10.f);
		}

		if (handFinder.fingers.size() > 1) {
			shader.setUniform2f("f2", handFinder.fingers[1].x, handFinder.fingers[1].y);
		}
		else {
			shader.setUniform2f("f2", -10.f, -10.f);
		}

		if (handFinder.fingers.size() > 2) {
			shader.setUniform2f("f3", handFinder.fingers[2].x, handFinder.fingers[2].y);
		}
		else {
			shader.setUniform2f("f3", -10.f, -10.f);
		}

		if (handFinder.fingers.size() > 3) {
			shader.setUniform2f("f4", handFinder.fingers[3].x, handFinder.fingers[3].y);
		}
		else {
			shader.setUniform2f("f4", -10.f, -10.f);
		}

		if (handFinder.fingers.size() > 4) {
			shader.setUniform2f("f5", handFinder.fingers[4].x, handFinder.fingers[4].y);
		}
		else {
			shader.setUniform2f("f5", -10.f, -10.f);
		}
		
		//sending the resolution of the webcam input to normalize finger positions
		shader.setUniform2f("ires", grayImg.width, grayImg.height);

		//drawing the rectangle to draw the shader on
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		shader.end();
	}


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//shift key toggles shader
	if (key == OF_KEY_SHIFT) {
		display = !display;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
