#include "ofxVirtualKeyboard.h"

ofxVirtualKeyboard::ofxVirtualKeyboard(){}

ofxVirtualKeyboard::~ofxVirtualKeyboard()
{
    if (usingMouseEvents) disableMouseEvents();
    if (usingTouchEvents) disableTouchEvents();
}

//--------------------------------------------------------------
void ofxVirtualKeyboard::setup(int width, int padding, string labelFontPath, float labelSize)
{
    setShape(width, padding);
    setFont(labelFontPath, labelSize);
}

void ofxVirtualKeyboard::setShape(int width, int padding)
{
    this->padding = padding;
    
    keySize = (width - padding * 11) / 13;
    effectiveWidth = keySize * 13 + padding * 11;
    float effectiveHeight = keySize * 4 + padding * 3;
    
    boundingBox = ofRectangle(0, 0, effectiveWidth, effectiveHeight);
    
    buttons = createButtons(effectiveWidth, padding, keySize);
}

void ofxVirtualKeyboard::setPosition(ofVec2f position)
{
    translation = position;
}

void ofxVirtualKeyboard::setFont(string path, float labelSize)
{
    font.load(path, keySize / 3 * labelSize);
    fontText.load(path, keySize / 4 * labelSize);
    createLabels();

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
    isKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onMouseDragged(ofMouseEventArgs & data)
{
    isKeyDown(data.x, data.y);
}

void ofxVirtualKeyboard::onMouseReleased(ofMouseEventArgs & data)
{
    cout << "onMouseReleased\n";
}

void ofxVirtualKeyboard::onTouchUp(ofTouchEventArgs& data)
{
    cout << "onTouchUp\n";
}

void ofxVirtualKeyboard::onTouchDown(ofTouchEventArgs& data)
{
    cout << "onTouchDown\n";
}

void ofxVirtualKeyboard::onTouchMoved(ofTouchEventArgs& data)
{
    cout << "onTouchMoved\n";
}

//--------------------------------------------------------------
bool ofxVirtualKeyboard::isKeyDown(int x, int y)
{
    // Correct input position
    int xPos = x - translation.x;
    int yPos = y - translation.y;
    
    // Keys
    for (int i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].inside(xPos, yPos)) keyDown = i;
    }
    
    // Caps Lock
    if (keyDown == 23) isCapsLockActive = !isCapsLockActive;
    
    // Shift
    if (keyDown == 33) isShiftActive = true;
    
    // Enter
    if (enterKeyHitter.inside(xPos, yPos)) enterKeyDown = true;
    
    
    if (keyDown == -1 && enterKeyDown == false) return false;
    else return true;
}

string ofxVirtualKeyboard::keyReleased()
{
    string labelToReturn = "";
    
    // If it's a key
    if (keyDown != -1)
    {
        if (isCapsLockActive) labelToReturn = ofToUpper(keysLabel[keyDown]);
        else if (isShiftActive && keyDown != 33)
        {
            labelToReturn = ofToUpper(keysLabel[keyDown]);
            isShiftActive = false;
        }
        else labelToReturn = keysLabel[keyDown];
    }
    
    // If it's enter
    if (enterKeyDown)
    {
        labelToReturn = keysLabel[44];
    }
    
    // Reset Values
    keyDown = -1;
    enterKeyDown = false;
    
    return labelToReturn;
}


void ofxVirtualKeyboard::draw()
{
    ofPushMatrix();
    ofTranslate(translation.x, translation.y);
    ofPushStyle();
    ofFill();
    
    for(int i = 0; i < buttons.size(); i++)
    {
        if (i == keyDown || (i == 23 && isCapsLockActive) || (i == 33 && isShiftActive)) ofSetColor(buttonDownColor);
        
        else ofSetColor(buttonColor);
        ofDrawRectRounded(buttons[i], padding * 0.75);
        
        if (i == keyDown || (i == 23 && isCapsLockActive) || (i == 33 && isShiftActive)) ofSetColor(labelDownColor);
        else ofSetColor(labelColor);
        if (i == 0 || i == 11 || i == 12 || i == 23 || i == 33 || i == 43 || i == 44) fontText.drawString(keysLabel[i], fontPosition[i].x, fontPosition[i].y);
        else
        {
            if (isCapsLockActive || isShiftActive) font.drawString(ofToUpper(keysLabel[i]), fontPosition[i].x, fontPosition[i].y);
            else font.drawString(keysLabel[i], fontPosition[i].x, fontPosition[i].y);
        }
    }
    
    // Enter Button
    if (enterKeyDown) enterKeyDisplay.setFillColor(buttonDownColor);
    else enterKeyDisplay.setFillColor(buttonColor);
    enterKeyDisplay.draw();
    if (enterKeyDown) ofSetColor(labelDownColor);
    else ofSetColor(labelColor);
    fontText.drawString(keysLabel[44], fontPosition[44].x, fontPosition[44].y);
    
    
    ofPopStyle();
    ofPopMatrix();
}

