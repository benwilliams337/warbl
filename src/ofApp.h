#pragma once

#include <sstream>
#include "ofMain.h"
#include "dywapitchtrack.h"
#include "Level.h"
#include "Trainer.h"

#define BUFFER_SIZE 		1024
#define NUM_CHANNELS		2
#define SAMPLE_RATE			44100
#define NUM_BUFFERS			2

#define SHIP_WIDTH			70.0f
#define SHIP_HEIGHT			30.0f
#define SHIP_TAIL_LENGTH	20.0f
#define LEFT_BUFFER			50.0f

#define INVINCIBILITY_COOLDOWN		1000
#define FLICKER_TIME				10
#define CONTROL_SMOOTHING_FACTOR	0.15f

class ofApp : public ofBaseApp{
	private:
		float prevTimeStamp;

		ofSoundStream soundStream;
		dywapitchtracker pitchtracker;

		enum Phase { Training, Intro, Gameplay, LevelEnd };
		Phase phase;

		Trainer* intro;
		Level* level;
		float curBeatsPerScreen;

		void buildLevel(float centerFreq);
		void buildZeldaLevel(float centerFreq);
		void buildStarWarsLevel(float centerFreq);

		double curPitch;
		float curHeight;
		bool isPassing;
		int invincibilityCooldown;

		float score;
		float maxScore;

		void drawShip();
		void drawTarget();
		void drawEndScreen();

		float pitchToHeight(float pitch);
		float heightToPitch(float height);
	public:
		void setup();
		void update();
		void draw();

		void audioIn(float* input, int bufferSize, int numChannels);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

};
