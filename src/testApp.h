#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class Orniment : public ofxBox2dCircle {
public:
	Orniment(){
		scale = 1.0;
		step = 0.01;
	}
	
	float scale;
	float step;
	ofColor color;
	void update() {
		if(scale >=1 || scale<=0) {
			step*=-1;
		}
		
		scale+=step;
	}
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r*scale, color.g*scale, color.b*scale);
		ofFill();
		ofCircle(0, 0, radius);
		
		glPopMatrix();
		
	}
};


class Snowflake : public ofxBox2dCircle {
public:
	Snowflake(){
		scale = 1.0;
		step = 0.230;
		color = 0xffffff;
//		if(!img) {
//			img = new ofImage;
//			img->loadImage("01.png");
//			img->resize(24,24);
//		}
	}
	
	float scale;
	float step;
	ofColor color;
	static ofImage* img;

	void update() {
		if(scale >=1 || scale<=0.8) {
			step*=-1;
		}
		
		scale+=step;
		
		if(!isSleeping()) {

			if(!(rand()%2)) {
				int dx = 1;
				if(rand()%2) {
					dx*=-1;
				}

				int dy = 1;
				if(rand()%2) {
					dy*=-1;
				}

				ofVec2f vec(ofRandom(.001)*dx, ofRandom(.001)*dy);
				addForce(vec, .003);
			}
		}
		
	}

	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
//		img->draw(getPosition().x, getPosition().y);

		ofSetColor(color.r*scale, color.g*scale, color.b*scale);
		ofFill();
		ofCircle(0, 0, getRadius());
		
		glPopMatrix();
		
	}
};

// -------------------------------------------------
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	void addEdge(ofPolyline* drawing);
	
	void createSnowflake();
	void createOrniment();
	ofxBox2dCircle cursor;
	
	
	float                                   px, py;
	bool                                    bDrawLines;
	bool                                    bMouseForce;
	bool									bDebugMode;
	bool									bOrnimentMode;
	
	
	ofxBox2d                                box2d;			  //	the box2d world
	ofPolyline                              drawing;		  //	we draw with this first
	ofxBox2dEdge                            edgeLine;		  //	the box2d edge/line shape (min 2 points)
	vector		<ofPtr<ofxBox2dEdge> >		edges;		  //	default box2d circles
	vector		<ofPtr<Snowflake> >	snowflakes;		  //	default box2d circles

	vector		<ofPtr<Orniment> >	orniments;		  //	default box2d circles
	
	
	
};
