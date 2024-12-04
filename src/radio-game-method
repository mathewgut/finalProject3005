void ofApp::readPotentiometer() {
	//outputs potentiometer value to console

	//check for serial error
	if (serial.available() == 0) {
		potValue = "Arduino Error";
	}
	else {
		while (serial.available() > 0) {
			byteData = serial.readByte();
			cout << "received: " << byteData << endl;
			if (byteData == '\n') {
				potValue = "value: " + ofToString(ofToInt(stringData));
				stringData = "";
			}
			else {
				stringData += byteData;
			}
		}
	}
	cout << potValue << endl;
}

void ofApp::radioFader(float fadeValue, float volume1, float volume2, float volume3, float volume4) {
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
	else if {
		volume4 = 1.0f - (fadeValue - 0.8333f) * 5;
		volume1 = (fadeValue - 0.8333f) * 5;
	}

	sound_static.setVolume(volume1);
	sound_brightside.setVolume(volume2);
	sound_station1.setVolume(volume3);
	sound_station2.setVolume(volume4);
}
