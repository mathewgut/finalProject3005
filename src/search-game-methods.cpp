#include "ofMain.h"
#include "ofApp.h"


// basic collision check between two object's lowest and highest points. if another object's point is within that range, return true
bool ofApp::collisionCheck(ofVec2f obj1TL, ofVec2f obj1BR, ofVec2f obj2TL, ofVec2f obj2BR) {
    //std::cout << "obj1TL: (" << obj1TL.x << ", " << obj1TL.y << "), obj1BR: (" << obj1BR.x << ", " << obj1BR.y << ")\n";
   // std::cout << "obj2TL: (" << obj2TL.x << ", " << obj2TL.y << "), obj2BR: (" << obj2BR.x << ", " << obj2BR.y << ")\n";

    if (obj1TL.x < obj2BR.x && obj1BR.x > obj2TL.x &&
        obj1TL.y < obj2BR.y && obj1BR.y > obj2TL.y) {
        //std::cout << "YES.\n";
        return true;
    }

    else {
        //std::cout << "NO.\n";
        return false;
    }
}

// sets boundries for spawnedObj based on position
void ofApp::setSpawnedObjBoundries() {
    //spawnedObjTL.x = spawnedObjPos.x - spawnedObjBoundryPadding;
    //spawnedObjTL.y = spawnedObjPos.y - spawnedObjBoundryPadding + 10;

    // set bottom right points x and y based off center point and size of image
    //spawnedObjBR.x = spawnedObjPos.x + spawnedObjBoundryPadding;
    //spawnedObjBR.y = spawnedObjPos.y + spawnedObjBoundryPadding - 10;

}


// sets boundries for hand1 and hand2 based on position
void ofApp::setHandBoundries() {
    // set top left points x and y based off center point and size of image
    //hand1TL.x = hand1Center.x - boundryPadding;

    //hand1TL.y = hand1Center.y - boundryPadding + 15; // altering vertical y so collision works better

    // set bottom right points x and y based off center point and size of image
    //hand1BR.x = hand1Center.x + boundryPadding;

    //hand1BR.y = hand1Center.y + boundryPadding - 15;


}

void ofApp::processColor(ofxCvColorImage& image) {
    const int numChannelsPerPixel = image.getPixels().getNumChannels(); //should be 3 for red, green, blue channels
    const int numChannels = CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT * numChannelsPerPixel;
    float pixel[3] = { 0.0f, 0.0f, 0.0f };

    //loop through color image pixels
    //check if pixel color matches our tracked color
    //if true, set our diff image pixel color to white
    //if false, set our diff image pixel color to black
    for (int i = 0; i < numChannels; i += numChannelsPerPixel) {
        pixel[0] = image.getPixels()[i + 0];        //red
        pixel[1] = image.getPixels()[i + 1];        //green
        pixel[2] = image.getPixels()[i + 2];        //blue

        //check if tracked color matches pixel color
        //m_trackedColor is a float color [0.0, 1.0] so we need to convert back to a byte color [0,255]
        if ((abs(pixel[0] - m_trackedColor[0] * 255.0f) < m_threshold) &&
            (abs(pixel[1] - m_trackedColor[1] * 255.0f) < m_threshold) &&
            (abs(pixel[2] - m_trackedColor[2] * 255.0f) < m_threshold)) {

            //COLOR THE SAME - YAAYYYYYYY!!
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 255;
        }
        else {
            //NOT THE SAME - BOOO
            m_grayscaleDiffImage.getPixels()[i / numChannelsPerPixel] = 0;
        }


    }

    //update cv image
    m_grayscaleDiffImage.flagImageChanged();        //tells the compy that the image has changed on the CPU RAM and should be moved to GPU RAM

    //find contours/blobs
    m_hand1.findContours(m_grayscaleDiffImage, m_minArea, m_maxArea, m_numContoursConsidered, false, true);

}
