#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "ofMain.h"

class Obstacle
{
	public:
		Obstacle(float myLength);
		virtual ~Obstacle();
		virtual void draw(float xStart, float speed, float minFreq, float maxFreq) { };
		virtual bool pass(float freq, float t) { };
		virtual bool isScoreable() { };
		float getLengthMillisecs();
		float getLengthPixels();
	protected:
		float length; //Measured in millisecs

		float freqYPos(float freq, float minFreq, float maxFreq);
	private:

};

class EmptySpace : public Obstacle
{
	public:
		EmptySpace(float myLength);
		void draw(float xStart, float speed, float minFreq, float maxFreq);
		bool pass(float freq, float t);
		bool isScoreable() { return false; };
};

class Tunnel : public Obstacle
{
	public:
		Tunnel(float myLength, float c_targetFreq, float c_allowedError);
		void draw(float xStart, float speed, float minFreq, float maxFreq);
		bool pass(float freq, float t);
		bool isScoreable() { return true; };
	private:
		float targetFreq;
		float allowedError;
};

class SlopedTunnel : public Obstacle
{
	public:
		SlopedTunnel(float myLength, float c_startFreq, float c_endFreq, float c_allowedError);
		void draw(float xStart, float speed, float minFreq, float maxFreq);
		bool pass(float freq, float t);
		bool isScoreable() { return true; };
	private:
		float startFreq;
		float endFreq;
		float allowedError;
};

class Asteroid : public Obstacle
{
	public:
        Asteroid(float myLength, float c_targetFreq, float c_width);
        void draw(float xStart, float speed, float minFreq, float maxFreq);
        bool pass(float freq, float t);
        bool isScoreable() { return true; }
	private:
		float targetFreq;
		float freqWidth;
};

#endif // OBSTACLE_H
