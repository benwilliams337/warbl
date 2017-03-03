#ifndef TRAINER_H
#define TRAINER_H

#include "ofMain.h"

#define FONT_PATH					"segoeui.ttf"
#define FONT_SIZE					24
#define PITCH_HISTORY_LENGTH		160
#define MIN_POSSIBLE_PITCH			60.0f
#define MAX_POSSIBLE_PITCH			1200.0f
#define MISSED_FRAME_TOLERANCE		20
#define PITCH_VARIANCE_TOLERANCE    10.0f
#define CENTER_NOTE_TOLERANCE		10.0f
#define LEVEL_START_COUNTDOWN_TIME	5000 //Minimum of 3000
#define TOTAL_COUNTDOWN_TIME		LEVEL_START_COUNTDOWN_TIME + 2000
#define PROGRESS_BAR_WIDTH			200.0f
#define PROGRESS_BAR_HEIGHT			30.0f

class Trainer
{
	public:
		Trainer();
		virtual ~Trainer();

		void draw();
		void update(float curPitch);
		float getCenterFreq();
		void setLaunchTargetFreq(float newLaunchTargetFreq);
		bool isTrained();
		bool isInCountdown();
		bool isLevelStarted();
		bool isDone();
		void restart();
		ofTrueTypeFont* getFont();
	protected:
	private:
		enum Phase { HighNote, LowNote, CenterNote, Countdown };
		Phase phase;
		deque<float> pitchHistory;

		float highFreq;
		float lowFreq;
		float launchTargetFreq;

		float curAvgPitch;
		float curPitchVariance;
		int outOfRangeCounter;

		float updateForPitch(float curPitch);
		void saveNewPitch(float newPitch);
		void processPitchHistory();
		void resetPitchHistory();

		float countdownStartTime;
		string countdownMessage();

		ofTrueTypeFont* font;
};

#endif // TRAINER_H
