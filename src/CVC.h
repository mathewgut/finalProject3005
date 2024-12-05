#pragma once

#include "ofMain.h"

namespace CVC {
    static const int APP_WINDOW_WIDTH       = 1280;
    static const int APP_WINDOW_HEIGHT      = 720;
    static const int APP_DESIRED_FRAMERATE  = 60;
    static const int CAM_DESIRED_FRAMERATE  = 30;

    static const std::string VIDEO_PATH_COLOR = "";
    static const int VIDEO_WIDTH    = 640;      //we will use these values for BOTH cam and video
    static const int VIDEO_HEIGHT   = 360;

    static const int VIDEO_BORDER_SIZE      = 10;

    typedef enum TYPE {
        APP_VIDEO,            //live feed
        APP_CAM           //simulated feed
    } APP_MODE;
};
