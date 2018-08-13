//originally created as 'vidDev' around 07/2018, now in the process of modifying to run on RPi with native camera support
//This version, piVidDev started on 10/08/2018 by David Williams, https://github.com/eccirc
//To do:
//*Make this work with identical/simialr functionality to the original
//*create and RPi cross compiler to speed up compiling and debugging - rather than having to do it on the Pi itseld (this will save a lot of time!


#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);
    //ofSetFrameRate(5);

    //initialise the printer for use with RX & TX (serial send and recieve pins) on the pi
    myPrinter.open("/dev/serial0");
    //these settings seem to work well!
    myPrinter.setControlParameter(1, 100, 10);
    //Settings to play with
    //density
    myPrinter.setPrintDensity(1,1);


    ofSetBackgroundColor(255);
//    w = ofGetWidth()/2;
//    h = ofGetHeight()/2;

    w = 352;
    h = 288;

    grainSize = 1;

    //camera settins for non-pi cam, comment out for now
     //myCam.setDesiredFrameRate(5);
     //myCam.initGrabber(w,h);

    //Rpi Cam stuff
    piCam.setDesiredFrameRate(5);
    piCam.initGrabber(w, h);


    //set max buffer size to be the same as the width (for now)
    maxBuffersize = w*2;
}

//--------------------------------------------------------------
void ofApp::update(){
    //as above, use RPi cam and comment out for use with Pi
    //myCam.update();
    piCam.update();
    //save video data into an ofImage deque to store the individual frames
    if(piCam.isFrameNew()){
        //to push to the deque
        //'normal' image
        ofImage frameImage;
        frameImage.setFromPixels(piCam.getPixels());
        frameImage.mirror(false, true);
        ImgBuffer.push_front(frameImage);
        //alt image #1
        ofImage aI1;
        aI1.grabScreen(0,0, w, h);
        sGrab.push_front(aI1);
        //alt image #2
        ofImage aI2;
        //change this to include more/less of the image
        aI2.grabScreen(0, h, w, h);
        sGrab2.push_front(aI2);

    }
    //remove frames once max buffer size has been reached
    if(ImgBuffer.size() > maxBuffersize){
        ImgBuffer.pop_back();
        sGrab.pop_back();
        sGrab2.pop_back();
    }


}

//--------------------------------------------------------------
void ofApp::draw(){
    //myCam.draw(0,0);

    //for the binarising bit, set between 0-255, find which one works best for the light levels
    int threshold = 150;

    for(int x = 0; x < w; x += grainSize*4){
        for(int y = 0; y < h; y += grainSize*4){
            ofPushStyle();
            ofColor colXY = ImgBuffer[0].getColor(x,y);
            int colBright = colXY.getBrightness();
            float colMap = ofMap(colBright, 0, 255, 20, 0);
            int b;
            //binarise the values to get just black or white
            if(colBright < threshold){
                b = 0;
            }
            else if(colBright > threshold){
                b = 255;
            }
            //different ways of setting the colour - 'colBright' gives more detailed greyscale, 'b' is binary contrast(black or white)
            ofSetColor(0);
            //ofSetCircleResolution(20);
            //ofSetColor(colBright);
            //use 'grain size' to draw a uniform grid, or use 'colMap' for brightness to size mapping
            //ofDrawCircle(x,y, grainSize);
            OF_RECTMODE_CENTER;
            ofNoFill();
            ofDrawRectangle(x,y, colMap, colMap);
            ofPopStyle();

        }
    }



    //Now to figure out how to get a slitscan from the image above...
        for(int i = 0; i < sGrab.size(); i ++){
            ofPushStyle();
            ofSetColor(255);
            //vertical scan
            sGrab[i].drawSubsection(i,h,grainSize,h,i, 0);
            //horizontal scan
            sGrab[i].drawSubsection(0, i + h + h,w,grainSize, 0, i);
            ofPopStyle();
        }
//    for(int i = 0; i < sGrab.size(); i ++){
//        sGrab[sGrab.size() - 1].draw(0,h,w,h);
//    }

}
void ofApp::exit(){
    myPrinter.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'p'){
        //with a for loop this prints quite a lot...
//        for(int i = 0; i < sGrab2.size(); i += 10){
//        myPrinter.print(sGrab2[i]);
//        }
        myPrinter.print(sGrab2[0]);

    }

}

//--------------------------------------------------------------
