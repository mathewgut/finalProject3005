#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){

    /* CONTROLLER IMAGES */
    handImage.load("hand-left.png");
    handImage.resize(handSize, handSize);

    handImage.setAnchorPoint(handImage.getWidth() / 2, handImage.getHeight() / 2);

    /* DARK AREA IMAGE OVERLAY */
    darkAreaImage.load("dark-area.png");
    darkAreaImage.setAnchorPoint(darkAreaImage.getWidth()/2, darkAreaImage.getHeight()/2);

    /* BACKGROUND IMAGE */
    backgroundImage.load("test-background.jpg");
    backgroundImage.setAnchorPoint(backgroundImage.getWidth() / 2, backgroundImage.getHeight() / 2);

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
}

//--------------------------------------------------------------
void ofApp::update(){
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
}

//--------------------------------------------------------------
void ofApp::draw(){
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

        // loop for drawing hand 1
        for (int i = 0; i < m_hand1.nBlobs; i++) {
            contourCenter.set(m_hand1.blobs[i].boundingRect.getCenter().x,
                m_hand1.blobs[i].boundingRect.getCenter().y);
            contourArea = m_hand1.blobs[i].area;


            
            // remap to convert the lower resolution 640x360 to 1280x720 for drawing hand
            hand1Center.x = ofMap(contourCenter.x, 0, 640, 0, 1280);
            hand1Center.y = ofMap(contourCenter.y, 0, 360, 0, 720);

            
            handImage.draw(hand1Center.x, hand1Center.y);
            darkAreaImage.draw(hand1Center.x, hand1Center.y);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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


//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}


// stores which object color is being applied to
vector<int> currColorObject;
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

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

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
