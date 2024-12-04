#pragma once

#include "ofMain.h"
#include "ofEvents.h"

namespace config {
    static const std::string ARDUINO_DEVICE_NAME = "COM3";  // Update with your device name
};

class ofApp : public ofBaseApp{

	public:
		static const int PIN_BUTTON_5 = 6; // Button 5 on digital pin 6
		void setup();
		void update();
		void draw();

		ofSerial			serial;
		string				potValue;
		int				byteData;
		string				stringData;
	
	   	 // Arduino stuff
	   	 ofArduino m_arduino;
	    	int m_code[4] = { 0, 0, 0, 0 };  // The 4-digit code, initialized to 0
	    	bool m_bSetup;                 // Is Arduino initialized?

		void setupArduino(const int& _version);
		void updateArduino();
		void digitalPinChanged(const int& pinNum);

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
		

		void readPotentiometer();
		bool radioFader(float fadeValue, float volume1, float volume2, float volume3, float volume4, int PIN_BUTTON_5, ofArduino m_arduino);
};
