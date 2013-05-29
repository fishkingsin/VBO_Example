/**
 *  testApp.cpp
 *
 *  Created by Marek Bereza on 09/01/2012.
 */

#include "testApp.h"
int nrOfCircles = 1;
float rotation = 0;
float rotationSteps = 0.01;
float lastSwitch;
bool closing;
bool opening;

void testApp::setup() {
	ofBackground(0, 0, 0, 255);
	ofSetFrameRate(60);
	createNewArcs();
	closing = false;
	opening = true;
	ofSetVerticalSync(false);
	lastSwitch = ofGetElapsedTimef();
	rotationSteps= 0.4;
	snapCounter =0;
	bSnapshot = false;
	scale=0;
    currentMinute = ofGetMinutes();
    font.loadFont("TRON.TTF", 24);
//    fbo.allocate(ofGetWidth(), ofGetHeight());
//    fbo.begin();
//    ofClear(0, 0, 0, 0);
//	fbo.end();
//    shader.load("bloom");
}

void testApp::update() {
    
    if(ofGetMinutes()!=currentMinute)
    {
        closing =true;
        opening = false;
        lastSwitch = ofGetElapsedTimef();
        currentMinute = ofGetMinutes();
    }
    
	rotation += rotationSteps;
	
	if(ofGetElapsedTimef() - lastSwitch > 9 && !closing && !opening)
	{
		closing =true;
		opening = false;
	}
    
	if (closing) {
		scale -=(scale) / 8;
		
		if(scale < 0.1)
		{
			createNewArcs();
			opening = true;
			closing = false;
		}
	}
	
	if (opening) {
		scale+=(1-scale) / 8;
		if(scale > 0.99)
		{
			scale = 1;
			lastSwitch = ofGetElapsedTimef();
			opening = false;
		}
	}
}

void testApp::draw() {
    //	if (ofGetElapsedTimef() < 10) {
    //		ofSetColor(40, 100, 100);
    //
    //		char fpsStr[255]; // an array of chars
    //
    //		sprintf(fpsStr, "press [f] to \ntoggle fullscreen\nmove mouse horizontaly\nfor speed");
    //		ofDrawBitmapString(fpsStr, ofGetWidth()/2 - 82,ofGetHeight()/2 - 30);
    //	}
	
	
	// uncomment to see the frame - currently framerate limited to 60fps
	/*
     char fpsStr[255]; // an array of chars
     sprintf(fpsStr, "frame rate: %f", ofGetFrameRate());
     ofDrawBitmapString(fpsStr, 10,10);
     */
    
    ofEnableAlphaBlending();
//	fbo.begin();
//	ofClear(0, 0, 0, 0);
    
	glPushMatrix();
	glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
    
	ofScale(scale, scale, 1);
    
	for (int i=0; i < nrOfCircles; i++)
	{
		arc* arcObject = arcs.at(i);
		arcObject->draw(rotation);
	}
    glPopMatrix();
    
    char c [256];
    sprintf(c, "%02i:%02i:%02i",ofGetHours(), currentMinute, ofGetSeconds());
    ofSetColor(255);
    ofRectangle rect = font.getStringBoundingBox(c, 0,0);
    glPushMatrix();
    glTranslated(ofGetWidth()*0.5, ofGetHeight()*0.5, 0);
    glScalef(scale, scale, scale);
    font.drawString(c,-rect.width*0.5,rect.height*0.5);
    glPopMatrix();
    
//    fbo.end();
//    shader.begin();
//    shader.setUniform1f("u_time", ofGetElapsedTimef() );
//    fbo.draw(0, 0);
//    shader.end();
}
void testApp::createNewArcs()
{
	for (int i=0; i < arcs.size(); i++) {
		// not sure if cleans out everything...
		delete arcs[i];
	}
	arcs.clear();
	
	nrOfCircles = 35 + ofRandom(70);
	
	float innerRadius = 100;
	float angle=0;
	float cirleAngle =0;
	bool clockWise = true;
	float lineWidth = 10;
	float margin = 0;
	float circleStartAngle = 0;
	
	for (int i=0; i < nrOfCircles; i++) {
		
		arc* arcObject = new arc();
		arcObject->innerRadius = innerRadius;
		arcObject->outerRadius = innerRadius + lineWidth;
        
		angle += ofRandom(0,10) > 2 ? 0.1 : 0;
		arcObject->startAngle = angle;
        
		angle += ofRandom(0.2,4.5);
		arcObject->endAngle = angle;
		
		arcObject->clockwise = clockWise;
		
		// using negative random values to avoid to much coloring
		arcObject->colorBlueStartFactor = ofClamp(ofRandom(-0.4,1.0), 0, 1);
		arcObject->colorBlueEndFactor = ofClamp(ofRandom(-0.4,1.0), 0, 1);
		arcObject->colorGreenCorrection = ofClamp(ofRandom(-0.4,0.1), 0, 1);
        
        
		if (angle >= ( TWO_PI + circleStartAngle  )) {
			innerRadius += (margin + lineWidth) ;
			
			// do correction on last arc
			arcObject->endAngle = TWO_PI + circleStartAngle ;
			
			angle= (angle - ( TWO_PI));
			circleStartAngle = angle;
			clockWise = ofRandom(10) > 5;
            
			int lineWidthRandom = ofRandom(20);
			if (lineWidthRandom < 3) {
				lineWidth = 30;
			}else if (lineWidth <10) {
				lineWidth = 20;
			}else {
				lineWidth =10;
			}
            
			//lineWidth = ofRandom(20) > 5 ? 10 : 20;
            
			int marginRandom = ofRandom(0,20);
            
			if(marginRandom > 6){
				margin =0;
			}
			else if(marginRandom > 2){
				margin =10;
			}else
			{
				margin =30;
			}
		}
        
		//cout << arcObject->startAngle << "  " << arcObject->endAngle << "   angle " << angle << "\n" ;
        
		arcObject->createMesh();
		arcs.push_back(arcObject);
        
	}
}