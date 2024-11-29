#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxImGui.h"
#include "ImHelpers.h"		//this gives us some extra ofxImGui features (needed for drop-down menus)

#include "CVC.h"

class ofApp : public ofBaseApp{

	public:
		ofVideoPlayer m_videoPlayer;
		ofVideoGrabber m_videoGrabber;
		bool m_videoResolutionConflict;		//do we need to re-size our video before we process?
		bool m_camResolutionConflict;		//do we need to re-size our video before we process?
		bool m_camPaused;					//pausing our camera feed

		bool debug = true;

		/* BOUNDRY PADDING */
		float boundryPadding = 50.0f;
		float spawnedObjBoundryPadding = 10.0f;
		float buttonBoundryPadding = 20.0f;

		/* HANDS */
		float handSize = 100.0f;

		ofxCvContourFinder m_hand1;

		ofVec2f hand1Center;
		ofVec2f hand1TL;
		ofVec2f hand1BR;

		ofImage handImage;

		/* dark area */
		ofImage darkAreaImage;

		/* background */
		ofImage backgroundImage;


		//CV images
		ofxCvColorImage		m_colorImage;
		ofxCvGrayscaleImage	m_grayscaleDiffImage;
		ofxCvGrayscaleImage	m_grayscaleDiffImage2;

		//GUI
		ofxImGui::Gui		m_gui;
		int					m_threshold;	//how closely does the colour have to match to track?
		int					m_numContoursConsidered;
		float				m_minArea;
		float				m_maxArea;

		//we want to select which color we are tracking within the video
		float m_trackedColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//ofxImgui requires color in this form
		float m_trackedColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//ofxImgui requires color in this form

		//app modes
		CVC::APP_MODE m_appMode;				//current app state
		std::vector<std::string> m_appModes;	//"list"/dynamic array of app states


		//we will do all color CV processing here
		//we are passing a reference here ...
		void processColor(ofxCvColorImage& image);

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		bool collisionCheck(ofVec2f obj1TL, ofVec2f obj1BR, ofVec2f obj2TL, ofVec2f obj2BR);
		void setHandBoundries();
		void setSpawnedObjBoundries();
		
};
