#include "ofApp.h"
#include "radio-game-method.cpp"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);

	//connect to arduino
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

	int baud = 9600;
	serial.setup("COM3", baud);

	//initialize values
	stringData = "";
	potValue = "";

	fadeValue = 0.0f;
	volume1 = 0.0f;
	volume2 = 0.0f;
	volume3 = 0.0f;
	volume4 = 0.0f;

	//load and play sound sources
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

//--------------------------------------------------------------
void ofApp::update(){
	readPotentiometer();
	radioFader(fadeValue,  volume1, volume2, volume3, volume4);
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(255);
	ofDrawBitmapString("potentiometer value: " + ofToString(potValue), 20, 20);

	//DEBUG: display mouse input, volume vars, and fadeValue
	int mouseX = ofGetMouseX();
	ofDrawBitmapString("mouse pos: " + ofToString(mouseX), 20, 40);

	ofDrawBitmapString("volume (static): " + ofToString(volume1), 20, 60);
	ofDrawBitmapString("volume (brightside): " + ofToString(volume2), 20, 80);
	ofDrawBitmapString("volume (station1): " + ofToString(volume3), 20, 100);
	ofDrawBitmapString("volume (station2): " + ofToString(volume4), 20, 120);

	ofDrawBitmapString("fadeValue: " + ofToString(fadeValue), 20, 140);
}
