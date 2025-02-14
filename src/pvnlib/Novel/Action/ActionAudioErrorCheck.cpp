#include "pvnLib/Novel/Action/Audio/ActionAudioSetMusic.h"
#include "pvnLib/Novel/Action/Audio/ActionAudioSetSounds.h"

#include "pvnLib/Novel/Data/Scene.h"

bool ActionAudio::errorCheck(bool bComprehensive) const
{
	bool bError = Action::errorCheck(bComprehensive);

	//auto errorChecker = [this](bool bComprehensive)
	//{
	//};

	//Only leafs should report, but if needed for further debug, uncomment it
	//bError |= NovelLib::catchExceptions(errorChecker, bComprehensive))
	//if (bError)
	//	qDebug() << "Error occurred in ActionAudio::errorCheck of Scene \"" + parentEvent->parentScene->name + "\" Event" << parentEvent->getIndex();
	return bError;
}

bool ActionAudioSetMusic::errorCheck(bool bComprehensive) const
{
	bool bError = ActionAudio::errorCheck(bComprehensive);

	bError |= musicPlaylist_.errorCheck(bComprehensive);

	//auto errorChecker = [this](bool bComprehensive)
	//{
	//};

	//bError |= NovelLib::catchExceptions(errorChecker, bComprehensive);

	if (bError)
		qDebug() << "Error occurred in ActionAudioSetMusic::errorCheck of Scene \"" + parentEvent->parentScene->name + "\" Event" << parentEvent->getIndex();

	return bError;
}

bool ActionAudioSetSounds::errorCheck(bool bComprehensive) const
{
	bool bError = ActionAudio::errorCheck(bComprehensive);

	for (const Sound& sound : sounds_)
		bError |= sound.errorCheck(bComprehensive);

	//auto errorChecker = [this](bool bComprehensive)
	//{
	//};

	//bError |= NovelLib::catchExceptions(errorChecker, bComprehensive);
	if (bError)
		qCritical() << "An Error occurred in ActionAudioSetSounds::errorCheck of Scene \"" + parentEvent->parentScene->name + "\" Event" << parentEvent->getIndex();

	return bError;
}