#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;

	settings.setGLVersion(3, 3);
	ofCreateWindow(settings);
	//ofSetupOpenGL(1024,768,OF_WINDOW);			

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