ofRectangle ofxVirtualKeyboard::getBoundingBox()
{
    return boundingBox;
}

vector <ofRectangle> ofxVirtualKeyboard::createButtons(int _effectiveWidth, int _padding, int _keySize)
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
        }
        else
        {
            keyButton.setPosition(_keySize / 2 + i * (_keySize + _padding), _keySize + _padding);
            keyButton.setSize(_keySize, _keySize);
        }
        
        tempButtons.push_back(keyButton);
        
        // Enter Button
        if (i == 10)
        {
            enterKeyHitter.addVertex(keyButton.x + _keySize + _padding, _keySize + _padding);
            enterKeyHitter.addVertex(_effectiveWidth, _keySize + _padding);
            enterKeyHitter.addVertex(_effectiveWidth, _keySize * 3 + _padding * 2);
            enterKeyHitter.addVertex(keyButton.x + _keySize / 2, _keySize * 3 + _padding * 2);
            enterKeyHitter.addVertex(keyButton.x + _keySize / 2, _keySize * 2 + _padding * 2);
            enterKeyHitter.addVertex(keyButton.x + _keySize + _padding, _keySize * 2 + _padding * 2);
            enterKeyHitter.close();
            
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
            
            enterKeyDisplay.lineTo(aX, aY);
            enterKeyDisplay.lineTo(bX, aY);
            enterKeyDisplay.bezierTo(_effectiveWidth, aY, _effectiveWidth, aY, _effectiveWidth, bY);
            
            enterKeyDisplay.lineTo(_effectiveWidth, cY);
            enterKeyDisplay.bezierTo(_effectiveWidth, dY, _effectiveWidth, dY, bX, dY);
            
            enterKeyDisplay.lineTo(cX, dY);
            enterKeyDisplay.bezierTo(dX, dY, dX, dY, dX, cY);
            
            enterKeyDisplay.lineTo(dX, eY + radius);
            enterKeyDisplay.bezierTo(dX, eY, dX, eY, cX, eY);
            
            enterKeyDisplay.lineTo(fX - radius, eY);
            enterKeyDisplay.bezierTo(fX, eY, fX, eY, fX, eY - radius);
            
            enterKeyDisplay.lineTo(fX, bY);
            enterKeyDisplay.bezierTo(fX, aY, fX, aY, aX, aY);
            enterKeyDisplay.close();
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
    
    return tempButtons;
    
}

void ofxVirtualKeyboard::createLabels()
{
    int fontTextX = keySize / 3;
    int fontTextY = 0;
    ofRectangle fontRectangle;
    ofPoint fontPositionTemp;
    
    fontPosition.clear();
    
    // Abcs # . _
    for(int i = 0; i < buttons.size(); i++)
    {
        fontRectangle = font.getStringBoundingBox(ofToUpper(keysLabel[i]), buttons[i].x, buttons[i].y);
        fontPositionTemp.x = buttons[i].x + keySize / 2 - fontRectangle.width / 2;
        fontPositionTemp.y = buttons[i].y + keySize / 2 + fontRectangle.height / 2;
        fontPosition.push_back(fontPositionTemp);
        
        if (i == 12) fontTextY = fontPositionTemp.y;
        
        
        if (i == 41) fontPosition[i].y = fontPosition[i].y + keySize / 4 ;
        if (i == 42) fontPosition[i].y = fontPosition[i].y + keySize / 4;
    }
    
    // Backspace
    fontRectangle = fontText.getStringBoundingBox(ofToUpper(keysLabel[11]), buttons[11].x, buttons[11].y);
    fontPosition[11].x = buttons[11].x + buttons[11].width / 2 - fontRectangle.width;
    fontPosition[11].y = fontTextY - keySize - padding / 2;
    
    // Tab
    fontPosition[12].x = fontTextX;
    
    // Caps Lock
    fontPosition[23].x = fontTextX;
    fontPosition[23].y = fontTextY + keySize + padding * 1.8;
    
    // Shift
    fontPosition[33].x = fontTextX;
    fontPosition[33].y = fontTextY + keySize * 2 + padding * 3;
    
    // Hold To Cancel
    // @
    fontRectangle = fontText.getStringBoundingBox(ofToUpper(keysLabel[43]), buttons[43].x, buttons[43].y);
    fontPosition[43].x = buttons[43].x + buttons[43].width / 2 - fontRectangle.width / 2;
    fontPosition[43].y = fontTextY + keySize * 2 + padding * 3;
    
    // Enter
    fontPosition[44].x = fontPosition[11].x - keySize / 1.75;
    fontPosition[44].y = fontTextY + keySize + padding * 2;
}
