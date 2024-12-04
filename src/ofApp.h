#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		ofSerial			serial;
		string				potValue;
		int				byteData;
		string				stringData;

		ofSoundPlayer			sound_static;
		ofSoundPlayer			sound_brightside;
		ofSoundPlayer			sound_station1;
		ofSoundPlayer			sound_station2;
		const int			numSounds = 4;

		float				fadeValue;
		float				volume1;
		float				volume2;
		float				volume3;
		float				volume4;
		
};
