#pragma once
#include "ofMain.h"

class ofxVirtualKeyboard {
    public:
    
        // CLASS
        ofxVirtualKeyboard();
        ~ofxVirtualKeyboard();
    
        // SETUP
        void setup(int width, int padding, string fontPath, float labelSize = 1.0);
        void setShape(int width, int padding);
        void setPosition(ofVec2f position);
        void setLabel(string fontPath, float labelSize = 1.0);
        void setColors(ofColor button, ofColor buttonDown, ofColor label, ofColor labelDown);
    
        // EVENTS & STATES
        void enableMouseEvents();
        void disableMouseEvents();
        void enableTouchEvents();
        void disableTouchEvents();
    
        bool isKeyDown(int x, int y);
        string keyReleased();
    
        // DRAW
        void draw();
    
        // UTILS
        ofRectangle getBoundingBox();
        
        
    private:
    
        // SHAPE
        int padding;
        int keySize;
        int effectiveWidth;
        ofRectangle boundingBox;
    
        // Special: Enter Key
        ofPath enterKeyDisplay;
        ofPolyline enterKeyHitter;
    
        vector <ofRectangle> buttons;
        vector <ofRectangle> createButtons(int _effectiveWidth, int _padding, int _keySize);
    
        ofVec2f translation = ofVec2f(0,0);

        // STYLE
        // Colors
        ofColor buttonColor = ofColor::white;
        ofColor buttonDownColor = ofColor::black;
        ofColor labelColor = ofColor::grey;
        ofColor labelDownColor = ofColor::white;
    
        // Labels
        ofTrueTypeFont font;
        ofTrueTypeFont fontText;
        vector <ofPoint> fontPosition;
        bool loadFont = true;
    
        void createLabels();

        vector <string> keysLabel = { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<<", "", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "Caps", "a", "s", "d", "f", "g", "h", "j", "k", "l", "Shift", "z", "x", "c", "v", "b", "n", "m", ".", "_", "@", "Enter" };
    

        // EVENTS & STATES
        bool usingMouseEvents = false;
        bool usingTouchEvents = false;

        void onMouseReleased(ofMouseEventArgs& data);
        void onMouseDragged(ofMouseEventArgs& data);
        void onMousePressed(ofMouseEventArgs& data);
    
        void onTouchUp(ofTouchEventArgs &data);
        void onTouchDown(ofTouchEventArgs &data);
        void onTouchMoved(ofTouchEventArgs &data);
    
        int keyDown = -1;
        bool enterKeyDown = false;

        // Caps Lock Key
        bool isCapsLockActive = false;
        
        // Shift Key
        bool isShiftActive = false;
    };
