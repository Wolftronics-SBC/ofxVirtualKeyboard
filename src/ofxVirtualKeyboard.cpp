#include "ofxVirtualKeyboard.h"

ofxVirtualKeyboard::ofxVirtualKeyboard()
{
	for (int i = 0; i < labels.size() - 1; i++)
	{
		RectangleKey newKey = RectangleKey();
		newKey.label = labels[i];

		keys.push_back(newKey);
	}

	enterKey.label = labels[labels.size() - 1];
}

ofxVirtualKeyboard::~ofxVirtualKeyboard()
{
    if (usingMouseEvents) disableMouseEvents();
    if (usingTouchEvents) disableTouchEvents();
}

//--------------------------------------------------------------
void ofxVirtualKeyboard::setup(int width, int padding, string labelFontPath, float labelSize)
{
    setShape(width, padding);
	setLabel(labelFontPath, labelSize);
}

void ofxVirtualKeyboard::setShape(int width, int padding)
{
	keyboardSettings.padding = padding;
    
	keyboardSettings.keySize = (width - padding * 11) / 13;
	keyboardSettings.effectiveWidth = keyboardSettings.keySize * 13 + padding * 11;
    float effectiveHeight = keyboardSettings.keySize * 4 + padding * 3;
    
	keyboardSettings.boundingBox = ofRectangle(0, 0, keyboardSettings.effectiveWidth, effectiveHeight);
    
    createButtons(keyboardSettings.effectiveWidth, padding, keyboardSettings.keySize);
}

void ofxVirtualKeyboard::setPosition(ofVec2f position)
{
	keyboardSettings.translation = position;
}

void ofxVirtualKeyboard::setLabel(string path, float labelSize)
{
    font.load(path, keyboardSettings.keySize / 3 * labelSize);
    fontText.load(path, keyboardSettings.keySize / 4 * labelSize);
    setLabelsPosition();
}

void ofxVirtualKeyboard::setColors(ofColor button, ofColor buttonDown, ofColor label, ofColor labelDown)
{
    this->buttonColor = button;
    this->buttonDownColor = buttonDown;
    this->labelColor = label;
    this->labelDownColor = labelDown;
}

//--------------------------------------------------------------
void ofxVirtualKeyboard::enableMouseEvents()
{
    ofAddListener(ofEvents().mousePressed, this, &ofxVirtualKeyboard::onMousePressed);
    ofAddListener(ofEvents().mouseDragged, this, &ofxVirtualKeyboard::onMouseDragged);
    ofAddListener(ofEvents().mouseReleased, this, &ofxVirtualKeyboard::onMouseReleased);
}

void ofxVirtualKeyboard::disableMouseEvents()
{
    ofRemoveListener(ofEvents().mousePressed, this, &ofxVirtualKeyboard::onMousePressed);
    ofRemoveListener(ofEvents().mouseDragged, this, &ofxVirtualKeyboard::onMouseDragged);
    ofRemoveListener(ofEvents().mouseReleased, this, &ofxVirtualKeyboard::onMouseReleased);
}

void ofxVirtualKeyboard::enableTouchEvents()
{
    ofAddListener(ofEvents().touchUp, this, &ofxVirtualKeyboard::onTouchUp);
    ofAddListener(ofEvents().touchDown, this, &ofxVirtualKeyboard::onTouchDown);
    ofAddListener(ofEvents().touchMoved, this, &ofxVirtualKeyboard::onTouchMoved);
}

void ofxVirtualKeyboard::disableTouchEvents()
{
    ofRemoveListener(ofEvents().touchUp, this, &ofxVirtualKeyboard::onTouchUp);
    ofRemoveListener(ofEvents().touchDown, this, &ofxVirtualKeyboard::onTouchDown);
    ofRemoveListener(ofEvents().touchMoved, this, &ofxVirtualKeyboard::onTouchMoved);
}

