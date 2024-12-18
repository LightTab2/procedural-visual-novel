#pragma once
#include "pvnLib/Novel/Action/Visual/Animation/ActionSceneryObjectAnim.h"

#include "pvnLib/Novel/Data/Visual/Animation/AnimatorSceneryObjectColor.h"
#include "pvnLib/Novel/Data/Save/NovelState.h"

/// Creates AnimatorSceneryObjectColor and adds it to the Scenery, which will perform a **color** Animation on a SceneryObject
class ActionSceneryObjectAnimColor final : public ActionSceneryObjectAnim<AnimNodeDouble4D>
{
	friend class ActionVisitorCorrectAssetAnimColor;
	/// Swap trick
	friend void swap(ActionSceneryObjectAnimColor& first, ActionSceneryObjectAnimColor& second) noexcept;
public:
	explicit ActionSceneryObjectAnimColor(Event* const parentEvent) noexcept;
	/// \param sceneryObject Copies the SceneryObject pointer. It's okay to leave it as nullptr, as it will be loaded later. This is a very minor optimization 
	/// \param assetAnim Copies the AssetAnim pointer. It's okay to leave it as nullptr, as it will be loaded later. This is a very minor optimization 
	/// \param priority Animations can be queued, this is the priority in the queue (lower number equals higher priority)
	/// \param startDelay In milliseconds
	/// \param speed Animation speed multiplier
	/// \param Remember to copy the description to the constructor parameter description as well, if it changes
	/// \param timesPlayed `-1` means infinite times
	/// \exception Error Couldn't find the SceneryObject named `sceneryObjectName` or couldn't find/load the **color** AssetAnim named `assetAnimName`
	ActionSceneryObjectAnimColor(Event* const parentEvent, const QString& sceneryObjectName, const QString& assetAnimName = "", uint priority = 0, uint startDelay = 0, double speed = 1.0, int timesPlayed = 1, bool bFinishAnimationAtEventEnd = false, SceneryObject* sceneryObject = nullptr, AssetAnim<AnimNodeDouble4D>* assetAnim = nullptr);
	ActionSceneryObjectAnimColor(const ActionSceneryObjectAnimColor& obj)      noexcept;
	ActionSceneryObjectAnimColor(ActionSceneryObjectAnimColor&& obj)           noexcept;
	ActionSceneryObjectAnimColor& operator=(ActionSceneryObjectAnimColor obj)  noexcept;
	bool operator==(const ActionSceneryObjectAnimColor& obj) const             noexcept;
	bool operator!=(const ActionSceneryObjectAnimColor& obj) const             noexcept = default;

	/// \exception Error `sceneryObject_`/`assetAnim_` is invalid
	/// \return Whether an Error has occurred
	bool errorCheck(bool bComprehensive = false) const override;

	void run() override;

	/// Sets a function pointer that is called (if not nullptr) after the ActionSceneryObjectAnimColor's `void run()` allowing for data read. Consts are safe to be casted to non-consts, they are there to indicate you should not do that, unless you have a very reason for it
	void setOnRunListener(std::function<void(const Event* const parentEvent, const SceneryObject* const parentSceneryObject, const AssetAnimColor* const assetAnimColor, const uint& priority, const uint& startDelay, const double& speed, const int& timesPlayed, const bool& bFinishAnimationAtEventEnd)> onRun) noexcept;

	void setAssetAnim(const QString& assetAnimName, AssetAnim<AnimNodeDouble4D>* assetAnim = nullptr) noexcept;

	void acceptVisitor(ActionVisitor* visitor) override;

private:
	/// Needed for Serialization, to know the class of an object about to be Serialization loaded
	/// \return NovelLib::SerializationID corresponding to the class of a serialized object
	NovelLib::SerializationID getType() const noexcept override;

	/// A function pointer that is called (if not nullptr) after the ActionSceneryObjectAnimColor's `void run()` allowing for data read. Consts are safe to be casted to non-consts, they are there to indicate you should not do that, unless you have a very reason for it
	std::function<void(const Event* const parentEvent, const SceneryObject* const parentSceneryObject, const AssetAnimColor* const assetAnimColor, const uint& priority, const uint& startDelay, const double& speed, const int& timesPlayed, const bool& bFinishAnimationAtEventEnd)> onRun_ = nullptr;

public:
	//---SERIALIZATION---
	/// Loading an object from a binary file
	/// \param dataStream Stream (presumably connected to a QFile) to read from
	void serializableLoad(QDataStream& dataStream) override;
	/// Saving an object to a binary file
	/// \param dataStream Stream (presumably connected to a QFile) to save to
	void serializableSave(QDataStream& dataStream) const override;
};