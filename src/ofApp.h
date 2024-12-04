#pragma once

#include "ofMain.h"
#include "ofEvents.h"

namespace config {
    static const std::string ARDUINO_DEVICE_NAME = "COM9";  // Update with your device name
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
};