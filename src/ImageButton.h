
#include "ofMain.h"

class ImageButton {
public:
    ImageButton();
    ImageButton(int width, int height, string imgPath);
    void setup(int width, int height, string imgPath);
    void draw(float x, float y, bool selected, bool arrow);
    void update();
    int width, height;
    void mousePressed(ofMouseEventArgs & mouse);
private:
    ofImage image;
    bool selected;
};