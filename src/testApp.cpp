#include "testApp.h"

//#define SNOW_COLOR 0x77F5F9
#define SNOW_COLOR 0xffffff
#define BACKGROUND_COLOR 0x000000
#define DEBUG_COLOR 0xff0000

ofImage* Snowflake::img = NULL;

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(BACKGROUND_COLOR);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	bMouseForce = false;
	bDebugMode = false;
	
	bOrnimentMode = false;
	
	box2d.init();
	box2d.setGravity(0, .5);
//	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	cursor.setup(box2d.getWorld(), mouseX, mouseY, 20);
	cursor.fixture.filter.maskBits = 0x0000;

}


void testApp::addEdge(ofPolyline* drawing) {
	ofxBox2dEdge* e = new ofxBox2dEdge;
	e->addVertexes(*drawing);
	e->setPhysics(0.0, 0.01, 1.0);
	e->create(box2d.getWorld());
	edges.push_back(ofPtr<ofxBox2dEdge>(e));
}

//--------------------------------------------------------------
void testApp::update() {
	
	createSnowflake();
	createSnowflake();
	
	cursor.setPosition(mouseX, mouseY);

	for(int i=0; i<orniments.size(); ++i)
	{
		Orniment* o = orniments[i].get();
		o->update();
	}
	
	for(int i=0; i<snowflakes.size(); ++i)
	{
		Snowflake* flake = snowflakes[i].get();
		flake->update();
	}
	
	
	while(snowflakes.size() > 1500) {
		Snowflake* c = snowflakes[snowflakes.size()-1].get();
		c->destroy();
		snowflakes.erase(snowflakes.begin());
	}
	box2d.update();
	
    // remove shapes offscreen
    ofRemove(snowflakes, ofxBox2dBaseShape::shouldRemoveOffScreen);
}


//--------------------------------------------------------------
void testApp::draw() {
	
	for(int i=0; i<snowflakes.size(); i++) {
		snowflakes[i].get()->draw();
	}
	
	ofNoFill();
	ofSetHexColor(bDebugMode?DEBUG_COLOR:BACKGROUND_COLOR);
	
	for(int i=0; i<edges.size(); ++i) {
		edges[i].get()->updateShape();
		edges[i].get()->draw();
	}

	for(int i=0; i<orniments.size(); ++i) {
//		orniments[i].get()->updateShape();
		orniments[i].get()->draw();
	}
	
	
	if(drawing.size()==0) {
        edgeLine.updateShape();
        edgeLine.draw();
    }
	else {
		drawing.draw();
	}
	
	if(bDebugMode || bOrnimentMode) {
		ofFill();
		cursor.draw();
	}
	
	if(bDebugMode) {
		string info = "";
		info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
		info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
		info += "Total Edges: "+ofToString(edges.size())+"\n\n";
		info += "Total Orniments: "+ofToString(orniments.size())+"\n\n";
		info += "mouse : "+ofToString(mouseX) + " " + ofToString(mouseY)+"\n\n";
		info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
		ofSetHexColor(0x444342);
		ofDrawBitmapString(info, 30, 30);
	}
	
}


void testApp::createOrniment() {
	float r = ofRandom(20, 30);
	Orniment* circle = new Orniment;
	orniments.push_back(ofPtr<Orniment>(circle));
	circle->color.r = ofRandom(255);
	circle->color.g = ofRandom(255);
	circle->color.b = ofRandom(255);
//	circle->setPhysics(0.0001, 0.03, 1.0);
	float x = ofRandom(ofGetWindowWidth());
	
	circle->setup(box2d.getWorld(), mouseX, mouseY, r);
}


void testApp::createSnowflake() {
	float r = ofRandom(2, 4);		// a random radius 4px - 20px
	Snowflake* flake = new Snowflake;
	snowflakes.push_back(ofPtr<Snowflake>(flake));
	flake->setPhysics(0.0001, 0.03, 1.0);
	float x = ofRandom(ofGetWindowWidth());
	
	if(!(rand()%2)) {
		flake->fixture.filter.maskBits = 0x0000;
	} else {
		flake->fixture.filter.maskBits = 0xffff;
	}
	
	flake->setup(box2d.getWorld(), x, 0.0f, r);
}
//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if(key == 'd') {
		bDebugMode = !bDebugMode;
	}
//
//	if(key == 'b') {
//		float w = ofRandom(4, 20);	
//		float h = ofRandom(4, 20);	
//		boxes.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
//		boxes.back().get()->setPhysics(3.0, 0.53, 0.1);
//		boxes.back().get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
//	}
//	
//	if(key == 'z') {
//        
//		customParticles.push_back(ofPtr<CustomParticle>(new CustomParticle));
//        CustomParticle * p = customParticles.back().get();
//		float r = ofRandom(3, 10);		// a random radius 4px - 20px
//		p->setPhysics(0.4, 0.53, 0.31);
//		p->setup(box2d.getWorld(), mouseX, mouseY, r);
//		p->color.r = ofRandom(20, 100);
//		p->color.g = 0;
//		p->color.b = ofRandom(150, 255);
//	}	
	
	if(key == 'c') {
		for(int i=0; i<edges.size(); ++i) {
			edges[i].get()->destroy();
		}
		edges.clear();
		
		for(int i=0; i<orniments.size(); ++i) {
			orniments[i].get()->destroy();
		}
		orniments.clear();
	}
	
	if(key == 't') {
		ofToggleFullscreen();
	}
	
	if(key == 'o') {
		bOrnimentMode = !bOrnimentMode;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	
	if(bDebugMode) {
		drawing.addVertex(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	if(bDebugMode) {
		if(edgeLine.isBody()) {
			drawing.clear();
			edgeLine.destroy();
		}
		drawing.addVertex(x, y);
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
	if(bDebugMode) {

		drawing.setClosed(false);
		drawing.simplify();
		
		
		addEdge(&drawing);
		edgeLine.destroy();
			
		drawing.clear();
	}
	
	if(bOrnimentMode) {
		createOrniment();
	}

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

