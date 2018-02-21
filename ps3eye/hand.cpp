#include "hand.h"

hand::hand() {
	cent.set(0, 0);
	pCent.set(0, 0);
}

void hand::findFingers(ofxCvGrayscaleImage &img) {

	fingers.clear();

	ofxCvContourFinder contours;

	contours.findContours(img, 1, 1000, 5, true);

	for (int i = 0; i < contours.nBlobs;i++) {

		fingers.push_back(contours.blobs[i].centroid);

	}

	cout << fingers.size();
}

ofVec3f hand::getVel() {

	ofVec3f t = pCent;

	pCent = getCentroid();

	return(getCentroid() - t);

}

ofVec3f hand::getCentroid() {
	if (fingers.size() > 0) {
		ofVec3f avg;

		for (int i = 0; i < fingers.size(); i++) {
			avg += fingers[i];
		}

		avg /= fingers.size();

		return avg;
	}
	else {
		return ofVec3f(0, 0);
	}

}

float hand::getAvgDistance() {
	
	ofVec3f cent = getCentroid();

	float d = 0;

	

	for (auto iter = fingers.begin(); iter != fingers.end(); ++iter) {

		d += cent.distance((*iter));

	}

	d /= fingers.size();

	return d;

}