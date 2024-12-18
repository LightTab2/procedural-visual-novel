#include "pvnLib/Novel/Action/Stat/ActionStatAll.h"

#include "pvnLib/Novel/Data/Save/NovelState.h"
#include "pvnLib/Novel/Data/Scene.h"
#include "pvnLib/Exceptions.h"

bool ActionStat::errorCheck(bool bComprehensive) const
{
	bool bError = Action::errorCheck(bComprehensive);

	auto errorChecker = [this](bool bComprehensive)
	{
		if (NovelState::getCurrentlyLoadedState()->getStat(statName_) == nullptr)
		{
			qCritical() << NovelLib::ErrorType::StatInvalid << "No valid Stat assigned. Was it deleted and not replaced?";
			if (!statName_.isEmpty())
				qCritical() << NovelLib::ErrorType::StatMissing << "Stat \"" + statName_ + "\" does not exist. Definition file might be corrupted";
		}
	};

	bError |= NovelLib::catchExceptions(errorChecker, bComprehensive);
	//if (bError)
	//	qDebug() << "Error occurred in ActionStat::errorCheck of Scene \"" + parentEvent->parentScene->name + "\" Event" << parentEvent->getIndex();
	return bError;
}

bool ActionStatSetValue::errorCheck(bool bComprehensive) const
{
	bool bError = ActionStat::errorCheck(bComprehensive);

	auto errorChecker = [this](bool bComprehensive)
	{
		//TODO: check `expression` by trying to parse it with an evaluator
	};

	bError |= NovelLib::catchExceptions(errorChecker, bComprehensive);
	if (bError)
		qDebug() << "Error occurred in ActionStatSetValue::errorCheck of Scene \"" + parentEvent->parentScene->name + "\" Event" << parentEvent->getIndex();

	return bError;
}
