#include "Level.h"

//Level::Level(float aSpeed)
//{
//	t = 0;
//	curObstacleIndex = 0;
//	setSpeed(aSpeed);
//}

Level::Level(int screenWidth, float tempo, float beatsOnScreen)
{
	t = 0;
	totalTimePassed = 0;
	curObstacleIndex = 0;
	setSpeed(screenWidth, tempo, beatsOnScreen);
}

Level::~Level()
{
	//TODO
}

void Level::advanceTime(float dt)
{
	t += dt;
	totalTimePassed += dt;

	//Stop here if we're at the end of the level
	if(curObstacleIndex >= obstacles.size())
	{
		return;
	}

	//Move onto the next obstacle if we've passed the current one
	float curObstacleLength = obstacles[curObstacleIndex]->getLengthMillisecs();
	if(t > curObstacleLength)
	{
        t -= curObstacleLength;
        curObstacleIndex++;
	}
}

bool Level::isDone()
{
	return curObstacleIndex >= obstacles.size();
}

void Level::restart()
{
	t = 0;
	totalTimePassed = 0;
	curObstacleIndex = 0;
}

float Level::getSpeed()
{
	return speed;
}

//void Level::setSpeed(float pixelsPerMillis)
//{
//	speed = pixelsPerMillis;
//}

//Sets the speed the level moves at, based on 3 parameters:
//Screen width: width of the screen in pixels (int)
//Tempo: tempo of level in beats per minute (float)
//Beats On Screen: Number of beats that should be on-screen at any given time (float)
void Level::setSpeed(int screenWidth, float aTempo, float beatsOnScreen)
{
	//Units: screenWidth pixels/screen * 1/beatsOnScreen screens/beat * tempo beats/min * 1/60 mins/sec * 1/1000 secs/millisec
    speed = ((float)screenWidth) * aTempo / (beatsOnScreen * 60000);
    tempo = aTempo;
}

float Level::timeToPixels(float timeAmt)
{
	return timeAmt * speed;
}

float Level::getTempo()
{
	return tempo;
}

float Level::beatsToTime(float beats)
{
    //beats * mins/beat * secs/min * millisecs/sec
    return 60000.0f * beats / tempo;
}

void Level::setSemitoneRange(int minS, int maxS)
{
	minSemitone = minS;
	maxSemitone = maxS;
}

int Level::getCenterSemitone()
{
	return minSemitone + ((maxSemitone - minSemitone) / 2);
}

void Level::setCenterFreq(float cFreq)
{
	centerFreq = cFreq;
}

float Level::getMinFreq()
{
	return semitoneToFreq(minSemitone);
}

float Level::getMaxFreq()
{
	return semitoneToFreq(maxSemitone);
}

float Level::getDoFreq()
{
	return semitoneToFreq(0);
}

float Level::semitoneToFreq(int semitone)
{
	return pow(2, ((float)(semitone - getCenterSemitone())) / 12.0f) * centerFreq;
}

void Level::draw(float leftBuffer)
{
	float pixelOffset = -totalTimePassed * speed; //Draw current obstacle the appropriate distance off-screen to the left
    for(int i = 0; i < obstacles.size(); i++)
	{
		float curObstacleWidth = timeToPixels(obstacles[i]->getLengthMillisecs());

		//Don't draw anything off-screen to the left
		if(pixelOffset + curObstacleWidth >= -leftBuffer)
		{
			obstacles[i]->draw(pixelOffset, speed, getMinFreq(), getMaxFreq());
		}

        pixelOffset += curObstacleWidth; //Move forward the length of the obstacle we just drew

		//Break out if we've moved into obstacles off-screen to the right
        if(pixelOffset > ofGetWindowWidth())
			break;
	}
}


void Level::addObstacle(Obstacle* newObstacle)
{
	obstacles.push_back(newObstacle);
}

Obstacle* Level::getCurrentObstacle()
{
    if(curObstacleIndex >= obstacles.size())
		return NULL;
	return obstacles[curObstacleIndex];
}

bool Level::pass(float freq, float dFreq, float dt)
{
	//Iterate through all obstacles
	float curTime = 0;
	for(int i = 0; i < obstacles.size(); i++)
	{
		float curObstacleTime = obstacles[i]->getLengthMillisecs();
		float shipLeftTime = totalTimePassed - dt;
		float shipRightTime = totalTimePassed + dt;

		//Test to see if the left or right ends of the ship are under the current obstacle
		bool isLeftUnderObstacle = shipLeftTime >= curTime && shipLeftTime < curTime + curObstacleTime;
		bool isRightUnderObstacle = shipRightTime >= curTime && shipRightTime < curTime + curObstacleTime;

		if(isLeftUnderObstacle)
		{
			//Test left point of ship
			if(!testBothFreqs(obstacles[i], freq, dFreq, shipLeftTime - curTime))
				return false;

			if(!isRightUnderObstacle)
			{
				//Test right end of obstacle
				float percentAlongShip = (curTime + curObstacleTime - shipLeftTime)/(2.0f*dt);
				if(!testBothFreqs(obstacles[i], freq, dFreq * (1.0f - percentAlongShip), curObstacleTime))
					return false;
			}
		}
		if(isRightUnderObstacle)
		{
			//Test right point of ship
			if(!testBothFreqs(obstacles[i], freq, 0, shipRightTime - curTime))
				return false;

			if(!isLeftUnderObstacle)
			{
                //Test left end of obstacle
                float percentAlongShip = (curTime - shipLeftTime)/(2.0f*dt);
                if(!testBothFreqs(obstacles[i], freq, dFreq * (1.0f - percentAlongShip), 0))
					return false;
			}
			break; //Can stop looping now, we know the ship won't be under any future obstacles
		}

		curTime += curObstacleTime;
	}
	return true;
}

bool Level::testBothFreqs(Obstacle* obstacle, float freq, float dFreq, float t)
{
	float topTestFreq = freq + dFreq;
	float bottomTestFreq = freq - dFreq;
	if(!obstacle->pass(topTestFreq, t))
		return false;
	if(dFreq != 0 && !obstacle->pass(bottomTestFreq, t))
		return false;
	return true;
}

//Level-building tools
void Level::addSpace(float beats)
{
	addObstacle(new EmptySpace(beatsToTime(beats)));
}

void Level::addTunnel(float beats, int semitone)
{
	addTunnel(beats, semitone, STANDARD_TOLERANCE);
}

void Level::addTunnel(float beats, int semitone, float tolerance)
{
	addObstacle(new Tunnel(beatsToTime(beats), semitoneToFreq(semitone), tolerance));
}

void Level::addSlopedTunnel(float beats, int startSemitone, int endSemitone)
{
    addSlopedTunnel(beats, startSemitone, endSemitone, STANDARD_TOLERANCE);
}

void Level::addSlopedTunnel(float beats, int startSemitone, int endSemitone, float tolerance)
{
	addObstacle(new SlopedTunnel(beatsToTime(beats), semitoneToFreq(startSemitone), semitoneToFreq(endSemitone), tolerance));
}

void Level::addAsteroid(float beats, int semitone, float width)
{
    addObstacle(new Asteroid(beatsToTime(beats), semitoneToFreq(semitone), width));
}
