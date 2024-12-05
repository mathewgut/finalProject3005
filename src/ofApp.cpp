#include "ofApp.h"
#include "password-game-method.cpp"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(3840, 2160);

    // Setup Arduino
    m_arduino.connect(config::ARDUINO_DEVICE_NAME, 57600);
    m_bSetup = false;

    // Listen for EInitialized notification, this indicates the Arduino is ready
    ofAddListener(m_arduino.EInitialized, this, &ofApp::setupArduino);

    // Load background images
    background[0].load("intro.png");
    background[1].load("search.png");
    background[2].load("password.png");
    background[3].load("radio.png");
    background[4].load("ending.png");

}

//--------------------------------------------------------------
void ofApp::update() {
    updateArduino();
    if (m_code[0] == 1 && m_code[1] == 5 && m_code[2] == 2 && m_code[3] == 7)
    {
        passwordComplete = 1;
        cout << "good job!" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {

    // USE THE ORDER BELOW //
    /*background[2].draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    displayCode();
    ofSetColor(255);*/

    //displayCode();


    
    if (introComplete == 0 && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ofSetColor(255);
        ////////INTRO SCENE////////
        background[0].draw(0, 0, ofGetWidth(), ofGetHeight());
        displayEntries();
        
    }
    else if (introComplete && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ////////SEARCH GAME////////
        background[1].draw(0, 0, ofGetWidth(), ofGetHeight());
        displayEntries();
    }
    else if (introComplete && searchComplete && passwordComplete == 0 && radioComplete == 0)
    {
        ////////PASSWORD GAME////////
        background[2].draw(0, 0, ofGetWidth(), ofGetHeight());
        displayCode();
        displayEntries();

    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete == 0)
    {
        ////////RADIO GAME////////
        background[3].draw(0, 0, ofGetWidth(), ofGetHeight());
        displayEntries();
    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete)
    {
        ////////ENDING SCENE////////
        background[4].draw(0, 0, ofGetWidth(), ofGetHeight());
        displayEntries();
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
}

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

void ofApp:: displayEntries() {
ofSetColor(0); // Set color to black for other text as well
    if (introComplete == 0 && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ////////INTRO SCENE////////
        ofSetColor(ofColor::red);
        ofDrawBitmapString(m_entries[0], ofGetWidth()/2, ofGetHeight()/2); // Display the entry text
        cout << "this is getting called" << endl;
        ofSetColor(255);
    }
    else if (introComplete && searchComplete == 0 && passwordComplete == 0 && radioComplete == 0)
    {
        ////////SEARCH GAME////////
        ofDrawBitmapString(m_entries[1], 2750, 350);
        ofSetColor(255);
    }
    else if (introComplete && searchComplete && passwordComplete == 0 && radioComplete == 0)
    {
        ////////PASSWORD GAME////////
        ofDrawBitmapString(m_entries[2], 2750, 350);
        ofSetColor(255);

    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete == 0)
    {
        ////////RADIO GAME////////
        ofDrawBitmapString(m_entries[3], 2750, 350);
        ofSetColor(255);
    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete)
    {
        ////////ENDING SCENE////////
        ofDrawBitmapString(m_entries[4], 2750, 350);
        ofSetColor(255);
    }
    
}

void ofApp::mousePressed(int x, int y, int button) {
    // Change the introComplete state to true when mouse is clicked
    introComplete = 1;
}