#include "ofApp.h"
#include "password-game-method.cpp"
#include "search-game-methods.cpp"
#include "radio-game-methods.cpp"

//--------------------------------------------------------------
void ofApp::setup() {


    // Setup Arduino
 
    // Listen for EInitialized notification, this indicates the Arduino is ready

    // Load background images
    background[0].load("intro.png");
    background[1].load("search.png");
    background[2].load("password.png");
    background[3].load("radio.png");
    background[4].load("ending.png");

    ofSetWindowShape(1280, 720);
    searchGameSetup();
    passwordGameSetup();
    

    if (introComplete && searchComplete && passwordComplete && radioComplete == 0) {
        radioGameSetup();
    }
    else {
        sound_static.stop();
        sound_brightside.stop();
        sound_station1.stop();
        sound_station2.stop();
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    //passwordGameUpdate();

    
    if (introComplete&& searchComplete == 0 && passwordComplete == 0 && radioComplete == 0) {
        searchGameUpdate();
    }
    if (introComplete && searchComplete && passwordComplete == 0 && radioComplete == 0) {
        passwordGameUpdate();
    }

    if (introComplete && searchComplete && passwordComplete && radioComplete == 0) {
        displayRadio(potValue);
        radioFader(potValue, fadeValue, volume1, volume1, volume1, volume4, PIN_BUTTON_5);
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
        searchGameDraw();
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
    //ofRemoveListener(m_arduino.EInitialized, this, &ofApp::setupArduino);
    
    // Set digital pins as input (buttons) and enable pull-up resistors
    m_arduino.sendDigitalPinMode(PIN_BUTTON_1, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_2, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_3, ARD_INPUT);
    m_arduino.sendDigitalPinMode(PIN_BUTTON_4, ARD_INPUT);

   

    // Listen for changes in digital pins
    ofAddListener(m_arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(m_arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);


    // Set analog pins as input (potentiometer)
    m_arduino.sendAnalogPinReporting(PIN_POT, ARD_ANALOG);

}

//--------------------------------------------------------------
void ofApp::updateArduino() {
    // Update the Arduino, which checks for incoming data
    cout << "hi i'm working arduino! \n";
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
    if (pinNum == PIN_BUTTON_5 && m_arduino.getDigital(pinNum) == 0) {
        button5Pressed = true;
    }
}

//--------------------------------------------------------------
void ofApp::analogPinChanged(const int& pinNum) {
    //get and output the the potentiometer value
    potValue = m_arduino.getAnalog(pinNum);
    cout << "potentiometer: " << potValue << '\n';
}

// stores which object color is being applied to
vector<int> currColorObject;



//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    introComplete = 1;
    searchGameMousePress(x, y, button, currColorObject);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    searchGameKeyPress(key);
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}



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
        ofSetColor(ofColor::red);
        ofDrawBitmapString(m_entries[1], 700, 350);
        ofSetColor(255);
    }
    else if (introComplete && searchComplete && passwordComplete == 0 && radioComplete == 0)
    {
        ////////PASSWORD GAME////////
        ofSetColor(ofColor::red);
        ofDrawBitmapString(m_entries[2], 775, 75);
        ofSetColor(255);

    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete == 0)
    {
        ////////RADIO GAME////////
        ofSetColor(ofColor::red);
        ofDrawBitmapString(m_entries[3], 700, 350);
        ofSetColor(255);
    }
    else if (introComplete && searchComplete && passwordComplete && radioComplete)
    {
        ////////ENDING SCENE////////
        ofSetColor(ofColor::red);
        ofDrawBitmapString(m_entries[4], 700, 350);
        ofSetColor(255);
    }
    
}
