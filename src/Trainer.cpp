#include "Trainer.h"

Trainer::Trainer()
{
	phase = HighNote;
	outOfRangeCounter = 0;
	highFreq = 0;
	lowFreq = 0;
	launchTargetFreq = 0;

	font = new ofTrueTypeFont();
	font->loadFont(FONT_PATH, FONT_SIZE);
}

Trainer::~Trainer()
{
	//dtor
}

void Trainer::update(float curPitch)
{
	if(phase != Countdown)
	{
		float foundPitch = updateForPitch(curPitch);
		if(foundPitch == 0)
			return;
		if(phase == HighNote)
		{
            highFreq = foundPitch;
            phase = LowNote;
            resetPitchHistory();
		}
		else if(phase == LowNote)
		{
			lowFreq = foundPitch;
			//TODO: sanity checking on detected pitches
			phase = CenterNote;
			resetPitchHistory();
		}
		else if(phase == CenterNote && launchTargetFreq != 0)
		{
			float dist = abs(foundPitch - launchTargetFreq);
			if(dist < CENTER_NOTE_TOLERANCE)
			{
				phase = Countdown;
				countdownStartTime = ofGetElapsedTimeMillis();
			}
		}
	}
}

void Trainer::draw()
{
	ofPushStyle();
		ofPushMatrix();

			string message = "";
			if(phase == HighNote)
				message = "Please sing a comfortable high note.";
			else if(phase == LowNote)
				message = "Please sing a comfortable low note.";
			else if(phase == CenterNote)
				message = "Please align your ship with the target.";
			else if(phase == Countdown)
				message = countdownMessage();

			float stringHeight = 0;
			float stringWidth = 0;
			float halfScreenHeight = ofGetWindowHeight() / 2;
			float halfScreenWidth = ofGetWindowWidth() / 2;
			if(font->isLoaded())
			{
				stringHeight = font->stringHeight(message);
				stringWidth = font->stringWidth(message);
				ofSetColor(0, 0, 0);
				ofRect(halfScreenWidth - stringWidth/2 - 15, halfScreenHeight - stringHeight - 20, stringWidth + 30, stringHeight + 20);
				ofSetColor(255, 255, 255);
				font->drawString(message, halfScreenWidth - stringWidth/2, halfScreenHeight - stringHeight/2);
			}
			if(phase == HighNote || phase == LowNote)
			{
				ofNoFill();
				ofRect(halfScreenWidth - PROGRESS_BAR_WIDTH/2, halfScreenHeight + stringHeight/2 + 5, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT);
				ofFill();
				float barWidth = ((float)pitchHistory.size() / (float)PITCH_HISTORY_LENGTH) * PROGRESS_BAR_WIDTH;
				ofRect(halfScreenWidth - PROGRESS_BAR_WIDTH/2, halfScreenHeight + stringHeight/2 + 5, barWidth, PROGRESS_BAR_HEIGHT);
			}
			ofPopMatrix();
	ofPopStyle();
}

bool Trainer::isTrained()
{
	return phase == CenterNote || phase == Countdown;
}

bool Trainer::isInCountdown()
{
	return phase == Countdown;
}

bool Trainer::isLevelStarted()
{
	if(phase != Countdown)
		return false;
	float dt = ofGetElapsedTimeMillis() - countdownStartTime;
	return dt >= LEVEL_START_COUNTDOWN_TIME;
}

bool Trainer::isDone()
{
	float dt = ofGetElapsedTimeMillis() - countdownStartTime;
	return dt >= TOTAL_COUNTDOWN_TIME;
}

void Trainer::restart()
{
    //Don't restart to training, reset to CenterNote
    phase = CenterNote;
    resetPitchHistory();
}

ofTrueTypeFont* Trainer::getFont()
{
	return font;
}

float Trainer::getCenterFreq()
{
    return lowFreq + ((highFreq - lowFreq) / 2);
}

void Trainer::setLaunchTargetFreq(float newLaunchTargetFreq)
{
	launchTargetFreq = newLaunchTargetFreq;
}

//Accepts a new frame of pitch data and keeps a running analysis of pitch history data.
//Once it's confident it's zeroed in on the sung pitch, returns it. Otherwise returns 0.
float Trainer::updateForPitch(float curPitch)
{
	//Only accept pitches within a defined range
	if(curPitch >= MIN_POSSIBLE_PITCH && curPitch <= MAX_POSSIBLE_PITCH)
	{
		outOfRangeCounter = 0;
		saveNewPitch(curPitch);
		processPitchHistory();

		//Once we have a full pitch history buffer and our variance is within tolerance, accept the average as our pitch
		if(pitchHistory.size() == PITCH_HISTORY_LENGTH && curPitchVariance < PITCH_VARIANCE_TOLERANCE)
		{
            return curAvgPitch;
		}
	}
	else
	{
		//Keep a tally of frames with pitches outside range. Past a certain tolerance, start the process over.
		outOfRangeCounter++;
		if(outOfRangeCounter > MISSED_FRAME_TOLERANCE)
		{
            resetPitchHistory();
		}
	}
	return 0;
}

void Trainer::saveNewPitch(float newPitch)
{
    if(pitchHistory.size() >= PITCH_HISTORY_LENGTH)
		pitchHistory.pop_front();
	pitchHistory.push_back(newPitch);
}

void Trainer::processPitchHistory()
{
    //Mean
    float pitchSum = 0;
    for(int i = 0; i < pitchHistory.size(); i++)
	{
		pitchSum += pitchHistory[i];
	}
	curAvgPitch = pitchSum / (float)pitchHistory.size();

	//Variance
	float varianceSum = 0;
	for(int i = 0; i < pitchHistory.size(); i++)
	{
		varianceSum += pow(pitchHistory[i] - curAvgPitch, 2);
	}
	curPitchVariance = varianceSum / (float)pitchHistory.size();
}

void Trainer::resetPitchHistory()
{
    curAvgPitch = 0;
    curPitchVariance = 0;
    outOfRangeCounter = 0;
    pitchHistory.clear();
}

string Trainer::countdownMessage()
{
    float dt = ofGetElapsedTimeMillis() - countdownStartTime;
    if(dt < LEVEL_START_COUNTDOWN_TIME - 3000)
		return "Ready...";
	else if(dt < LEVEL_START_COUNTDOWN_TIME - 2000)
		return "3";
	else if(dt < LEVEL_START_COUNTDOWN_TIME - 1000)
		return "2";
	else if(dt < LEVEL_START_COUNTDOWN_TIME)
		return "1";
	else if(dt < TOTAL_COUNTDOWN_TIME)
		return "Go!";
	return "";
}
