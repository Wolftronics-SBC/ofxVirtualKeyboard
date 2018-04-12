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
    
        string checkForKeyDown(int x, int y);
        string getVirtualKeyReleased();

		ofEvent<string> virtualKeyReleased;
    
        // DRAW
        void draw();
    
        // UTILS
        ofRectangle getBoundingBox();
        
        
    private:

		struct KeyboardSettings
		{
			ofVec2f translation = ofVec2f(0, 0);
			int effectiveWidth;
			ofRectangle boundingBox;
			int padding;
			int keySize;
		};
        
		KeyboardSettings keyboardSettings;

		// SHAPE
		// Keys
		struct RectangleKey
		{
			string label;
			ofPoint labelPostion;
			ofRectangle button;
		};

		vector <RectangleKey> keys;

        // Custom Key
		struct CustomKey
		{
			string label;
			ofPoint labelPostion;
			ofPath buttonDisplay;
			ofPolyline buttonHit;
		};

		CustomKey enterKey;
        
    

        void createButtons(int _effectiveWidth, int _padding, int _keySize);
    

        // STYLE
        // Colors
        ofColor buttonColor = ofColor::white;
        ofColor buttonDownColor = ofColor::black;
        ofColor labelColor = ofColor::grey;
        ofColor labelDownColor = ofColor::white;
    
        // Labels
        ofTrueTypeFont font;
        ofTrueTypeFont fontText;
        bool loadFont = true;
    
        void setLabelsPosition();

        vector <string> labels = { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "Backspace", "", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "Caps", "a", "s", "d", "f", "g", "h", "j", "k", "l", "Shift", "z", "x", "c", "v", "b", "n", "m", ".", "_", "@", "Enter"};
    

        // EVENTS & STATES
        bool usingMouseEvents = false;
        bool usingTouchEvents = false;

        void onMouseReleased(ofMouseEventArgs& data);
        void onMouseDragged(ofMouseEventArgs& data);
        void onMousePressed(ofMouseEventArgs& data);
    
        void onTouchUp(ofTouchEventArgs &data);
        void onTouchDown(ofTouchEventArgs &data);
        void onTouchMoved(ofTouchEventArgs &data);

		string labelToReturn = "";

    
        int keyDown = -1;
        bool enterKeyDown = false;

        // Caps Lock Key
        bool isCapsLockActive = false;
        
        // Shift Key
        bool isShiftActive = false;
    };
