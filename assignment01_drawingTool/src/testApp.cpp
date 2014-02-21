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
    
    remoteX = 0;
    remoteY = 0;
    remoteLastX = 0;
    remoteLastY = 0;

    penColor = ofColor(0, 0, 0);
    remoteColor = ofColor(0, 0, 0);
    
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
    spacebrew.addPublish("posX", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("posY", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("lastPosX", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("lastPosY", Spacebrew::TYPE_RANGE);
    spacebrew.addPublish("color", Spacebrew::TYPE_STRING);
    // subscribers
    spacebrew.addSubscribe("button", Spacebrew::TYPE_BOOLEAN);
    spacebrew.addSubscribe("posX", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("posY", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("lastPosX", Spacebrew::TYPE_RANGE);
    spacebrew.addSubscribe("lastPosY", Spacebrew::TYPE_RANGE);
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
    if ( bSaveRequestReceived ) {
        lHeart.draw(400, 450);
    }
    if ( bSaveRequestSent && bSaveRequestReceived ) {
        fullHeart.draw(400, 450);
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
    
    // local drawing
    if ( isDrawing ) {
        ofLine(lastX, lastY, mouseX, mouseY);
        
        // every time we draw a line, send a message to the remote app
        spacebrew.sendRange("lastPosX", lastX);
        spacebrew.sendRange("lastPosY", lastY);
        spacebrew.sendRange("posX", mouseX);
        spacebrew.sendRange("posY", mouseY);

        lastX = mouseX;
        lastY = mouseY;
    }
    
    // draw remote information
    ofSetColor(remoteColor);
    ofLine(remoteLastX, remoteLastY, remoteX, remoteY);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    // resetting lastX and lastY so line will separate when unclick the mouse
    if ( !isDrawing ) {
        lastX = mouseX;
        lastY = mouseY;
        isDrawing = true;
    }
    
    // change local pen color and notify remote
    if ( ofDistSquared(mouseX, mouseY, 50, 450) < 225 ) {
        penColor.set(0, 0, 0); // black
        spacebrew.sendString("color", "black");
    } else if ( ofDistSquared(mouseX, mouseY, 100, 450) < 225 ) {
        penColor.set(255, 0, 0); // red
        spacebrew.sendString("color", "red");
    } else if ( ofDistSquared(mouseX, mouseY, 150, 450) < 225 ) {
        penColor.set(0, 255, 0); // green
        spacebrew.sendString("color", "green");
    } else if ( ofDistSquared(mouseX, mouseY, 200, 450) < 225 ) {
        penColor.set(0, 0, 255); // blue
        spacebrew.sendString("color", "blue");
    }
    
    if ( ofDistSquared(mouseX, mouseY, 400, 450) < 1600 ) {
        bSaveRequestSent = true;
        spacebrew.sendBoolean("button", true);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    isDrawing = false;
}

//--------------------------------------------------------------
void testApp::onMessage( Spacebrew::Message & msg ){
    
    cout << "received message, name is: " << msg.name << " and value is " << msg.value << endl;
    
    if (msg.name == "button") {
        bSaveRequestReceived = true;
    }
    
    if (msg.name == "color") {
        if ( msg.value == "black" ) {
            remoteColor.set(0, 0, 0);
        } else if ( msg.value == "red" ) {
            remoteColor.set(255, 0, 0);
        } else if ( msg.value == "green" ) {
            remoteColor.set(0, 255, 0);
        } else if ( msg.value == "blue" ) {
            remoteColor.set(0, 0, 255);
        }
    }
    
    char chars[] = "\"";
    if (msg.name == "posX") {
//        remoteX = ofToInt(msg.value);
        string tempX = msg.value;
        for ( unsigned int i = 0; i < strlen(chars); ++i) {
            tempX.erase (std::remove(tempX.begin(), tempX.end(), chars[i]), tempX.end());
        }
//        tempX = atoi(msg.value.c_str());
        remoteX = ofToInt(tempX);
        cout << "tempX: " << tempX << " remoteX: " << remoteX << endl;
        cout << "type is: " << typeid(msg.value).name() << endl;
        cout << "just value is " << msg.value << endl;
        cout << "with ofToInt: " << ofToInt(msg.value) << endl;
        cout << "with crazy c++ stuff: " << atoi(msg.value.c_str()) << endl;
        cout << "calling posX function and remoteX is now: " << remoteX << endl;
    }
    if (msg.name == "posY") {
        remoteY = ofToInt(msg.value);
    }
    if (msg.name == "lastPosX") {
        remoteLastX = ofToInt(msg.value);
    }
    if (msg.name == "lastPosY") {
        remoteLastY = ofToInt(msg.value);
    }
    
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
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}