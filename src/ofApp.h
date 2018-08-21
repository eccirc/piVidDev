#pragma once

#include "ofMain.h"
#include "ofxThermalPrinter.h"
#include "RPiVideoGrabber.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();
    void printFrame();

    void keyPressed(int key);

    //piCam and Image buffer stuff

    RPiVideoGrabber piCam;

    deque<ofImage> ImgBuffer;

    ofPixels pixelData;

    //printer stuff

    ofxThermalPrinter myPrinter;

    //use this for counting function for printing

    int timer;

    //set up a buffer to store the frames

    int maxBuffersize;

    //width & height of the image buffer
    int w,h;

};
