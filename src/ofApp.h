#pragma once

#include "ofMain.h"
#include "ofEvents.h"

namespace config {
    static const std::string ARDUINO_DEVICE_NAME = "COM3";  // Update with your device name
};

class ofApp : public ofBaseApp {
public:
    static const int PIN_BUTTON_1 = 2;  // Button 1 on digital pin 2
    static const int PIN_BUTTON_2 = 3;  // Button 2 on digital pin 3
    static const int PIN_BUTTON_3 = 4;  // Button 3 on digital pin 4
    static const int PIN_BUTTON_4 = 5;  // Button 4 on digital pin 5

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
        "Your ship, Project Willow, jolts violently as it’s struck by an asteroid. \nSystems fail one by one, leaving you drifting in the cold void of space. The cabin plunges into darkness, and siren is going off. ",
        "...",
        "08 / 13 / 2034 - My precious backup battery \nThe code to the Backup Battery trigger isn’t written, but it’s hidden in plain sight.\nThink of the constellation I cherished most : Cassiopeia. \nFive stars, bright and bold, forming a ‘W’ — it always reminded me of my late wife, Willow.\nThey say one of the few recorded supernovas in the Milky Way occurred in this constellation long ago.\nA brilliant burst of light, fleeting but unforgettable.\nIt reminded me of her—how cancer took her too soon, in a way both devastating and strangely beautiful.\nCassiopeia was our anchor, our reminder of home.Remember : 4 numbers.\nIt’s all about that year.",
        "10 / 04 / 2043 - **** THAT **** SONG \nThe comms are temperamental—like an old antenna rattling in a storm. \nAdjust the gain carefully; too much, and you’ll fry the system. \nStart at the knob on the control panel—it’s labeled COMMS-1. \nEarth’s frequency? Somewhere between 115.5 and 119.2 MHz. \nTune slowly, and listen closely. \nYou’ll know you’ve connected when you hear the intro to that infuriating song… \nWhat was it again? Something about a Mr.? \nIt was the most overplayed hit of the 2000s, \nand if I hear it one more time, I might rip out the comms myself.\nKeep listening to that for at least 10 seconds, then you should have all the information you need.Good luck cutting through the static.",
        "GAME COMPLETE\nThank you for playing! (:\nHairuo, Mat and Peter."
    };

    void displayEntries();

    //Background
    ofImage background[5];
};