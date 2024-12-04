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
   
    // creating a boundary area around the object from its center point for collision
    // boundary areas allow for a more natural feeling collision

    journalTL.x = journalSpawnPos.x - journalBoundaryPadding;
    journalTL.y = journalSpawnPos.y - journalBoundaryPadding;

    journalBR.x = journalSpawnPos.x + journalBoundaryPadding;
    journalBR.y = journalSpawnPos.y + journalBoundaryPadding;

}


// sets boundries for hand1 and hand2 based on position
void ofApp::setHandBoundries() {
    // set top left points x and y based off center point and size of image
    hand1TL.x = hand1Center.x - boundryPadding;

    hand1TL.y = hand1Center.y - boundryPadding; // altering vertical y so collision works better

    // set bottom right points x and y based off center point and size of image
    hand1BR.x = hand1Center.x + boundryPadding;

    hand1BR.y = hand1Center.y + boundryPadding;


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

void ofApp::searchGameSetup() {
    /* CONTROLLER IMAGES */
    handImage.load("hand-left.png");
    handImage.resize(handSize, handSize);

    handImage.setAnchorPoint(handImage.getWidth() / 2, handImage.getHeight() / 2);

    /* DARK AREA IMAGE OVERLAY */
    darkAreaImage.load("dark-area.png");
    darkAreaImage.setAnchorPoint(darkAreaImage.getWidth() / 2, darkAreaImage.getHeight() / 2);

    /* BACKGROUND IMAGE */
    backgroundImage.load("test-background.jpg");
    backgroundImage.setAnchorPoint(backgroundImage.getWidth() / 2, backgroundImage.getHeight() / 2);

    /* NO CONTROLLER IMAGE */
    noControllerImage.load("no-controller-black.jpg");
    noControllerImage.setAnchorPoint(noControllerImage.getWidth() / 2, noControllerImage.getHeight() / 2);

    /* JOURNAL IMAGE */
    journalImage.load("box.png");
    journalImage.resize(journalSize, journalSize);
    journalImage.setAnchorPoint(journalImage.getWidth() / 2, journalImage.getHeight() / 2);

    journalSpawnPos.x = 700;
    journalSpawnPos.y = 400;

    //set up app
    ofSetWindowShape(CVC::APP_WINDOW_WIDTH, CVC::APP_WINDOW_HEIGHT);
    ofSetFrameRate(CVC::APP_DESIRED_FRAMERATE);

    //set up cam
    //m_videoGrabber.listDevices();     //to find other cameras attached
    m_videoGrabber.setDeviceID(0);      //assuming first and only camera attached
    m_videoGrabber.setDesiredFrameRate(CVC::CAM_DESIRED_FRAMERATE);
    m_videoGrabber.initGrabber(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_camResolutionConflict = (((int)m_videoGrabber.getWidth() != CVC::VIDEO_WIDTH) ||
        ((int)m_videoGrabber.getHeight() != CVC::VIDEO_HEIGHT));

    //set up video
    m_videoPlayer.load(CVC::VIDEO_PATH_COLOR);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    m_videoPlayer.play();

    //want to check to see if video being loaded in matches the video size consts we wish to process at
    m_videoResolutionConflict = (((int)m_videoPlayer.getWidth() != CVC::VIDEO_WIDTH) ||
        ((int)m_videoPlayer.getHeight() != CVC::VIDEO_HEIGHT));

    //allocate CV images
    m_colorImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);

    m_grayscaleDiffImage.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    m_grayscaleDiffImage2.allocate(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);

    //init vars
    m_threshold = 128;
    m_numContoursConsidered = 5;
    m_minArea = 10.0f;
    m_maxArea = (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT);

    //gui
    m_gui.setup();

    //set up app states
    m_appMode = CVC::APP_MODE::APP_VIDEO;
    m_appModes.push_back("APP_VIDEO");
    m_appModes.push_back("APP_CAM");
    setSpawnedObjBoundries();
}

void ofApp::searchGameUpdate() {
    switch (m_appMode) {
    case CVC::APP_MODE::APP_VIDEO: {
        m_videoPlayer.update();

        //only process if new frame
        if (m_videoPlayer.isFrameNew() == true) {
            m_colorImage.setFromPixels(m_videoPlayer.getPixels());

            //resize, if res conflict
            if (m_videoResolutionConflict == true) {
                m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
            }

            processColor(m_colorImage);
        }
    }
                                 break;
    case CVC::APP_MODE::APP_CAM: {
        if (m_camPaused == false) {
            m_videoGrabber.update();

            if (m_videoGrabber.isFrameNew() == true) {
                m_colorImage.setFromPixels(m_videoGrabber.getPixels());
            }

            //resize, if res conflict
            if (m_camResolutionConflict == true) {
                m_colorImage.resize(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
            }

            processColor(m_colorImage);
        }

    }
                               break;
    }
    // update the hand collision boundries every frame
    setHandBoundries();
    if (collisionCheck(hand1TL, hand1BR, journalTL, journalBR)) {
        cout << " COLLIDING \n";
    }
    else {
        cout << "NOT COLLIDING \n";
    }
}

void ofApp::searchGameDraw(){
    //draw videos
    ofSetColor(255, 255, 255);

    // draw grayscale and colour on debug
    if (debug == true) {
        m_grayscaleDiffImage.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);
        m_grayscaleDiffImage2.draw(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE * 2 + CVC::VIDEO_HEIGHT);
        m_colorImage.draw(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2, CVC::VIDEO_BORDER_SIZE);
    }
    // draw just colour
    else {
        // m_colorImage.draw(CVC::VIDEO_WIDTH, CVC::VIDEO_HEIGHT);
    }

    // contour info
    static ofVec2f contourCenter;
    static float contourArea;

    // when debug is true (when setting up controls), show imgui and greyscale
    if (debug == true) {
        ofPushMatrix();
        {
            ofTranslate(CVC::VIDEO_BORDER_SIZE, CVC::VIDEO_BORDER_SIZE);

            //loop through blobs and draw them with some debug info.
            for (int i = 0; i < m_hand1.nBlobs; i++) {
                contourCenter.set(m_hand1.blobs[i].boundingRect.getCenter().x,
                    m_hand1.blobs[i].boundingRect.getCenter().y);
                contourArea = m_hand1.blobs[i].area;

                //draw contour
                m_hand1.blobs[i].draw();

                //draw center
                ofSetColor(ofColor::coral);
                ofDrawCircle(contourCenter.x, contourCenter.y, 5.0f);

                //draw text info.
                ofDrawBitmapString("HAND1 \n Centre: " + ofToString(contourCenter.x) + "," + ofToString(contourCenter.y),
                    m_hand1.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                    contourCenter.y);
                ofDrawBitmapString("Area: " + ofToString(contourArea),
                    m_hand1.blobs[i].boundingRect.getMaxX() + CVC::VIDEO_BORDER_SIZE,
                    contourCenter.y + 20.0f);
            }
        }
    }
    if (debug == true) {
        //draw gui
        m_gui.begin();
        {
            ImGui::Text("Set up your controllers!");
            ImGui::SliderInt("Threshold", &m_threshold, 0, 255);
            ImGui::SliderInt("Num of Contours", &m_numContoursConsidered, 0, 30);
            ImGui::SliderFloat("Min. Area", &m_minArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));
            ImGui::SliderFloat("Max. Area", &m_maxArea, 0.0f, (float)(CVC::VIDEO_WIDTH * CVC::VIDEO_HEIGHT));

            ImGui::Separator(); //you've got to keep them separated

            ImGui::Text("\n App state? o_0");
            static int currentListBoxIndex = 0;
            if (ofxImGui::VectorCombo("App Mode", &currentListBoxIndex, m_appModes)) {
                m_appMode = (CVC::APP_MODE)currentListBoxIndex;
            }
            ImGui::ColorEdit3("Color - Left Hand", (float*)m_trackedColor);
            ImGui::ColorEdit3("Color - Right Hand", (float*)m_trackedColor2);
            ImGui::Text("\n Instructions: \npress spacebar to toggle pause \nright-click on video to select colour \npress shift for on/off debug view");
        }
        m_gui.end();
    }
    else {
        backgroundImage.draw(1280 / 2, 720 / 2);
        // loop for drawing hand 1
        for (int i = 0; i < m_hand1.nBlobs; i++) {
            contourCenter.set(m_hand1.blobs[i].boundingRect.getCenter().x,
                m_hand1.blobs[i].boundingRect.getCenter().y);
            contourArea = m_hand1.blobs[i].area;

            // remap to convert the lower resolution 640x360 to 1280x720 for drawing hand
            hand1Center.x = ofMap(contourCenter.x, 0, 640, 0, 1280);
            hand1Center.y = ofMap(contourCenter.y, 0, 360, 0, 720);

            journalImage.draw(journalSpawnPos.x, journalSpawnPos.y);

            handImage.draw(hand1Center.x, hand1Center.y);
            darkAreaImage.draw(hand1Center.x, hand1Center.y);

        }
        if (m_hand1.blobs.size() == 0) {
            noControllerImage.draw(640, 360);
        }

    }

}

