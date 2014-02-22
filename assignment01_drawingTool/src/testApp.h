/*******************************
 
 Two-person drawing tool
 Two people on separate devices draw one picture;
 when they both hit the button indicating they're
 done, the app saves the image to a file.
 
 Spacebrew, Assignment 1
 February 25, 2014
 
 Jennifer Presto
 Susan
 
 ******************************/

#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // spacebrew stuff
    Spacebrew::Connection spacebrew;
    void onMessage( Spacebrew::Message & msg );
    
    int convertProcessingRangeMessage( string recdMsg );
    
    // basic drawing information
    int lastX, lastY;
    int remoteX, remoteY;
    int remoteLastX, remoteLastY;
    bool isDrawing;
    ofColor penColor;
    ofColor remoteColor;
    ofImage gHeart, lHeart, rHeart, fullHeart;
    
    // variables to test data to make sure we're getting everything
    int currentFrame, lastFrame;
    // including temp variables to make sure everything is received
    int tempX, tempY, tempLX, tempLY; // these will hold remote variables temporarily
    bool txRecd, tyRecd, tlxRecd, tlyRecd;

    
    bool bSaveRequestSent;
    bool bSaveRequestReceived;
};
