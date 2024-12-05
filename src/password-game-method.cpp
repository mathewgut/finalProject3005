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
}