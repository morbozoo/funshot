#include "ImageSlideShow.h"



ImageSlideShow::ImageSlideShow(){
    buttons.resize(2);
}
ImageSlideShow::~ImageSlideShow() {
    ofRemoveListener(ofEvents().mousePressed, this, &ImageSlideShow::mousePressed);
}

void ImageSlideShow::setup(int x, int y, int width, int height, bool **toggle) {
    
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    
    selectedIdx = -1;
    visibleCount = 3;
    visibleIdx = 1;
    size = 0;
    
    //toggleAcc = toggle;
    
    gap = width/ ((visibleCount + 2) * (3+ 1.0/3.0));

    ofImage *backImg  = new ofImage("ui/back.png");
    ofImage *forwardImg = new ofImage("ui/forward.png");

    ImageButton back(gap * 2, gap * 2, "ui/back.png");
    ImageButton forward(gap * 2, gap * 2,"ui/forward.png");

    
    buttons[0] = back;
    buttons[1] = forward;

    
    toggleChanged = false;
    
    ofAddListener(ofEvents().mousePressed, this, &ImageSlideShow::mousePressed);
}

void ImageSlideShow::addImage(string fileName) {

    ImageButton button(gap * 2, gap * 2, fileName);
    
    buttons.insert(buttons.end()-1,button);
    size++;
}


void ImageSlideShow::draw(){
    

    buttons[0].draw(x + gap , y, false, true);
    for(int i = 0; i < visibleCount; i++) {
        //ImageButton button = buttons[i + visibleIdx];
        
        bool selected = false;
        if(i + visibleIdx == selectedIdx) {
            selected = true;
        }
        buttons[i + visibleIdx].draw(x + gap * (i + 2) + (gap * 2) * (i + 1), y, selected,false);

        
    }
    int lastIdx = buttons.size()-1;
    buttons[lastIdx].draw(x + gap * (visibleCount + 2) + (gap * 2) * (visibleCount + 1) , y, false, true);
    
    
}
int ImageSlideShow::update() {
    for(ImageButton button: buttons) {
        button.update();
    }
    if(toggleChanged) {
        toggleChanged = false;
        return selectedIdx - 1;
    }
    return -1;

}

void ImageSlideShow::mousePressed(ofMouseEventArgs & mouse){
    int idx = -1;
    
    for(int i = 0; i < visibleCount + 2; i++) {
        int xx = this->x + gap * (i + 1) + (gap * 2) * i;
        //cout<< "xx: "<< xx<< endl;
        if(xx < mouse.x && mouse.x < xx + gap * 2
           && this->y < mouse.y && mouse.y < this->y + gap * 2) {
            idx = i;
            
        }
    }
    if(idx == -1) {
        
    } else if(idx == 0) {
        cout << "back" << endl;
        visibleIdx = (visibleIdx > 1) ? visibleIdx - 1 : visibleIdx;
        
    } else if(idx == visibleCount + 1) {
        cout << "forward" << endl;
        visibleIdx = (visibleIdx + visibleCount < buttons.size() - 1) ? visibleIdx + 1 : visibleIdx;
    } else {
        selectedIdx = idx +  visibleIdx - 1;
        //cout<< "point: "<< idx << " idx: "<< idx +  visibleIdx - 1 << endl;
    /*
        for(int i=0; i< buttons.size()-2; i++) {
            if(toggleAcc != NULL) {
                if(i==selectedIdx-1) {
                    *toggleAcc[selectedIdx-1]=true;
                } else {
                    *toggleAcc[i]=false;
                }
            }
        }
     */
        toggleChanged = true;
    }
    //cout<<mouse.x<< " "<< mouse.y<<endl;
    
}