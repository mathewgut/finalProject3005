#include "ofApp.h"

void ofApp::displayRadio(int potValue) {
	float radioFreq = ofMap(potValue, 0, 1024, 1750.0f, 1850.0f, true);
	std::round(
		std::round(radioFreq * 1000.0f) / 10.0f) / 100.0f;	// rounds radioFreq to 2 decimals using 2 stages of rounding
	ofDrawBitmapString(ofToString(radioFreq) + "%.2f" + " MHz", 100, 100);
}

void ofApp::radioGameSetup() {
	sound_static.load("static.mp3");
	sound_brightside.load("brightside.mp3");
	sound_station1.load("echoLima.mp3");
	sound_station2.load("conet333.mp3");

	sound_static.setLoop(true);
	sound_brightside.setLoop(true);
	sound_station1.setLoop(true);
	sound_station2.setLoop(true);

	sound_static.play();
	sound_brightside.play();
	sound_station1.play();
	sound_station2.play();
}

void ofApp::radioFader(int potValue, float fadeValue, float volume1, float volume2, float volume3, float volume4, bool button5Pressed) {
	//increases and lowers volume levels as potentiometer's range is swept through

	fadeValue = ofMap(potValue, 0, 1024, 0, 1, true);	  // for potentiometer
	//fadeValue = ofMap(ofGetMouseX(), 0, 1280, 0, 1, false);	// DEBUG: using mouse

	//order:		static -> station1 -> static -> station2 -> static -> station3 -> static
	//breakpoints:		  0.2f ->     3.5f ->   0.4f ->     0.6f ->   0.8f
	cout << "window width:" << ofGetWindowWidth() << "\n";


	if (fadeValue <= 0.2f) {
		volume1 = ofClamp(1.0f - fadeValue * 5, 0, 1);
		volume2 = ofClamp(fadeValue * 5, 0, 1);
		if (button5Pressed == true) {
			radioComplete = 1;
		}
	}
	else if (fadeValue <= 0.4f) {
		volume2 = ofClamp(1.0f - (fadeValue - 0.2f) * 5, 0, 1);
		volume3 = ofClamp((fadeValue - 0.2f) * 5, 0, 1);
	}
	else if (fadeValue <= 0.6f) {
		volume3 = ofClamp(1.0f - (fadeValue - 0.4f) * 5, 0, 1);
		volume4 = ofClamp((fadeValue - 0.4f) * 5, 0, 1);
	}
	else {
		volume4 = ofClamp(1.0f - (fadeValue - 0.6f) * 5, 0, 1);
		volume1 = ofClamp((fadeValue - 0.6f) * 5, 0, 1);
	}

	sound_static.setVolume(volume1);
	sound_brightside.setVolume(volume2);
	sound_station1.setVolume(volume3);
	sound_station2.setVolume(volume4);

	cout << fadeValue << "\n";
	cout << "vol1: " << volume1 << "\n";
	cout << "vol2: " << volume2 << "\n";
	cout << "vol3: " << volume3 << "\n";

	radioComplete = 0;
}