#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxCv.h"
#include "Clone.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ofxFaceTrackerMulti.h"
#include "ImageSlideShow.h"
#include "insta.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void loadFace(int currentFace);
    void exit();
	void keyPressed(int key);
    void mousePressed(ofMouseEventArgs & mouse);
    void addImage(string imgPath);
    void drawButtons();
    
	ofxFaceTrackerThreaded camTracker;
	ofVideoGrabber cam;
    ofImage camImage;
    
	ofxFaceTracker srcTracker;
	vector<ofImage> src;
	vector<ofVec2f> srcPoints;
	
	bool cloneReady;
	Clone cloneMulti;
	ofFbo srcFbo, maskFbo;
    vector<ofMesh> camMesh;
    
    bool hasNoAcc;
    bool hasSunglasses;
    bool hasCap;
    bool hasMustache;
    bool hasWig;
    bool hasHeadband;
    bool hasNecklace;
    
    bool hasNoFace;
    bool hasDogFace;
    bool hasCelebFace;
    bool hasGirlFace;
    bool hasObamaFace;
    bool hasGranpaFace;
    
    bool *toggleAcc[10];
    bool *toggleFace[10];
    
    ofImage sunglasses;
    ofImage cap;
    ofImage neck;
    ofImage mustache;
    ofImage wig;
    ofImage headband;
    
    ofImage cameraButton;
    
	int cameraX, cameraY;
    ofImage screenShot;
    ofImage shot[5];
    ofImage finalShot;
    ofImage countDown[5];
    
    bool photoTaken;
    int photoTakenCount;
    bool isBlinking;
    float blinkingTime;
	string timeStamp;
    
    ImageSlideShow imgSlideAcc;
    ImageSlideShow imgSlideFace;
    
    vector<ofImage> sunglassVec;
    vector<ofImage> capVec;
    vector<ofImage> neckVec;
    vector<ofImage> faceImages;
    
    float countdownStartTime;
    float countdownElaspedTime;
    
	ofDirectory faces;
	int currentFace;
    
    ofxFaceTrackerMulti multiTracker;
    int maxFaces;
    
    vector<ofVec2f> lastGlassesPos;

    ofVec2f imgSlideAccPos;
    ofVec2f imgSlideAccSize;
    vector<ofImage> imgButtons;
    int selectedIdx;
    int visibleCount;
    int visibleIdx;
    
    //INSTAGRAM
	insta photoUploader;
    
};
