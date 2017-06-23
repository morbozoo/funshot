#include "ofApp.h"

using namespace ofxCv;

void ofApp::addImage(string imgPath){
    ofImage img;
    img.load(imgPath);
    imgButtons.push_back(img);
}
void ofApp::setup() {
#ifdef TARGET_OSX
	//ofSetDataPathRoot("../data/");
#endif
    
    hasNoAcc = false;
    hasSunglasses = false;
    hasCap = false;
    hasHeadband = false;
    hasMustache = false;
    hasWig = false;
    hasNecklace = false;
    
    toggleAcc[0] = &hasNoAcc;
    toggleAcc[1] = &hasSunglasses;
    toggleAcc[2] = &hasCap;
    toggleAcc[3] = &hasHeadband;
    toggleAcc[4] = &hasMustache;
    toggleAcc[5] = &hasWig;
    
    hasNoFace = false;
    hasDogFace = false;
    hasCelebFace = false;
    hasGirlFace = false;
    hasObamaFace = false;
    hasGranpaFace = false;
    
    toggleFace[0] = &hasNoFace;
    toggleFace[1] = &hasDogFace;
    toggleFace[2] = &hasCelebFace;
    toggleFace[3] = &hasGirlFace;
    toggleFace[4] = &hasObamaFace;
    toggleFace[5] = &hasGranpaFace;
    
    
    //ImageSlideShow imgSlideAcc;
    imgSlideAcc.setup(0,10,ofGetWidth(),100,NULL);
    
    imgSlideAcc.addImage("ui/close.png");
    imgSlideAcc.addImage("ui/sunglass.png");
    imgSlideAcc.addImage( "ui/cap.png");
    imgSlideAcc.addImage( "ui/hairpin.png");
    imgSlideAcc.addImage("ui/mustache.png");
    imgSlideAcc.addImage( "ui/wig.png");
    
    imgSlideFace.setup(0,ofGetHeight()-100,ofGetWidth(),100,NULL);
    imgSlideFace.addImage("ui/close.png");
    imgSlideFace.addImage("ui/dog.png");
    imgSlideFace.addImage( "ui/leo.jpg");
    imgSlideFace.addImage( "ui/girl.png");
    imgSlideFace.addImage("ui/obama.png");
    imgSlideFace.addImage( "ui/grandpa.png");
    
    for(int i = 0; i < imgSlideFace.size; i++) {
        ofImage imgTmp;
        imgTmp.load("faces/" + std::to_string(i) + ".jpg");
        faceImages.push_back(imgTmp);
    }
    
    for(int i = 0; i < 5; i++) {
        ofImage imgTmp;
        imgTmp.load("ui/count" + std::to_string(i+1) + ".png");
        countDown[i] =imgTmp;
    }
    photoTaken = false;
    
	//ofSetVerticalSync(true);
	cloneReady = false;
    
	cameraX = 360 * 1.5*1.3;
	cameraY = 360 * 1.5;
	cam.initGrabber(cameraX, cameraY);
    
    
	ofFbo::Settings settings;
	settings.width = cam.getWidth();
	settings.height = cam.getHeight();
    
	//camTracker.setup();
	srcTracker.setup();
    /*
	srcTracker.setIterations(25);
    srcTracker.setClamp(3);
	srcTracker.setAttempts(3);
*/
    maxFaces = 2;
    multiTracker.setup(maxFaces, cam.getWidth(), cam.getHeight());
    
    for(int i = 0; i < maxFaces; i++) {
        cloneMulti.setup(cam.getWidth(), cam.getHeight());
 
        maskFbo.allocate(settings);
        srcFbo.allocate(settings);
        ofMesh mesh;
        camMesh.push_back(mesh);
        ofImage imgTmp;
        src.push_back(imgTmp);
        
        ofVec2f pos(-1,-1);
        lastGlassesPos.push_back(pos);
    }
    
    sunglasses.load("acc/sun2.png");
    sunglassVec.push_back(sunglasses);
    sunglasses.load("acc/sun3.png");
    sunglassVec.push_back(sunglasses);
    cap.load("acc/pharrell_hat.png");
    capVec.push_back(cap);
    /*cap.load("acc/hat.png");
    capVec.push_back(cap);
    */
     neck.load("acc/neck.jpg");
    mustache.load("acc/moustache2.png");
    wig.load("acc/wig1.png");
    headband.load("acc/bunnyhair.png");
    
    cameraButton.load("ui/camera2.png");
    
	currentFace = 0;
	
    if(faces.size()!=0){
		loadFace(currentFace);
	}

    
}

