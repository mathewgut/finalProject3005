#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::displayCode() {
    // Display the current 4-digit code
    string codeStr = "Code: ";
    for (int i = 0; i < 4; i++) {
        codeStr += ofToString(m_code[i]);
    }
    ofDrawBitmapString(codeStr, 530, 105);  // Display the code on the screen
}