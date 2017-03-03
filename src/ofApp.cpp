#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//Init graphics
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0,0,0);
	//ofEnableSmoothing();

	//Init soundstream and pitch tracking
	soundStream.setup(this, 0, NUM_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, NUM_BUFFERS);
	dywapitch_inittracking(&pitchtracker);

	//Init other variables
	curHeight = 0;
	isPassing = true;
	invincibilityCooldown = 0;
	score = 0;
	maxScore = 0;
	prevTimeStamp = ofGetElapsedTimeMillis();

	intro = new Trainer();
	phase = Training;
	level = NULL;
}

void ofApp::buildLevel(float centerFreq)
{
	if(level != NULL)
		return;
	//buildZeldaLevel();
	buildStarWarsLevel(centerFreq);

//	curBeatsPerScreen = 8.0f;
//	level = new Level(ofGetScreenWidth(), 75.0f, curBeatsPerScreen);
//	level->setCenterFreq(centerFreq);
//	level->setSemitoneRange(-7,12);
//
//	for(int i = 0; i < 200; i++)
//	{
//		level->addSpace(1);
//		level->addAsteroid(1,5, 20.0f);
//	}
}

void ofApp::buildZeldaLevel(float centerFreq)
{
	curBeatsPerScreen = 8.0f;
	level = new Level(ofGetScreenWidth(), 75.0f, curBeatsPerScreen);
	level->setCenterFreq(centerFreq);
	level->setSemitoneRange(0, 7);

	level->addSpace(4);
	level->addTunnel(0.75, 4);
	level->addSpace(0.75);
	level->addTunnel(0.5f, 7);
	level->addSpace(1);
	level->addTunnel(0.75, 2);
	level->addSpace(2.25);
	level->addTunnel(0.75, 4);
	level->addSpace(0.75);
	level->addTunnel(0.5f, 7);
	level->addSpace(1);
	level->addTunnel(0.75, 2);
	level->addSpace(2.25);
}

void ofApp::buildStarWarsLevel(float centerFreq)
{
	curBeatsPerScreen = 8.0f;
	level = new Level(ofGetScreenWidth(), 80.0f, curBeatsPerScreen);
	level->setCenterFreq(centerFreq);
	level->setSemitoneRange(-8, 14);

	//Starting ramp-up
	level->addSpace(4);

	//Easy asteroid dodges
	level->addAsteroid(0.5, 7, 10);
	level->addSpace(2);
	level->addAsteroid(0.5, 0, 10);
	level->addSpace(2);
	level->addAsteroid(0.5, 2, 10);
	level->addSpace(2);

	//Narrowing set of easy tunnels, interrupted with an asteroid or two
	level->addSpace(4);
	level->addTunnel(1, 4, 70);
	level->addSpace(1.5);
	level->addTunnel(1, 4, 50);
	level->addSpace(1.5);
	level->addAsteroid(1, 4, 15);
	level->addSpace(1.5);
	level->addTunnel(1, 4, 30);
	level->addSpace(1.5);
	level->addTunnel(1, 4);

	//Now a slightly wider/longer tunnel at a higher pitch, with an asteroid immediately after it
	level->addSpace(3);
	level->addTunnel(3, 9, 20);
	level->addSpace(0.5);
	level->addAsteroid(0.5, 9, 10);
	level->addSpace(2);

	//Star wars foreshadowing: first two notes
	level->addSpace(1);
	level->addTunnel(1, 0, 25);
	level->addSpace(1);
	level->addTunnel(1, 7, 25);
	level->addSpace(2);

	//introduce sloped tunnels
	level->addSlopedTunnel(4, 0, 4, 50);
	level->addSpace(3);
	level->addSlopedTunnel(4, 7, 4, 50);
	level->addSpace(2);

	//Filler asteroids
	level->addAsteroid(1, 10, 16);
	level->addSpace(1);
	level->addAsteroid(1, -4, 16);
	level->addSpace(3);

	//Star wars foreshadowing 2
	level->addSpace(1);
	level->addTunnel(1, 0, 30);
	level->addSpace(1);
	level->addTunnel(1, 7, 30);
	level->addSlopedTunnel(1, 7, 2, 30);
	level->addSpace(4);

	//Really long tunnel
	level->addSlopedTunnel(6, 0, 8, 23);
	level->addSpace(4);

	//star wars foreshadowing 3 (end of motif 1) (with some pitch id help)
	level->addTunnel(0.5, 6);
	level->addSpace(1);
	level->addTunnel(0.5, 4);
	level->addSpace(1);
	level->addTunnel(0.5, 6);
	level->addSpace(1.5);
	level->addSlopedTunnel(1, 6, 4);
	level->addSlopedTunnel(1, 4, 6);
	level->addSpace(2);
	level->addTunnel(1, 2);
	level->addSpace(2);

	//Couple of easy asteroids
	level->addAsteroid(1, 4, 15);
	level->addSpace(2);
	level->addAsteroid(1, -2, 15);
	level->addSpace(2);
	level->addAsteroid(1, 9, 15);
	level->addSpace(4);

	//Proper Star Wars theme starts here - motif 1
	level->addSpace(1);
	level->addTunnel(1, 0);
	level->addSpace(1);
	level->addTunnel(1, 7);
	level->addSlopedTunnel(1, 7, 2);
	level->addSpace(1);
	level->addTunnel(1, 12);
	level->addSpace(0.75);
	level->addTunnel(0.25, 7);
	level->addSlopedTunnel(1, 7, 2);
	level->addSpace(1);
	level->addTunnel(1, 12);
	level->addSpace(0.75);
	level->addTunnel(0.25, 7);
	level->addSlopedTunnel(0.5, 7, 4);
	level->addSlopedTunnel(0.5, 4, 6);
	level->addSpace(1);
	level->addTunnel(1, 2);
	level->addSpace(2);

	//Repeat motif 1
	level->addSpace(1);
	level->addTunnel(1, 0);
	level->addSpace(1);
	level->addTunnel(1, 7);
	level->addSlopedTunnel(1, 7, 2);
	level->addSpace(1);
	level->addTunnel(1, 12);
	level->addSpace(0.75);
	level->addTunnel(0.25, 7);
	level->addSlopedTunnel(1, 7, 2);
	level->addSpace(1);
	level->addTunnel(1, 12);
	level->addSpace(0.75);
	level->addTunnel(0.25, 7);
	level->addSlopedTunnel(0.5, 7, 4);
	level->addSlopedTunnel(0.5, 4, 6);
	level->addSpace(1);
	level->addTunnel(1, 2);
	level->addSpace(2);

}

