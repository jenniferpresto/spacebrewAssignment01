#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(30);
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
    
    currentFrame = ofGetFrameNum();
    lastFrame = ofGetFrameNum();
    tempX = 0;
    tempY = 0;
    tempLX = 0;
    tempLY = 0;
    txRecd = false;
    tyRecd = false;
    tlxRecd = false;
    tlyRecd = false;
    
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
    if ( bSaveRequestReceived && bSaveRequestSent ) {
        ofSaveFrame();
        bSaveRequestReceived = false;
        bSaveRequestSent = false;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //    cout << "frame at beginning of draw: " << ofGetFrameNum() << endl;
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
    
    //    cout << "frame at end of draw: " << ofGetFrameNum() << endl;
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
    
    //    cout << "received message, name is: " << msg.name << " and value is " << msg.value << endl;
    
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
    
    // temp variables will hold values from message;
    // we will only change the actual variables to affect the drawing if all four temp variables are received
    // in the same frame;
    
    if (msg.name == "posX" || msg.name == "posY" || msg.name == "lastPosX" || msg.name == "lastPosY") {
        
        // always reset the current frame
        currentFrame = ofGetFrameNum();
        //        cout << "currentFrame: " << currentFrame << endl;
        //        cout << "last Frame: " << lastFrame << endl;
        // this function is called multiple times per frame, want to accumulate if currentFrame and lastFrame match
        // but if not, reset everything
        if ( currentFrame > lastFrame ) {
            lastFrame = currentFrame;
            txRecd = false;
            tyRecd = false;
            tlxRecd = false;
            tlyRecd = false;
            //            cout << "resetting the booleans!" << endl;
        }
    }
    
    //    char chars[] = "\""; // variable for removing quotation marks
    
    if (msg.name == "posX") {
        tempX = convertProcessingRangeMessage(msg.value);
        cout << "tempX: " << tempX << endl;
        txRecd = true;
    }
    
    if (msg.name == "posY") {
        tempY = convertProcessingRangeMessage(msg.value);
        tyRecd = true;
    }
    if (msg.name == "lastPosX") {
        tempLX = convertProcessingRangeMessage(msg.value);
        tlxRecd = true;
    }
    if (msg.name == "lastPosY") {
        tempLY = convertProcessingRangeMessage(msg.value);
        tlyRecd = true;
    }
    
    // if we've received all four positions within the same frame, use them
    if( (currentFrame == lastFrame) && txRecd && tyRecd && tlxRecd && tlyRecd ) {
        //        cout << "all conditions satisfied" << endl;
        remoteX = tempX;
        remoteY = tempY;
        remoteLastX = tempLX;
        remoteLastY = tempLY;
    }
    
}


//--------------------------------------------------------------
int testApp::convertProcessingRangeMessage( string recdMsg ) {
    
    recdMsg.erase (std::remove(recdMsg.begin(), recdMsg.end(), '\"'), recdMsg.end());
    return ofToInt(recdMsg);
    
    /******************
     Alternate way to parse message
     Not currently working: includes blank character at the end
     
     cout << "recdMsg: " << recdMsg << endl;
     
     char * value = new char[recdMsg.length() + 1];
     strcpy(value, recdMsg.c_str());
     
     int final = 0;
     for (char * i = value; *i; ++i) {
         if (*i != '\"') {
             final = (final * 10) + ((*i) - '0');
             
             cout << "*i: " << *i << endl;
         }
     }
     
     delete[] value;
     return final;
     
     ******************/
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