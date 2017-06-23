
#include "ofMain.h"
#include "ImageButton.h"

class ImageSlideShow {
public:
    ImageSlideShow();
    ~ImageSlideShow();
    void addImage(string fileName);
    void setup(int x, int y, int width, int height, bool **toggleAcc);
    void draw();
    int update();
    int width, height;
    int x,y;
    int size;
    void mousePressed(ofMouseEventArgs & mouse);
    
private:
    vector<ofImage> images;
    int buttonArray[10];
    int buttonCount;
    vector<ImageButton > buttons;
    float gap;
    
    //bool **toggleAcc;
    bool toggleChanged;
    
    int selectedIdx;
    int visibleCount;
    int visibleIdx;
};