//--------------------------------------------------------------
void ofApp::update(){
	float dt = ofGetElapsedTimeMillis() - prevTimeStamp;
	prevTimeStamp = ofGetElapsedTimeMillis();
	if(phase == Intro || phase == Gameplay)
	{
		//Update height of ship
		double minPitch = level->getMinFreq();
		double maxPitch = level->getMaxFreq();
		float targetHeight = pitchToHeight(curPitch);
		if(curPitch >= minPitch && curPitch <= maxPitch)
		{
			curHeight = curHeight + CONTROL_SMOOTHING_FACTOR * (targetHeight - curHeight);
		}
	}
	if(phase == Gameplay)
	{
		//Update level
		level->advanceTime(dt);

		//If the current obstacle in the level is score-able, update the max score
		bool isCurrentlyScoreable = false;
		if(!level->isDone())
		{
			isCurrentlyScoreable = level->getCurrentObstacle()->isScoreable();
			if(isCurrentlyScoreable)
				maxScore += dt;
		}


		if(invincibilityCooldown <= 0)
		{
			//Check if passing current obstacle
			float curActualFreq = heightToPitch(curHeight);
			float dFreq = heightToPitch(SHIP_HEIGHT / 2) - level->getMinFreq();
			float timeDiff = (SHIP_WIDTH / 2) / level->getSpeed();
			isPassing = level->pass(curActualFreq, dFreq, timeDiff);

			if(!isPassing)
				invincibilityCooldown = INVINCIBILITY_COOLDOWN;
			if(isPassing && isCurrentlyScoreable)
				score += dt;
		}
        else
		{
            invincibilityCooldown -= dt;
            if(invincibilityCooldown < 0)
				invincibilityCooldown = 0;
		}

		//If level is complete, move onto next phase
		if(level->isDone())
			phase = LevelEnd;
	}
	if(phase == Training || phase == Intro || (phase == Gameplay && !intro->isDone()))
	{
		//Update training object: pass it different values depending on whether or not pitch training is complete
		if(intro->isTrained())
		{
			//Pass the height of the ship as a pitch if we have it, otherwise pass the straight detected pitch
			if(curHeight == 0)
				intro->update(curPitch);
			else
				intro->update(heightToPitch(curHeight));
			phase = Intro;
			buildLevel(intro->getCenterFreq());
			intro->setLaunchTargetFreq(level->getDoFreq());
		}
		else
		{
			intro->update(curPitch);
		}
		if(intro->isLevelStarted())
			phase = Gameplay;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushStyle();
		ofPushMatrix();
			ofTranslate(LEFT_BUFFER, 0, 0);

			if(phase != Training)
			{
				level->draw(LEFT_BUFFER);
				drawShip();
			}
			if(phase == Intro)
			{
				drawTarget();
			}
			if(phase == LevelEnd)
			{
                drawEndScreen();
			}

			ofSetColor(0,255,0);
		ofPopMatrix();
	ofPopStyle();

	if(phase == Training || phase == Intro || (phase == Gameplay && !intro->isDone()))
		intro->draw();
}

void ofApp::drawShip()
{
	int windowHeight = ofGetWindowHeight();

	ofFill();
	if(invincibilityCooldown <= 0)
		ofSetColor(255, 255, 255);
	else
		ofSetColor(255, 0, 0);

	if(invincibilityCooldown % (FLICKER_TIME * 2) < FLICKER_TIME)
	{
		ofPushMatrix();
			ofTranslate(0, windowHeight - curHeight, 0);
			ofTriangle(-SHIP_WIDTH/2, SHIP_HEIGHT/2, SHIP_WIDTH/2, 0, -SHIP_WIDTH/2 + SHIP_TAIL_LENGTH, 0);
			ofTriangle(-SHIP_WIDTH/2, -SHIP_HEIGHT/2, SHIP_WIDTH/2, 0, -SHIP_WIDTH/2 + SHIP_TAIL_LENGTH, 0);
		ofPopMatrix();
	}

}

void ofApp::drawTarget()
{
	int windowHeight = ofGetWindowHeight();
	float targetHeight = pitchToHeight(level->getDoFreq());
	ofNoFill();
	ofCircle(0, windowHeight - targetHeight, SHIP_WIDTH / 2 + 10);
}

void ofApp::drawEndScreen()
{
	ofTrueTypeFont* font = intro->getFont();
	if(font->isLoaded())
	{
		string message_one = "Level Complete!";
		string message_two = "Your Score: " + ofToString(100 * (score / maxScore), 0) + "%";
		string message_three = "(Press R to Restart)";
		float stringHeight_one = font->stringHeight(message_one);
		float stringWidth_one = font->stringWidth(message_one);
		float stringHeight_two = font->stringHeight(message_two);
		float stringWidth_two = font->stringWidth(message_two);
		float stringWidth_three = font->stringWidth(message_three);
		float halfScreenHeight = ofGetWindowHeight() / 2;
		float halfScreenWidth = ofGetWindowWidth() / 2;

		font->drawString(message_one, halfScreenWidth - stringWidth_one/2, halfScreenHeight - stringHeight_two/2 - stringHeight_one - 5);
		font->drawString(message_two, halfScreenWidth - stringWidth_two/2, halfScreenHeight - stringHeight_two/2);
		font->drawString(message_three, halfScreenWidth - stringWidth_three/2, halfScreenHeight + stringHeight_two/2 + 10);
	}
}

float ofApp::pitchToHeight(float pitch)
{
	return ((float)ofGetWindowHeight()) * ((pitch - level->getMinFreq()) / (level->getMaxFreq() - level->getMinFreq()));
}

float ofApp::heightToPitch(float height)
{
    return (height / (float)ofGetWindowHeight()) * (level->getMaxFreq() - level->getMinFreq()) + level->getMinFreq();
}

void ofApp::audioIn(float* input, int bufferSize, int numChannels)
{
	double audioLeft[bufferSize];
	double audioRight[bufferSize];

	//Read audio data into buffers
	for(int i = 0; i < bufferSize; i++)
	{
		audioLeft[i] = (double) input[i * 2];
		audioRight[i] = (double) input[i * 2 + 1];
	}

	//Pitch detect
	curPitch = dywapitch_computepitch(&pitchtracker, audioLeft, 0, bufferSize);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(phase == LevelEnd && key == 114) //"R" key
	{
        //Restart level
		intro->restart();
		level->restart();
		phase = Intro;
		score = 0;
		maxScore = 0;
		invincibilityCooldown = 0;
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
	//level->setSpeed(w, level->getTempo(), curBeatsPerScreen);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
