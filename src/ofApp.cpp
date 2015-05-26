
#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

    camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
    vidPlayer.loadMovie("fingers.mov");
    vidPlayer.play();
 
    colorImg.allocate(camWidth,camHeight);
	grayImage.allocate(camWidth,camHeight);
	grayBg.allocate(camWidth,camHeight);
	grayDiff.allocate(camWidth,camHeight);

	bLearnBakground = true;
	threshold = 80;

	// Box 2D
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(60.0);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);
	box2d.update();

	ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);











    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 50, (camWidth*camHeight), 10, false, false);	// find holes
	}





}

//--------------------------------------------------------------
void ofApp::draw(){



	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	/*
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);
    */

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(20,20,camWidth*2,camHeight*2);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(20,20);

    ofSetHexColor(0xff0000);

    edgeLine.destroy();
    for (unsigned int i = 0; i < contourFinder.blobs.size(); i++){
        drawing.addVertices(contourFinder.blobs[i].pts);
        drawing.setClosed(true);
        edgeLine.addVertexes(drawing);
        drawing.clear();
    }

    edgeLine.setPhysics(0.0, 0.5, 0.5);
    edgeLine.create(box2d.getWorld());

    //edgeLine.updateShape();
    edgeLine.draw();


    for(unsigned int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles[i].get()->draw();
	}
    /*
	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << threshold << " (press: +/-)" << endl
			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);
    */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}

    if(key == 'c') {
        cout << "creating circle" << endl;
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
		circles.back().get()->setPhysics(3.0, 0.53, 0.1);
		circles.back().get()->setup(box2d.getWorld(), mouseX, mouseY, r);

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
