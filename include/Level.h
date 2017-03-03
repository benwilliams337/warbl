#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Obstacle.h"

#define STANDARD_TOLERANCE 			20.0f

class Level
{
	public:
//		Level(float aSpeed);
		Level(int screenWidth, float tempo, float beatsOnScreen);
		virtual ~Level();

		void draw(float leftBuffer);
		void advanceTime(float dt);
		Obstacle* getCurrentObstacle();
		bool pass(float freq, float dFreq, float dt);
		void addObstacle(Obstacle* newObstacle);
		bool isDone();
		void restart();

		//Level-building functions
		void addSpace(float beats);
		void addTunnel(float beats, int semitone);
		void addTunnel(float beats, int semitone, float tolerance);
		void addSlopedTunnel(float beats, int startSemitone, int endSemitone);
		void addSlopedTunnel(float beats, int startSemitone, int endSemitone, float tolerance);
		void addAsteroid(float beats, int semitone, float width);

		float getSpeed();
//		void setSpeed(float pixelsPerMillis);
		void setSpeed(int screenWidth, float tempo, float beatsOnScreen);

		float getTempo();
//		void setTempo(float newTempo);

		void setSemitoneRange(int minS, int maxS); //Kind of cheating for now...
		void setCenterFreq(float cFreq);
		float getMinFreq();
		float getMaxFreq();
		float getDoFreq();
	protected:
	private:
		std::vector<Obstacle*> obstacles;
		int curObstacleIndex;
		float t; //Number of millisecs into current obstacle
		float totalTimePassed; //Number of millisecs we are into level
		//curObstacleIndex and t together define our position in the level
		float speed; //Speed the level progresses, in pixels per second
		float tempo; //Tempo of song being represented, in bpm

		int minSemitone;
		int maxSemitone;
		float centerFreq;

		bool testBothFreqs(Obstacle* obstacle, float freq, float dFreq, float t);

		float timeToPixels(float timeAmt);
		float beatsToTime(float beats);
		float semitoneToFreq(int semitone);
		int getCenterSemitone();
};

#endif // LEVEL_H
