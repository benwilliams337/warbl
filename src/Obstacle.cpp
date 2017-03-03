#include "Obstacle.h"

Obstacle::Obstacle(float myLength)
{
	length = myLength;
}

Obstacle::~Obstacle()
{
	//dtor
}

float Obstacle::getLengthMillisecs()
{
	return length;
}

float Obstacle::freqYPos(float freq, float minFreq, float maxFreq)
{
    return ofGetWindowHeight() * (1.0f - ((freq - minFreq) / (maxFreq - minFreq)));
}

EmptySpace::EmptySpace(float myLength):Obstacle(myLength) { }

void EmptySpace::draw(float xStart, float speed, float minFreq, float maxFreq)
{
	//Do nothing
}

bool EmptySpace::pass(float freq, float t)
{
	return true;
}

Tunnel::Tunnel(float myLength, float c_targetFreq, float c_allowedError):Obstacle(myLength)
{
	targetFreq = c_targetFreq;
	allowedError = c_allowedError;
}

void Tunnel::draw(float xStart, float speed, float minFreq, float maxFreq)
{
	ofPushStyle();
		ofPushMatrix();
			//Translate to start of our obstacle's window
			ofTranslate(xStart, 0, 0);
			ofSetColor(255, 255, 255);
			//Remember, larger freqs = lower y positions
			float gapStart = freqYPos(targetFreq + allowedError, minFreq, maxFreq);
			float gapEnd = freqYPos(targetFreq - allowedError, minFreq, maxFreq);
			ofRect(0, 0, length * speed, gapStart);
			ofRect(0, gapEnd, length * speed, ofGetWindowHeight() - gapEnd);
		ofPopMatrix();
	ofPopStyle();
}

bool Tunnel::pass(float freq, float t)
{
    return abs(freq - targetFreq) < allowedError;
}

SlopedTunnel::SlopedTunnel(float myLength, float c_startFreq, float c_endFreq, float c_allowedError):Obstacle(myLength)
{
	startFreq = c_startFreq;
	endFreq = c_endFreq;
	allowedError = c_allowedError;
}

void SlopedTunnel::draw(float xStart, float speed, float minFreq, float maxFreq)
{
	ofPushStyle();
		ofPushMatrix();
			//Translate to start of our obstacle's window
			ofTranslate(xStart, 0, 0);
			ofSetColor(255, 255, 255);

			//Calculate top/bottom y-coordinates of start/finish of tunnel
			//Remember, larger freqs = lower y positions
			float startTop = freqYPos(startFreq + allowedError, minFreq, maxFreq);
			float startBottom = freqYPos(startFreq - allowedError, minFreq, maxFreq);
			float endTop = freqYPos(endFreq + allowedError, minFreq, maxFreq);
			float endBottom = freqYPos(endFreq - allowedError, minFreq, maxFreq);
			float tunnelLength = length * speed;
			float screenHeight = ofGetWindowHeight();

			//Draw top of tunnel
			ofBeginShape();
				ofVertex(0, 0);
				ofVertex(0, startTop);
				ofVertex(tunnelLength, endTop);
				ofVertex(tunnelLength, 0);
			ofEndShape();

			//Draw bottom of tunnel
			ofBeginShape();
				ofVertex(0, screenHeight);
				ofVertex(0, startBottom);
				ofVertex(tunnelLength, endBottom);
				ofVertex(tunnelLength, screenHeight);
			ofEndShape();
		ofPopMatrix();
	ofPopStyle();
}

bool SlopedTunnel::pass(float freq, float t)
{
	float percentThrough = t / length;
	float curTarget = startFreq + percentThrough * (endFreq - startFreq);
	return abs(freq - curTarget) < allowedError;
}

Asteroid::Asteroid(float myLength, float c_targetFreq, float c_width):Obstacle(myLength)
{
	targetFreq = c_targetFreq;
	freqWidth = c_width;
}

void Asteroid::draw(float xStart, float speed, float minFreq, float maxFreq)
{
	ofPushStyle();
		ofPushMatrix();
			//Translate to start of our obstacle's window
			ofTranslate(xStart, 0, 0);
			ofSetColor(255, 255, 255);
			//Remember, larger freqs = lower y positions
			float obsStart = freqYPos(targetFreq + freqWidth, minFreq, maxFreq);
			float obsEnd = freqYPos(targetFreq - freqWidth, minFreq, maxFreq);
			ofRect(0, obsStart, length * speed, obsEnd - obsStart);
		ofPopMatrix();
	ofPopStyle();
}

bool Asteroid::pass(float freq, float t)
{
	return abs(freq - targetFreq) > freqWidth;
}