void ofxVirtualKeyboard::onMousePressed(ofMouseEventArgs & data)
{
	labelToReturn = checkForKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onMouseDragged(ofMouseEventArgs & data)
{
	labelToReturn = checkForKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onMouseReleased(ofMouseEventArgs & data)
{
	keyReleased();
}


void ofxVirtualKeyboard::onTouchDown(ofTouchEventArgs& data)
{
	labelToReturn = checkForKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onTouchMoved(ofTouchEventArgs& data)
{
	labelToReturn = checkForKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onTouchUp(ofTouchEventArgs& data)
{
	keyReleased();
}

//--------------------------------------------------------------
string ofxVirtualKeyboard::checkForKeyDown(int x, int y)
{
    // Correct input position
    int xPos = x - keyboardSettings.translation.x;
    int yPos = y - keyboardSettings.translation.y;
	string label = "";

    // Keys
    for (int i = 0; i < keys.size(); i++)
    {
		if (keys[i].button.inside(xPos, yPos))
		{
			keyDown = i;

			// Caps Lock
			if (keyDown == 23) isCapsLockActive = !isCapsLockActive;

			// Shift
			if (keyDown == 33) isShiftActive = true;

			if ((isCapsLockActive && keyDown != 23) || (isShiftActive && keyDown != 33)) label = ofToUpper(keys[keyDown].label);
			else label = keys[keyDown].label;
		}
    }
    
    // Enter
	if (enterKey.buttonHit.inside(xPos, yPos))
	{
		label = enterKey.label;
		enterKeyDown = true;
	}
    
	return label;
}

string ofxVirtualKeyboard::keyReleased()
{    
	ofNotifyEvent(keyRe, labelToReturn, this);

	// Reset
	if (keyDown != 33) isShiftActive = false;
	keyDown = -1;
	enterKeyDown = false;

    return labelToReturn;
}


void ofxVirtualKeyboard::draw()
{
    ofPushMatrix();
    ofTranslate(keyboardSettings.translation.x, keyboardSettings.translation.y);
    ofPushStyle();
    ofFill();
    
    for(int i = 0; i < keys.size(); i++)
    {
        if (i == keyDown || (i == 23 && isCapsLockActive) || (i == 33 && isShiftActive)) ofSetColor(buttonDownColor);
        
        else ofSetColor(buttonColor);
        ofDrawRectRounded(keys[i].button, keyboardSettings.padding * 0.75);
        
        if (i == keyDown || (i == 23 && isCapsLockActive) || (i == 33 && isShiftActive)) ofSetColor(labelDownColor);
        else ofSetColor(labelColor);
        if (i == 0 || i == 11 || i == 12 || i == 23 || i == 33 || i == 43 || i == 44) fontText.drawString(keys[i].label, keys[i].labelPostion.x, keys[i].labelPostion.y);
        else
        {
            if (isCapsLockActive || isShiftActive) font.drawString(ofToUpper(keys[i].label), keys[i].labelPostion.x, keys[i].labelPostion.y);
            else font.drawString(keys[i].label, keys[i].labelPostion.x, keys[i].labelPostion.y);
        }
    }
    
    // Enter Button
    if (enterKeyDown) enterKey.buttonDisplay.setFillColor(buttonDownColor);
    else enterKey.buttonDisplay.setFillColor(buttonColor);
    enterKey.buttonDisplay.draw();
    if (enterKeyDown) ofSetColor(labelDownColor);
    else ofSetColor(labelColor);
    fontText.drawString(enterKey.label, enterKey.labelPostion.x, enterKey.labelPostion.y);
    
    
    ofPopStyle();
    ofPopMatrix();
}

ofRectangle ofxVirtualKeyboard::getBoundingBox()
{
    return keyboardSettings.boundingBox;
}

void ofxVirtualKeyboard::createButtons(int _effectiveWidth, int _padding, int _keySize)
{
    vector <ofRectangle> tempButtons;
    
    // #1 Row
    for (int i = 0; i < 12; i++)
    {
        ofRectangle keyButton;
        keyButton.setPosition(i * (_keySize + _padding), 0);
        
        if (i == 11) keyButton.setSize(_keySize * 2, _keySize);
        else keyButton.setSize(_keySize, _keySize);
        
        tempButtons.push_back(keyButton);
    }
    
    // #2 Row
    for (int i = 0; i < 11; i++)
    {
        ofRectangle keyButton;
        
        if (i == 0)
        {
            keyButton.setPosition(0, _keySize + _padding);
            keyButton.setSize(_keySize + _keySize / 2, _keySize);
			tempButtons.push_back(keyButton);

        }
        else
        {
            keyButton.setPosition(_keySize / 2 + i * (_keySize + _padding), _keySize + _padding);
            keyButton.setSize(_keySize, _keySize);
			tempButtons.push_back(keyButton);

        }
        
        
        // Enter Button
        if (i == 10)
        {
            enterKey.buttonHit.addVertex(keyButton.x + _keySize + _padding, _keySize + _padding);
            enterKey.buttonHit.addVertex(_effectiveWidth, _keySize + _padding);
            enterKey.buttonHit.addVertex(_effectiveWidth, _keySize * 3 + _padding * 2);
            enterKey.buttonHit.addVertex(keyButton.x + _keySize / 2, _keySize * 3 + _padding * 2);
            enterKey.buttonHit.addVertex(keyButton.x + _keySize / 2, _keySize * 2 + _padding * 2);
            enterKey.buttonHit.addVertex(keyButton.x + _keySize + _padding, _keySize * 2 + _padding * 2);
            enterKey.buttonHit.close();
            
            int radius = _padding * 2;
            int aX = radius + keyButton.x + _keySize + _padding;
            int aY = _keySize + _padding;
            int bX = _effectiveWidth - radius;
            int bY = _keySize + _padding + radius;
            int cX = radius + keyButton.x + _keySize / 2;
            int cY = _keySize * 3 + _padding * 2 - radius;
            int dX = keyButton.x + _keySize / 2;
            int dY = _keySize * 3 + _padding * 2;
            int eY = _keySize * 2 + _padding * 2;
            int fX = keyButton.x + _keySize + _padding;
            
            enterKey.buttonDisplay.lineTo(aX, aY);
            enterKey.buttonDisplay.lineTo(bX, aY);
            enterKey.buttonDisplay.bezierTo(_effectiveWidth, aY, _effectiveWidth, aY, _effectiveWidth, bY);
            
            enterKey.buttonDisplay.lineTo(_effectiveWidth, cY);
            enterKey.buttonDisplay.bezierTo(_effectiveWidth, dY, _effectiveWidth, dY, bX, dY);
            
            enterKey.buttonDisplay.lineTo(cX, dY);
            enterKey.buttonDisplay.bezierTo(dX, dY, dX, dY, dX, cY);
            
            enterKey.buttonDisplay.lineTo(dX, eY + radius);
            enterKey.buttonDisplay.bezierTo(dX, eY, dX, eY, cX, eY);
            
            enterKey.buttonDisplay.lineTo(fX - radius, eY);
            enterKey.buttonDisplay.bezierTo(fX, eY, fX, eY, fX, eY - radius);
            
            enterKey.buttonDisplay.lineTo(fX, bY);
            enterKey.buttonDisplay.bezierTo(fX, aY, fX, aY, aX, aY);
            enterKey.buttonDisplay.close();
        }
    }
    
    // #3 Row
    for (int i = 0; i < 10; i++)
    {
        ofRectangle keyButton;
        
        if (i == 0)
        {
            keyButton.setPosition(0, _keySize * 2 + _padding * 2);
            keyButton.setSize(_keySize * 2, _keySize);
        }
        else
        {
            keyButton.setPosition(_keySize + i * (_keySize + _padding), _keySize * 2 + _padding * 2);
            keyButton.setSize(_keySize, _keySize);
        }
        
        tempButtons.push_back(keyButton);
    }
    // #4 Row
    for (int i = 0; i < 11; i++)
    {
        ofRectangle keyButton;
        
        if (i == 0)
        {
            keyButton.setPosition(0, _keySize * 3 + _padding * 3);
            keyButton.setSize(_keySize * 2 + _keySize / 2, _keySize);
        }
        else if (i == 10)
        {
            keyButton.setPosition((_keySize + _keySize / 2) + i * (_keySize + _padding), _keySize * 3 + _padding * 3);
            keyButton.setSize(_effectiveWidth - keyButton.x, _keySize);
        }
        else
        {
            keyButton.setPosition((_keySize + _keySize / 2) + i * (_keySize + _padding), _keySize * 3 + _padding * 3);
            keyButton.setSize(_keySize, _keySize);
        }
        
        tempButtons.push_back(keyButton);
    }
    

	for (int i = 0; i < keys.size(); i++)
	{
		keys[i].button = tempButtons[i];
	}
}

void ofxVirtualKeyboard::setLabelsPosition()
{
    int fontTextX = keyboardSettings.keySize / 3;
    int fontTextY = 0;
    ofRectangle fontRectangle;
    

    // Abcs # . _
    for (int i = 0; i < keys.size(); i++)
    {
		ofPoint fontPositionTemp;
        fontRectangle = font.getStringBoundingBox(ofToUpper(keys[i].label), keys[i].button.x, keys[i].button.y);
        fontPositionTemp.x = keys[i].button.x + keyboardSettings.keySize / 2 - fontRectangle.width / 2;
        fontPositionTemp.y = keys[i].button.y + keyboardSettings.keySize / 2 + fontRectangle.height / 2;

        keys[i].labelPostion = fontPositionTemp;
        
        if (i == 12) fontTextY = fontPositionTemp.y;
        
        
        if (i == 41) keys[i].labelPostion.y = keys[i].labelPostion.y + keyboardSettings.keySize / 4;
        if (i == 42) keys[i].labelPostion.y = keys[i].labelPostion.y + keyboardSettings.keySize / 4;
    }
    
    // Backspace
    fontRectangle = fontText.getStringBoundingBox(ofToUpper(keys[11].label), keys[11].button.x, keys[11].button.y);
    keys[11].labelPostion.x = keys[11].button.x + keys[11].button.width / 2 - fontRectangle.width;
    keys[11].labelPostion.y = fontTextY - keyboardSettings.keySize - keyboardSettings.padding / 2;
    
    // Tab
    keys[12].labelPostion.x = fontTextX;
    
    // Caps Lock
    keys[23].labelPostion.x = fontTextX;
    keys[23].labelPostion.y = fontTextY + keyboardSettings.keySize + keyboardSettings.padding * 1.8;
    
    // Shift
    keys[33].labelPostion.x = fontTextX;
    keys[33].labelPostion.y = fontTextY + keyboardSettings.keySize * 2 + keyboardSettings.padding * 3;
    
    // @
    fontRectangle = fontText.getStringBoundingBox(ofToUpper(keys[43].label), keys[43].button.x, keys[43].button.y);
    keys[43].labelPostion.x = keys[43].button.x + keys[43].button.width / 2 - fontRectangle.width / 2;
    keys[43].labelPostion.y = fontTextY + keyboardSettings.keySize * 2 + keyboardSettings.padding * 3;
    
    // Enter
	fontRectangle = font.getStringBoundingBox(ofToUpper(enterKey.label), enterKey.buttonHit[0].x, enterKey.buttonHit[0].y);
	enterKey.labelPostion.x = enterKey.buttonHit[0].x + keyboardSettings.keySize / 2 - fontRectangle.width / 2;
    enterKey.labelPostion.y = fontTextY + keyboardSettings.keySize + keyboardSettings.padding * 2;
}
