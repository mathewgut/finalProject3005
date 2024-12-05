#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "ImHelpers.h"		//this gives us some extra ofxImGui features (needed for drop-down menus)
#include "CVC.h"


namespace config {
    static const std::string ARDUINO_DEVICE_NAME = "COM9";  // Update with your device name
};

class ofApp : public ofBaseApp {
public:
    static const int PIN_BUTTON_1 = 2;  // Button 1 on digital pin 2
    static const int PIN_BUTTON_2 = 3;  // Button 2 on digital pin 3
    static const int PIN_BUTTON_3 = 4;  // Button 3 on digital pin 4
    static const int PIN_BUTTON_4 = 5;  // Button 4 on digital pin 5
	static const int PIN_BUTTON_5 = 6;	// Button 5 on digital pin 6
	static const int PIN_POT = 3;		// Potentiometer on analog pin 3

    void setup();
    void update();
    void draw();
    void mousePressed(int x, int y, int button);

    // Arduino stuff
    ofArduino m_arduino;
    int m_code[4] = { 0, 0, 0, 0 };  // The 4-digit code, initialized to 0
    bool m_bSetup;                 // Is Arduino initialized?

    void setupArduino(const int& _version);
    void updateArduino();
    void digitalPinChanged(const int& pinNum);
	void analogPinChanged(const int& pinNum);

    // Helper function to display the code
    void displayCode();

    // Game state
    bool introComplete = 0;
    bool searchComplete = 0;
    bool passwordComplete = 0;
    bool radioComplete = 0;


    //Journal
    string m_entries[5] =
    {
        "Your ship, Project Willow, jolts violently as it�s struck by an asteroid. \nSystems fail one by one, leaving you drifting in the cold void of space. The cabin plunges into darkness, and siren is going off. ",
        "...",
        "08 / 13 / 2034 - My precious backup battery \nThe code to the Backup Battery trigger isn�t written, but it�s hidden in plain sight.\nThink of the constellation I cherished most : Cassiopeia. \nFive stars, bright and bold, forming a �W� � it always reminded me of my late wife, Willow.\nThey say one of the few recorded supernovas in the Milky Way occurred in this constellation long ago.\nA brilliant burst of light, fleeting but unforgettable.\nIt reminded me of her�how cancer took her too soon, in a way both devastating and strangely beautiful.\nCassiopeia was our anchor, our reminder of home.Remember : 4 numbers.\nIt�s all about that year.",
        "10 / 04 / 2043 - **** THAT **** SONG \nThe comms are temperamental�like an old antenna rattling in a storm. \nAdjust the gain carefully; too much, and you�ll fry the system. \nStart at the knob on the control panel�it�s labeled COMMS-1. \nEarth�s frequency? Somewhere between 115.5 and 119.2 MHz. \nTune slowly, and listen closely. \nYou�ll know you�ve connected when you hear the intro to that infuriating song� \nWhat was it again? Something about a Mr.? \nIt was the most overplayed hit of the 2000s, \nand if I hear it one more time, I might rip out the comms myself.\nKeep listening to that for at least 10 seconds, then you should have all the information you need.Good luck cutting through the static.",
        "GAME COMPLETE\nThank you for playing! (:\nHairuo, Mat and Peter."
    };

    void displayEntries();

    //Background
    ofImage background[5];

    // password game methods

    void passwordGameUpdate();
    void passwordGameSetup();

	ofVideoPlayer m_videoPlayer;
	ofVideoGrabber m_videoGrabber;
	bool m_videoResolutionConflict;		//do we need to re-size our video before we process?
	bool m_camResolutionConflict;		//do we need to re-size our video before we process?
	bool m_camPaused;					//pausing our camera feed

	bool debug = true;

	// radio game methods
	void displayRadio(int potValue); // helper function to display radio frequency as string

	void radioGameSetup();
	bool radioFader(int potValue,float fadeValue, float volume1, float volume2, float volume3, float volume4,
					bool button5Pressed);

	int potValue = 0;
	bool button5Pressed = false;

	ofSoundPlayer		sound_static;
	ofSoundPlayer		sound_brightside;
	ofSoundPlayer		sound_station1;
	ofSoundPlayer		sound_station2;
	const int			numSounds = 4;

	float				fadeValue = 0.0f;
	float				volume1 = 0.0f;
	float				volume2 = 0.0f;
	float				volume3 = 0.0f;
	float				volume4 = 0.0f;


	/* BOUNDRY PADDING */
	float boundryPadding = 50.0f;
	float spawnedObjBoundryPadding = 10.0f;
	float buttonBoundryPadding = 20.0f;

	/* HANDS */
	float handSize = 100.0f;
	float handBoundaryPadding = 15;

	ofxCvContourFinder m_hand1;

	ofVec2f hand1Center;
	ofVec2f hand1TL;
	ofVec2f hand1BR;

	ofImage handImage;

	/* dark area */
	ofImage darkAreaImage;

	/* background */
	ofImage backgroundImage;

	/* no controller black image */
	ofImage noControllerImage;

	/* to be found object */
	ofImage journalImage;
	float journalSize = 100;
	float journalBoundaryPadding = 15;

	ofVec2f journalTL;
	ofVec2f journalBR;

	ofVec2f journalSpawnPos;

	/* win condition */
	bool journalFound = false;


	//CV images
	ofxCvColorImage		m_colorImage;
	ofxCvGrayscaleImage	m_grayscaleDiffImage;
	ofxCvGrayscaleImage	m_grayscaleDiffImage2;

	//GUI
	ofxImGui::Gui		m_gui;
	int					m_threshold;	//how closely does the colour have to match to track?
	int					m_numContoursConsidered;
	float				m_minArea;
	float				m_maxArea;

	//we want to select which color we are tracking within the video
	float m_trackedColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//ofxImgui requires color in this form
	float m_trackedColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//ofxImgui requires color in this form

	//app modes
	CVC::APP_MODE m_appMode;				//current app state
	std::vector<std::string> m_appModes;	//"list"/dynamic array of app states


	//we will do all color CV processing here
	//we are passing a reference here ...
	void processColor(ofxCvColorImage& image);

	

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);

	void mousePressed(int x, int y, int button);

	bool collisionCheck(ofVec2f obj1TL, ofVec2f obj1BR, ofVec2f obj2TL, ofVec2f obj2BR);
	void setHandBoundries();
	void setSpawnedObjBoundries();
	void searchGameSetup();
	void searchGameUpdate();
	void searchGameDraw();
	void searchGameMousePress(int x, int y, int button, vector<int> currColorObject);
	void searchGameKeyPress(int key);



};