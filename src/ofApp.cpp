#include "ofApp.h"
#include "password-game-method.cpp"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(1280, 720);
    searchGameSetup();
    passwordGameSetup();
}

//--------------------------------------------------------------
void ofApp::update() {
    passwordGameUpdate();
    searchGameUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {

    
    if (introComplete = 0 && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ////////INTRO SCENE////////
    }
    else if (introComplete && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ////////SEARCH GAME////////
        searchGameDraw();
    }
    else if (introComplete && searchComplete && passwordComplete == 0 && radioComplete == 0)
    {
        ////////PASSWORD GAME////////
        displayCode(); // Display the 4-digit code on the screen
    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete == 0)
    {
        ////////RADIO GAME////////
    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete)
    {
        ////////ENDING SCENE////////
    }

}




//--------------------------------------------------------------
void ofApp::setupArduino(const int& _version) {
    m_bSetup = true;
    
    // Remove the listener as we don't need it anymore
    ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
    
    // Set digital pins as input (buttons) and enable pull-up resistors
    m_arduino.sendDigitalPinMode(PIN_BUTTON_1, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_2, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_3, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_4, ARD_INPUT);

    // Listen for changes in digital pins
    ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);


//--------------------------------------------------------------
void ofApp::updateArduino() {
    // Update the Arduino, which checks for incoming data
    m_arduino.update();
}

//--------------------------------------------------------------
void ofApp::digitalPinChanged(const int& pinNum) {
    // Handle digital pin changes (button presses)
    /*
    if (pinNum == PIN_BUTTON_1 && m_arduino.getDigital(pinNum) == 0) {
        m_code[0] = (m_code[0] + 1) % 10;  // Increment the first code digit (0-9)
    } else if (pinNum == PIN_BUTTON_2 && m_arduino.getDigital(pinNum) == 0) {
        m_code[1] = (m_code[1] + 1) % 10;  // Increment the second code digit (0-9)
    } else if (pinNum == PIN_BUTTON_3 && m_arduino.getDigital(pinNum) == 0) {
        m_code[2] = (m_code[2] + 1) % 10;  // Increment the third code digit (0-9)
    } else if (pinNum == PIN_BUTTON_4 && m_arduino.getDigital(pinNum) == 0) {
        m_code[3] = (m_code[3] + 1) % 10;  // Increment the fourth code digit (0-9)
    }
    */

    cout << m_arduino.getDigital(pinNum) << endl;

    if (pinNum == PIN_BUTTON_1 && m_arduino.getDigital(pinNum) == 0) {
        ofDrawRectangle({ 0,0 }, 100, 100);
        m_code[0] = (m_code[0] + 1)%10;  // Increment the first code digit (0-9)
    }
    else if (pinNum == PIN_BUTTON_2 && m_arduino.getDigital(pinNum) == 0) {
        m_code[1] = (m_code[1] + 1)%10;  // Increment the second code digit (0-9)
    }
    else if (pinNum == PIN_BUTTON_3 && m_arduino.getDigital(pinNum) == 0) {
        m_code[2] = (m_code[2] + 1)%10;  // Increment the third code digit (0-9)
    }
    else if (pinNum == PIN_BUTTON_4 && m_arduino.getDigital(pinNum) == 0) {
        m_code[3] = (m_code[3] + 1)%10;  // Increment the fourth code digit (0-9)
    }
}

// stores which object color is being applied to
vector<int> currColorObject;
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}
  
  //--------------------------------------------------------------
void ofApp::keyPressed(int key){
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
/*
void ofApp::displayCode() {
    // Display the current 4-digit code
    string codeStr = "Code: ";
    for (int i = 0; i < 4; i++) {
        codeStr += ofToString(m_code[i]);
    }
    ofDrawBitmapString(codeStr, 530, 105);  // Display the code on the screen
}*/