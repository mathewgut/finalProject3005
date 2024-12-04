#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    ofSerial serial; // Serial object for communication
    std::string buttonState; // Stores the state of the button
};
