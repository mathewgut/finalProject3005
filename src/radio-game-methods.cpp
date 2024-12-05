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

bool ofApp::radioFader(float fadeValue, float volume1, float volume2, float volume3, float volume4, bool button5Pressed, ofArduino m_arduino) {
	//increases and lowers volume levels as potentiometer's range is swept through

	//fadeValue = ofMap(ofToInt(potValue), 0, 1024, 0, 1, true);	  // for potentiometer
	fadeValue = ofMap(ofGetMouseX(), 9, ofGetWidth(), 0, 1, true);	// DEBUG: using mouse

	//order:		static -> station1 -> static -> station2 -> static -> station3 -> static
	//breakpoints:		  0.2f ->     3.5f ->   0.4f ->     0.6f ->   0.8f

	if (fadeValue <= 0.2f) {
		volume1 = 1.0f - fadeValue * 5;
		volume2 = fadeValue * 5;
	}
	else if (0.2f < fadeValue <= 3.5f) {
		volume2 = 1.0f - (fadeValue - 0.1667f) * 5;
		volume1 = (fadeValue - 0.1667f) * 5;
		if (button5Pressed == true) {
			return 1;
		}
	}
	else if (3.5f < fadeValue <= 0.4f) {
		volume1 = 1.0f - (fadeValue - 0.3333f) * 5;
		volume3 = (fadeValue - 0.3333f) * 5;
	}
	else if (0.4f < fadeValue <= 0.6f) {
		volume3 = 1.0f - (fadeValue - 0.5f) * 5;
		volume1 = (fadeValue - 0.5f) * 5;
	}
	else if (0.6f < fadeValue <= 0.8f) {
		volume1 = 1.0f - (fadeValue - 0.6666f) * 5;
		volume4 = (fadeValue - 0.6666f) * 5;
	}
	else {
		volume4 = 1.0f - (fadeValue - 0.8333f) * 5;
		volume1 = (fadeValue - 0.8333f) * 5;
	}

	sound_static.setVolume(volume1);
	sound_brightside.setVolume(volume2);
	sound_station1.setVolume(volume3);
	sound_station2.setVolume(volume4);

	return 0;
}