#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::displayCode() {
    // Display the current 4-digit code
    string codeStr = "Code: ";
    for (int i = 0; i < 4; i++) {
        codeStr += ofToString(m_code[i]);
    }

    ofSetColor(ofColor::red);  // Set the text color to black
    ofDrawBitmapString(codeStr, 1250, 1800);  // Display the code on the screen

    ofDrawBitmapString(codeStr, 530, 105);  // Display the code on the screen
}

void ofApp::passwordGameUpdate() {
    updateArduino();
    if (m_code[0] == 1 && m_code[1] == 5 && m_code[2] == 2 && m_code[3] == 7)
    {
        passwordComplete == 1;
        cout << "good job!" << endl;
    }
}

void ofApp::passwordGameSetup() {
    // Setup Arduino
    m_arduino.connect(config::ARDUINO_DEVICE_NAME, 57600);
    m_bSetup = false;

    // Listen for EInitialized notification, this indicates the Arduino is ready
    ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

}