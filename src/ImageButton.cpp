#include "ImageButton.h"


ImageButton::ImageButton() {
    
}
ImageButton::ImageButton(int width, int height, string imgPath){
    this->width = width;
    this->height = height;
    
    image.allocate(width, height, OF_IMAGE_COLOR);
    image.load(imgPath);

}
void ImageButton::setup(int width, int height, string imgPath){
    
}
void ImageButton::update() {
    image.update();
}

void ImageButton::draw(float x, float y, bool selected, bool arrow){
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    
    if(selected) {
        ofSetColor(255,255,255,255);
    } else {
        ofSetColor(255,255,255,150);
    }
    
    if(image.isAllocated()){
        image.draw(x, y, width, height);
    }
    if(!arrow) {
        if(selected) {
            ofSetColor(0,255);
        } else {
            ofSetColor(0,150);
        }
        ofSetLineWidth(5);
        ofNoFill();
        ofDrawRectangle(x, y, width, height);
    }
    
    ofDisableAntiAliasing();
    ofDisableAlphaBlending();
    
    
}

void ImageButton::mousePressed(ofMouseEventArgs & mouse){
    //cout<<mouse.x<< " "<< mouse.y<<endl;
    
}