void ofApp::searchGameMousePress(int x, int y, int button, vector<int> currColorObject) {
    if (button == OF_MOUSE_BUTTON_RIGHT) {


        //select the colour from the image
        //we CAN't SELECT color from outside the image (may get an indes out of array error)
        ofRectangle videoRect = ofRectangle(CVC::VIDEO_WIDTH + CVC::VIDEO_BORDER_SIZE * 2,
            CVC::VIDEO_BORDER_SIZE,
            CVC::VIDEO_WIDTH,
            CVC::VIDEO_HEIGHT);

        //step 1: clamping screenspace to image space
        int convertX = ofClamp(x, videoRect.getMinX(), videoRect.getMaxX());
        int convertY = ofClamp(y, videoRect.getMinY(), videoRect.getMaxY());

        //step2: mapping screenspace to image space (by shift coordinate origin
        convertX -= videoRect.getMinX();
        convertY -= videoRect.getMinY();

        //get the color using x + y * width function ...
        const int index = (convertX + convertY * m_colorImage.getWidth()) * m_colorImage.getPixels().getNumChannels();


        // if vector is empty (applying to first color) alter m_trackedColor
        if (currColorObject.size() == 0) {
            auto currTrackedColor = m_trackedColor;
            currTrackedColor[0] = m_colorImage.getPixels()[index + 0] / 255.0f;       //red, divide by 255.0f as m_trackedColor is a float color [0.0f, 1.0f]
            currTrackedColor[1] = m_colorImage.getPixels()[index + 1] / 255.0f;       //green
            currTrackedColor[2] = m_colorImage.getPixels()[index + 2] / 255.0f;       //blue
            currColorObject.push_back(1);
        }
        // if vector is not empty (applying to second color) alter m_trackedColor2
        else if (currColorObject.size() == 1) {
            auto currTrackedColor = m_trackedColor2;
            currTrackedColor[0] = m_colorImage.getPixels()[index + 0] / 255.0f;       //red, divide by 255.0f as m_trackedColor is a float color [0.0f, 1.0f]
            currTrackedColor[1] = m_colorImage.getPixels()[index + 1] / 255.0f;       //green
            currTrackedColor[2] = m_colorImage.getPixels()[index + 2] / 255.0f;       //blue
            currColorObject.clear();
        }

    }
}

void ofApp::searchGameKeyPress(int key) {
    //each key on a keyboard ha sa keycode associated with it
//32 = spacebar
    if (key == 32) {
        switch (m_appMode) {
        case CVC::APP_MODE::APP_VIDEO: {
            //set the opposite of what pause state it alrready is
            m_videoPlayer.setPaused(!m_videoPlayer.isPaused());
        }
                                     break;

        case CVC::APP_MODE::APP_CAM: {
            m_camPaused = !m_camPaused;
        }
                                   break;
        }
    }
    // switch debug value if shift is pressed (game view/debug view)
    else if (key == 3680) {
        debug = !debug;
    }

}