void ofApp::update() {
    
	cam.update();
    
    int xx = (cameraX - cameraY)/2;
    camImage = cam.getPixels();
    camImage.crop(xx, 0, cameraY, cameraY);
    
    camImage.mirror(false, true);
	if(cam.isFrameNew()) {
        
        multiTracker.update(toCv(camImage));
        
        for (int i = 0; i< multiTracker.trackers_active.size(); i++) {

            if (multiTracker.trackers_active[i]->getFound()) {
                
                ofMesh camMeshTmp = multiTracker.trackers_active[i]->getImageMesh();
                camMesh[i] = camMeshTmp;
                camMesh[i].clearTexCoords();
                camMesh[i].addTexCoords(srcPoints);
                
            }
        }
        maskFbo.begin();
        ofClear(0, 255);
        //ofEnableAlphaBlending();
        for (int i = 0; i< multiTracker.trackers_active.size(); i++) {
            
            if (multiTracker.trackers_active[i]->getFound()) {
        camMesh[i].draw();
            }
        }
        //ofDisableAlphaBlending();
        maskFbo.end();
        
        srcFbo.begin();
        ofClear(0, 255);
        src[0].bind();
        ofEnableAlphaBlending();
        for (int i = 0; i< multiTracker.trackers_active.size(); i++) {
            if (multiTracker.trackers_active[i]->getFound()) {
                camMesh[i].draw();
            }
        }
        ofDisableAlphaBlending();
        src[0].unbind();
        srcFbo.end();
        
        cloneMulti.setStrength(10);
        cloneMulti.update(srcFbo.getTexture(), camImage.getTexture(), maskFbo.getTexture());
	}
    
    int accChanged = imgSlideAcc.update();
    if(accChanged > 0) {
        cout << "accChanged: " << accChanged << endl;
        for(int i=0;i<6;i++){
            if(i == accChanged) {
               *toggleAcc[i] = true;
            } else {
                *toggleAcc[i] = false;
            }
        }
    } else if(accChanged == 0) {
        for(int i=0;i<6;i++){
            *toggleAcc[i] = false;
        }
    }

    int faceChanged = imgSlideFace.update();
    if(faceChanged >= 0) {
        cout << "faceChanged: " << faceChanged << endl;
        loadFace(faceChanged);
    }
    
    
}

void ofApp::exit() {
    multiTracker.exit();
}

void ofApp::drawButtons(){
    
    float height = imgSlideAccSize.y;
    float width = imgSlideAccSize.x;
    
    float x = imgSlideAccPos.x;
    float y = imgSlideAccPos.y;
    
    float gap = width/ ((visibleCount + 2) * (3+ 1.0/3.0));
    
    ofSetColor(255,255,255,255);
    imgButtons[0].draw(x + gap , y, gap * 2, gap * 2);
    
    for(int i = 0; i < visibleCount; i++) {
        //ImageButton button = buttons[i + visibleIdx];
        
        bool selected = false;
        if(i + visibleIdx == selectedIdx) {
            ofSetColor(255,255,255,255);
        } else {
            ofSetColor(255,255,255,100);
        }
        imgButtons[i + visibleIdx].draw(x + gap * (i + 2) + (gap * 2) * (i + 1), y, gap * 2, gap * 2);
        
        ofSetLineWidth(5);
        ofNoFill();
        if(i + visibleIdx == selectedIdx) {
            ofSetColor(0,255);
        } else {
            ofSetColor(0,100);
        }
        ofDrawRectangle(x + gap * (i + 2) + (gap * 2) * (i + 1), y, gap * 2, gap * 2);
        
    }
    ofSetColor(255,255,255,255);
    int lastIdx = imgButtons.size()-1;
    imgButtons[lastIdx].draw(x + gap * (visibleCount + 2) + (gap * 2) * (visibleCount + 1) , y, gap * 2, gap * 2);
    
}

