#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255);
    ofSetBackgroundAuto(FALSE);
    ofSetColor(0);
    ofSetCircleResolution( 100 );
    ofSetLineWidth(3);
    
    isDrawing = false;
    lastX = 0;
    lastY = 0;

    penColor = ofColor(0, 0, 0);
    
    gHeart.loadImage("gHeart.png");
    lHeart.loadImage("lHeart.png");
    rHeart.loadImage("rHeart.png");
    fullHeart.loadImage("fullHeart.png");
    gHeart.setAnchorPercent(0.5, 0.5);
    lHeart.setAnchorPercent(0.5, 0.5);
    rHeart.setAnchorPercent(0.5, 0.5);
    fullHeart.setAnchorPercent(0.5, 0.5);
    
    bSaveRequestSent = false;
    
    // Spacebrew stuff--------------
    // spacebrew connection stuff
    string host = Spacebrew::SPACEBREW_CLOUD;
    string name = "ofxSideOfTool";
    string description = "Drawing tool for lovers";
    
    // publishers
    spacebrew.addPublish("button", Spacebrew::TYPE_BOOLEAN);
    spacebrew.addPublish("lastPosX", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("lastPosY", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("posX", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("posY", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("color", Spacebrew::TYPE_STRING);
    // subscribers
    spacebrew.addSubscribe("button", Spacebrew::TYPE_BOOLEAN);
    spacebrew.addSubscribe("lastPosX", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("lastPosY", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("posX", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("posY", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("color", Spacebrew::TYPE_STRING);
    
    // connection and listener
    spacebrew.connect( host, name, description );
    Spacebrew::addListener(this, spacebrew);
    
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    // gray rectangle
    ofSetColor(240);
    ofRect(0, 430, ofGetWindowWidth(), 40);
    
    // black dot
    ofSetColor(0);
    ofCircle(50, 450, 15);
    
    // red dot
    ofSetColor(255, 0, 0);
    ofCircle(100, 450, 15);
    
    // green dot
    ofSetColor(0, 255, 0);
    ofCircle(150, 450, 15);
    
    // blue dot
    ofSetColor(0, 0, 255);
    ofCircle(200, 450, 15);
    
    // heart indicator
    ofSetColor(255);
    gHeart.draw(400, 450);
    
    // if the drawer has requested to save, draw a half-heart
    // (will just draw on top of other one
    if ( bSaveRequestSent ) {
        rHeart.draw(400, 450);
    }
    
    // drawing indicator for what color is selected
    ofNoFill();
    ofSetColor(100);
    if (penColor == ofColor(0, 0, 0) ) {
        ofCircle(50, 450, 18);
    } else if (penColor == ofColor(255, 0, 0) ) {
        ofCircle(100, 450, 18);
    } else if (penColor == ofColor(0, 255, 0) ) {
        ofCircle(150, 450, 18);
    } else if (penColor == ofColor(0, 0, 255) ) {
        ofCircle(200, 450, 18);
    }
    ofFill();

    ofSetColor( penColor );
        
    if ( isDrawing ) {
        ofLine(lastX, lastY, mouseX, mouseY);
        
        // every time we draw a line, send a message to the remote app
        spacebrew.sendRange("lastPosX", lastX);
        spacebrew.sendRange("lastPosY", lastY);
        spacebrew.sendRange("posX", mouseX);
        spacebrew.sendRange("posX", mouseY);

        if (penColor == ofColor(0, 0, 0) ) {
            spacebrew.sendString("color", "black");
        } else if (penColor == ofColor(255, 0, 0) ) {
            spacebrew.sendString("color", "red");
        } else if (penColor == ofColor(0, 255, 0) ) {
            spacebrew.sendString("color", "green");
        } else if (penColor == ofColor(0, 0, 255) ) {
            spacebrew.sendString("color", "blue");
        }

        lastX = mouseX;
        lastY = mouseY;

    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // resetting lastX and lastY so line will separate when unclick the mouse
    if ( !isDrawing ) {
        lastX = mouseX;
        lastY = mouseY;
        isDrawing = true;
    }
    
    // change pen color
    if ( ofDistSquared(mouseX, mouseY, 50, 450) < 225 ) {
        penColor.set(0, 0, 0); // black
    } else if ( ofDistSquared(mouseX, mouseY, 100, 450) < 225 ) {
        penColor.set(255, 0, 0); // red
    } else if ( ofDistSquared(mouseX, mouseY, 150, 450) < 225 ) {
        penColor.set(0, 255, 0); // green
    } else if ( ofDistSquared(mouseX, mouseY, 200, 450) < 225 ) {
        penColor.set(0, 0, 255); // blue
    }
    
    if ( ofDistSquared(mouseX, mouseY, 400, 450) < 1600 ) {
        bSaveRequestSent = true;
    }
}


//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & msg ){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    isDrawing = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}