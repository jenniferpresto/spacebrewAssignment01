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
    int posCounter;
    // including temp variables to make sure everything is received
    string tempX, tempY, tempLX, tempLY; // these will hold remote variables temporarily
    bool txRecd, tyRecd, tlxRecd, tlyRecd;

    
    bool bSaveRequestSent;
    bool bSaveRequestReceived;
};
