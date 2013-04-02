#pragma once

#include "Interface.h"

#include "Task.h"
#include "EventRecorder.h"

class Cromartie : public BWAPI::AIModule
{
public:
	Cromartie();
	~Cromartie(void);

	virtual void onStart();
	virtual void onEnd(bool isWinner);

	virtual void onFrame();

	virtual void onSendText(std::string text);
	virtual void onPlayerLeft(Player player);
	virtual void registerListeners();

private:
	bool mOnBegin;

	int mLeavingGame;
	bool mSaidGG;
	EventRecorder* eventRecorderListener;
};