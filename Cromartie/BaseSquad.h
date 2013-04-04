#pragma once

#include "Interface.h"

#include "Task.h"
#include "BuildOrder.h"

class BaseSquadTask : public Task
{
public:
	BaseSquadTask(ArmyBehaviour behaviour)
		: Task(TaskType::Army)
		, mArmyBehaviour(behaviour)
	{}

	virtual int getEndTime() const = 0;
	virtual int getEndTime(Unit unit) const = 0;

	virtual int getPriority(Unit unit) const = 0;

	virtual Position getStartLocation(Unit unit) const = 0;
	virtual Position getEndLocation(Unit unit) const = 0;

	virtual bool preUpdate() = 0;
	virtual bool update() = 0;

	virtual bool waitingForUnit(Unit unit) const = 0;
	virtual void giveUnit(Unit unit) = 0;
	virtual void returnUnit(Unit unit) = 0;
	virtual bool morph(Unit unit, BWAPI::UnitType previousType) = 0;
	virtual UnitGroup getFinishedUnits() = 0;

	virtual std::string getTaskName() const = 0;
	virtual std::string getOutputName() const = 0;

	virtual unsigned int controlSize() = 0;

	virtual void updateRequirements(){}
	virtual const UnitGroup units() const = 0;

	void changeBehaviour(ArmyBehaviour armyBehaviour) { mArmyBehaviour = armyBehaviour; }
	virtual void attack() {}
	virtual void stop() {}
protected:
	ArmyBehaviour mArmyBehaviour;
};

typedef std::tr1::shared_ptr<BaseSquadTask> BaseSquadTaskPointer;