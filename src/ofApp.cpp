//originally created as 'vidDev' around 07/2018, now in the process of modifying to run on RPi with native camera support
//This version, piVidDev started on 10/08/2018 by David Williams, https://github.com/eccirc
//This branch created 21/08/2018
//very much reduced the complexity of the original 'vidDev' program (not that it was that complex!)
//but this version will just hsow four tiles with each at a different point in time - hopefully to display some good slime mould growth comparison


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

    w = ofGetWidth()/2;
    h = ofGetWidth()/2;
    //Rpi Cam stuff
    piCam.initGrabber(w, h);
    //set a big max buffer size so we can 'look back in time' by a fair while
    maxBuffersize = 1000;
}

//--------------------------------------------------------------
void ofApp::update(){

    piCam.update();
    //save video data into an ofImage deque to store the individual frames
    if(piCam.isFrameNew()){
        //to push to the deque
        //'normal' image
        ofImage frameImage;
        frameImage.setFromPixels(piCam.getPixels());
        frameImage.mirror(false, true);
        ImgBuffer.push_front(frameImage);
    }
    //remove frames once max buffer size has been reached
    if(ImgBuffer.size() > maxBuffersize){
        ImgBuffer.pop_back();
    }
    //timer for the print function
    timer ++;

    if(timer >= maxBuffersize){
        timer = 0;
    }


}

//--------------------------------------------------------------
void ofApp::draw(){

    piCam.draw(0,0,w,h);

    if(ImgBuffer.size() == maxBuffersize){
        ImgBuffer[250].draw(w,0,w,h);
        ImgBuffer[500].draw(0,h,w,h);
        ImgBuffer[750].draw(w,0,w,h);
        ImgBuffer[999].draw(w,h,w,h);
    }


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
        myPrinter.print(ImgBuffer[0]);

    }

}

//--------------------------------------------------------------
void ofApp::printFrame(){
    if(timer == maxBuffersize){
        myPrinter.print(ImgBuffer[0]);
    }

}
