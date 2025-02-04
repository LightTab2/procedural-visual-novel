#pragma once
#include <QDialog>

#include <QDir>
#include <QFileDialog>
#include <QRegularExpressionValidator>
#include <QSettings>

#include "ui_NewProjectDialog.h"

/// Pop-up dialog, in which User can create a new Project
class NewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	NewProjectDialog(QWidget *parent = nullptr);

	NewProjectDialog(const NewProjectDialog&)            noexcept = delete;
	NewProjectDialog(NewProjectDialog&&)                 noexcept = default;
	NewProjectDialog& operator=(const NewProjectDialog&) noexcept = delete;

	QDir    projectPath();
	QString projectName();

private:
	void projectNameEdited(const QString& name);
	void projectPathEdited(const QString& name);

	bool checkInputErrors();

	/// The dialog enters "error" state, displaying the error and highlighting, which Input caused the error
	/// \param error The error message displayed as orange text
	/// \param bNameError Name's input box and label will become red, if true
	/// \param bPathError Path's input box and label will become red, if true
	void raiseInputError(const QString& error, bool bNameError = false, bool bPathError = false);

	/// The dialog enters "warning" state, displaying the warning and highlighting, which Input caused the error
	/// \param warning The warning message displayed as orange text
	/// \param bNameWarning Name's input box and label will become orange, if true
	/// \param bPathWarning Path's input box and label will become orange, if true
	void raiseInputWarning(const QString& warning, bool bNameWarning = false, bool bPathWarning = false);

	/// Also removes warnings
	void removeInputError();

	QDir    projectPath_ = QDir::currentPath();
	QString projectName_ = "NewProject";

	/// Invoked when clicking button with "..."
	QFileDialog chooseDirDialog_ = QFileDialog(nullptr, tr("Choose project location"), projectPath_.path());

	/// Allows only for the correct (portable on many filesystem) project name characters to be entered
	std::unique_ptr<QRegularExpressionValidator> correctNameValidator;

	Ui::NewProjectDialog ui_;
};
