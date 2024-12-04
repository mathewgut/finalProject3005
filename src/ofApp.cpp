#include "ofApp.h"

void ofApp::setup() {
    // List all serial devices
    auto devices = serial.getDeviceList();
    if (devices.empty()) {
        ofLogError() << "No serial devices found!";
        return;
    }

    // Print available devices
    for (auto& device : devices) {
        ofLog() << "Available device: " << device.getDevicePath();
    }

    // Try connecting to the first device in the list (or replace with your port)
    if (serial.setup(devices[0].getDevicePath(), 9600)) {
        ofLog() << "Successfully connected to " << devices[0].getDevicePath();
    }
    else {
        ofLogError() << "Failed to connect to " << devices[0].getDevicePath();
    }
}

void ofApp::update() {
    // Check if there is data available to read
    while (serial.available() > 0) {
        char byteData = serial.readByte(); // Read a single byte

        // Build the string (look for newline character as the end of data)
        if (byteData == '\n') {
            ofLog() << "Button State: " << buttonState; // Log the received state
            buttonState.clear(); // Clear for the next message
        }
        else {
            buttonState += byteData; // Append data to the string
        }
    }
}

void ofApp::draw() {
    // Display the button state on screen
    ofDrawBitmapString("Button State: " + buttonState, 20, 20);
}
