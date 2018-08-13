#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxThermalPrinter.h"
#include "RPiVideoGrabber.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);

    //cam stuff - this is for regular (possibly usb on pi - not yet tested as of 10/08/18 comment out for now and use the RPiGrabber instead
    //ofVideoGrabber myCam;

    RPiVideoGrabber piCam;

    deque<ofImage> ImgBuffer;

    deque<ofImage> sGrab;

    deque<ofImage> sGrab2;

    ofPixels pixelData;

    //printer stuff

    ofxThermalPrinter myPrinter;



    int maxBuffersize;

    //width & height of the image buffer
    int w,h;
    //location variables
    int posX,posY;
    //size of dots
    int grainSize;
    //colour mapping to brightness
    int colBright;






};