void ofApp::draw() {
    ofSetColor(255);
 
    if(src[0].getWidth() > 0 && (multiTracker.trackers_active[0]->getFound() || multiTracker.trackers_active[1]->getFound()) ) {
            ofEnableAlphaBlending();
            cloneMulti.draw(0, 0);
            ofDisableAlphaBlending();
    }
    else {
       camImage.draw(0, 0,ofGetWidth(),ofGetHeight());
    }
    
    for (int i = 0; i< multiTracker.trackers_active.size(); i++) {
        if (multiTracker.trackers_active[i]->getFound()) {
            
            ofxFaceTracker *camTracker;
            camTracker = multiTracker.trackers_active[i];
            
            ofPolyline polyLE = camTracker->getImageFeature(camTracker->LEFT_EYE);
            ofPolyline polyRE = camTracker->getImageFeature(camTracker->RIGHT_EYE);
            ofPolyline polyJAW = camTracker->getImageFeature(camTracker->JAW);
            ofPolyline polyNose = camTracker->getImageFeature(camTracker->NOSE_BASE);
            
            ofMatrix4x4 rotMat = camTracker->getRotationMatrix();
            ofQuaternion quar = rotMat.getRotate();
            float scale = camTracker->getScale();
            
            ofEnableAlphaBlending();
            
            //float ratio = ratio = (polyRE.getCentroid2D().x - polyLE.getCentroid2D().x) /sunglasses.getWidth();
            
            float ratio = 0;
            if(i == 0) {
                ratio = 0.02;
            } else {
                ratio = 0.09;
            }
            
            float wd = sunglassVec[i].getWidth() * ratio * scale;
            float ht = sunglassVec[i].getHeight() * ratio * scale;
            
            float x = (polyRE.getCentroid2D().x + polyLE.getCentroid2D().x)/2.0;
            float y = polyRE.getCentroid2D().y;
            
            if(lastGlassesPos[i].x != -1) {
                x = (x + lastGlassesPos[i].x) / 2.0;
                y = (y + lastGlassesPos[i].y) / 2.0;
            }
            lastGlassesPos[i].x = x;
            lastGlassesPos[i].y = y;
            
            if(hasSunglasses) {
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateX(ofRadToDeg(quar.x()) * 2 );
                ofRotateY(ofRadToDeg(quar.y()) * -2 );
                ofRotateZ(ofRadToDeg(quar.z()) * 2 -5);
                ofSetColor(255, 255, 255, 200);
                
                sunglassVec[i].setAnchorPercent(0.5, 0.4);
                sunglassVec[i].draw(0, 0, wd, ht);
                
                ofPopMatrix();
            }
           
            if(hasCap) {
                /* wd: 55.224 ht: 40.284 */
                /* wd: 44.8 ht: 40.545 */
                wd = cap.getWidth() * 0.0655;
                ht = cap.getHeight() * 0.069;
                //cout<< "wd: "<< wd <<" ht: "<<ht<<endl;
                wd *= scale;
                ht *= scale;
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateX(ofRadToDeg(quar.x()) * -2 +20);
                ofRotateY(ofRadToDeg(quar.y()) * -2 );
                ofRotateZ(ofRadToDeg(quar.z()) * 2  );
                ofSetColor(255, 255, 255, 240);
                
                cap.setAnchorPercent(0.53, 0.7);
                cap.draw(0, 0, wd, ht);
                ofPopMatrix();
            }
            
            if(hasWig) {
                wd = wig.getWidth() * 0.099;
                ht = wig.getHeight() * 0.07;
                //cout<< "wd: "<< wd <<" ht: "<<ht<<endl;
                wd *= scale;
                ht *= scale;
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateX(ofRadToDeg(quar.x()) *-2 +20);
                ofRotateY(ofRadToDeg(quar.y()) *-2 -10);
                ofRotateZ(ofRadToDeg(quar.z()) * 2 );
                ofSetColor(255, 155, 205, 240);
                
                //wig.setAnchorPercent(0.5, 0.48);
                wig.setAnchorPercent(0.5, 0.75);

                wig.draw(0, 0, wd, ht);
                ofPopMatrix();
            }
            if(hasHeadband) {
                wd = headband.getWidth() * 0.043;
                ht = headband.getHeight() * 0.04;
                //cout<< "wd: "<< wd <<" ht: "<<ht<<endl;
                wd *= scale;
                ht *= scale;
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateX(ofRadToDeg(quar.x()) *-2 );
                ofRotateY(ofRadToDeg(quar.y()) *-2 );
                ofRotateZ(ofRadToDeg(quar.z()) * 2 );
                ofSetColor(205, 255, 255, 180);
                
                headband.setAnchorPercent(0.5, 1.2);
                headband.draw(0, 0, wd, ht);
                
                ofPopMatrix();
            }
            
            if(hasMustache) {
                /*
                wd = mustache.getWidth() * 0.03 * scale;
                ht = mustache.getHeight() * 0.03 * scale;
                 */
                wd = mustache.getWidth() * 0.009 * scale;
                ht = mustache.getHeight() * 0.008 * scale;
                
                x = polyNose.getCentroid2D().x;
                y = polyNose.getCentroid2D().y;
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofRotateX(ofRadToDeg(quar.x()) *-2 );
                ofRotateY(ofRadToDeg(quar.y()) *-2 );
                ofRotateZ(ofRadToDeg(quar.z()) * 2 -5 );
                ofSetColor(255, 255, 255, 200);
                
                //mustache.setAnchorPercent(0.5, -0.3);
                mustache.setAnchorPercent(0.5, 0.25);

                mustache.draw(0, 0, wd, ht);
                
                ofPopMatrix();
            }
            
            if(hasNecklace){
                wd = neck.getWidth() * 0.04 * scale;
                ht = neck.getHeight() * 0.03 * scale;
                
                x = polyJAW.getCentroid2D().x;
                y = polyJAW.getCentroid2D().y;
                
                ofPushMatrix();
                ofTranslate(x, y);
                
                ofSetColor(255, 255, 255, 150);
                
                neck.setAnchorPercent(0.5, -0.85);
                neck.draw(0, 0, wd, ht);
                
                ofPopMatrix();
            }
            ofDisableAlphaBlending();
        }
    }
    screenShot.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    
    imgSlideAcc.draw();
    imgSlideFace.draw();
    ofEnableAlphaBlending();
    cameraButton.setAnchorPercent(0.5, 0.5);
    cameraButton.draw(ofGetWidth()-70, ofGetHeight()/2, 100,100);
    
    if(countdownElaspedTime > 0) {
        float elapsed = ofGetElapsedTimef() - countdownStartTime;
        countdownElaspedTime -= elapsed;
        if(photoTakenCount == 3){
            countDown[int(countdownElaspedTime)].setAnchorPercent(0.5, 0.5);
            countDown[int(countdownElaspedTime)].draw(ofGetWidth()/2, ofGetHeight()/2);
        }
        countdownStartTime =ofGetElapsedTimef();
        
    } else {
        if(photoTaken == true) {
            screenShot.save("sc" + std::to_string(photoTakenCount) + ".jpg");
            //shot[photoTakenCount-1] = screenShot;
            countdownStartTime = ofGetElapsedTimef();
            countdownElaspedTime = 1;
            photoTakenCount--;
            blinkingTime = ofGetElapsedTimef();
            isBlinking = true;
        }
        if(photoTakenCount == 0) {
			if (photoTaken){
                
                //ofFbo photoFbo;
                //photoFbo.allocate(screenShot.getWidth(), screenShot.getHeight()*3, GL_RGBA);
                
                //photoFbo.begin();
                //ofClear(255,255,255,0);
                
                finalShot.allocate(screenShot.getWidth(), screenShot.getHeight()*3, OF_IMAGE_COLOR);
                //ofPixels shotPixels;
                //shotPixels.allocate(screenShot.getWidth(), screenShot.getHeight()*3+30, OF_IMAGE_COLOR);
                
                for(int i = 0; i < 3; i++) {
                    shot[i].load("sc" + std::to_string(i+1) + ".jpg");
                    //shotPixels.
                    ofPixels tmp = shot[i].getPixels();
                    tmp.pasteInto(finalShot, 0,shot[i].getHeight() * i);
                
                
                    //finalShot.setFromPixels(shot[i].getPixels(), <#int w#>, <#int h#>, <#ofImageType type#>)
                    //shot[i].draw(0,shot[i].getHeight() * i);
                    //shot[i].save("sccccc" + std::to_string(i+1) + ".jpg");
                }
                

                //photoFbo.end();
                
        
                //photoFbo.readToPixels(finalShot.getPixels());
                finalShot.update();
				finalShot.rotate90(1);
                finalShot.save("finalShot.jpg");
                
                
				photoUploader.startThread();
				photoUploader.stopThread();
			}
            photoTaken = false;
        }
    }
    
    float elapsedBlinking = ofGetElapsedTimef() - blinkingTime;
    if(elapsedBlinking < 0.5) {
        int alpha = (0.5 - elapsedBlinking) * 2 * 255;
        ofSetColor(255,alpha);
        ofFill();
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    ofDisableAlphaBlending();
    //drawButtons();
}



void ofApp::loadFace(int currentFace){
    
    if(currentFace == 0) {
        for(int i=0;i<maxFaces;i++) {
            src[i].allocate(faceImages[currentFace].getWidth(), faceImages[currentFace].getHeight(), OF_IMAGE_COLOR);;
            src[i].clear();
        }
        ofImage non;
        cout<<"currentFace"<<currentFace<<endl;
    } else {
        for(int i=0;i<maxFaces;i++) {
            src[i].allocate(faceImages[currentFace].getWidth(), faceImages[currentFace].getHeight(), OF_IMAGE_COLOR);
            src[i] = faceImages[currentFace];
        }
        cout<<"currentFace DO SOMETHING"<<currentFace<<endl;
        for(int i=0;i<3;i++){
            if(src[0].getWidth() > 0 && src[0].getHeight() > 0) {
                srcTracker.update(toCv(src[0]));
                srcPoints = srcTracker.getImagePoints();
            }
        }
    }
    
}

void ofApp::keyPressed(int key){
	switch(key){
	case OF_KEY_UP:
		currentFace++;
		break;
	case OF_KEY_DOWN:
		currentFace--;
		break;
	}
	currentFace = ofClamp(currentFace,0,faces.size()-1);
	if(faces.size()!=0){
        //cout<<"currentFace "<< currentFace << " size " <<faces.size()<< faces.getPath(currentFace)<<endl;
		loadFace(currentFace);
	}
}
void ofApp::mousePressed(ofMouseEventArgs & mouse) {
    cameraButton.draw(ofGetWidth()-70, ofGetHeight()/2, 100,100);
    if(ofGetWidth()-70-50< mouse.x && mouse.x < ofGetWidth()-70+50) {
        if(ofGetHeight()/2-50 < mouse.y && mouse.y < ofGetHeight()/2+50) {
            cout<< "photo taken";
            countdownStartTime = ofGetElapsedTimef();
            countdownElaspedTime = 3;
            
            photoTaken = true;
            photoTakenCount = 3;
        }
    }
    
}

