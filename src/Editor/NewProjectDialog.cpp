#include "NewProjectDialog.h"

#include <QPushButton>

#include <boost/filesystem.hpp>

// TODO: this is actually pretty good. I should release this as a standalone custom component one day. And make portability a boolean

NewProjectDialog::NewProjectDialog(QWidget* parent) : QDialog(parent)
{
	ui_.setupUi(this);

	// Loading options from QSettings, defaulting to initialized value
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	projectName_ = settings.value("Project Name", projectName_).toString();
	projectPath_ = settings.value("Project Path", projectPath_.path()).toString();
	ui_.projectName_lineEdit->setText(projectName_);
	ui_.projectPath_lineEdit->setText(projectPath_.path() + '/' + projectName_);

	// Choosing directory pop up will begin from the current directory
	if (projectPath_.exists())
		chooseDirDialog_.setDirectory(projectPath_);

	// Buttons from QButtonBox decide whether a new project will be created or the process is cancelled
	connect(ui_.dialogButtons_buttonBox, &QDialogButtonBox::accepted, this, &NewProjectDialog::accept);
	connect(ui_.dialogButtons_buttonBox, &QDialogButtonBox::rejected, this, &NewProjectDialog::reject);

	// Create directory choosing menu on "..." button click 
	connect(ui_.chooseDirectory_toolButton, &QToolButton::clicked, &chooseDirDialog_, [&](bool checked) { chooseDirDialog_.open(); });
	// Allow only to pick Directories
	chooseDirDialog_.setFileMode(QFileDialog::Directory);
	chooseDirDialog_.setViewMode(QFileDialog::Detail);

	connect(ui_.projectName_lineEdit, &QLineEdit::textChanged, this, &NewProjectDialog::projectNameEdited);
	connect(ui_.projectPath_lineEdit, &QLineEdit::textChanged, this, &NewProjectDialog::projectPathEdited);

	// Picking a file from a popup from "..." button will change the projectPath
	connect(&chooseDirDialog_, &QFileDialog::fileSelected, this, [&](const QString& selectedDirectory) 
	{ 
		ui_.projectPath_lineEdit->setText(QDir::cleanPath(QDir(selectedDirectory).absolutePath()) + '/' + projectName_); 
	});

	/// Allows only for the correct (portable on many filesystem) project name characters to be entered
	QRegularExpression allowedName("^(?!-)[a-zA-Z0-9-_]+(?:[.][a-zA-Z0-9-_]{1,3})*$");
	correctNameValidator.reset(new QRegularExpressionValidator(allowedName, this));
	ui_.projectName_lineEdit->setValidator(correctNameValidator.get());
	
	checkInputErrors();
}

QDir NewProjectDialog::projectPath()
{
	return projectPath_;
}

QString NewProjectDialog::projectName()
{
	return projectName_;
}

void NewProjectDialog::projectNameEdited(const QString& newName)
{
	projectName_ = newName;

	ui_.projectPath_lineEdit->disconnect();
	ui_.projectPath_lineEdit->setText(projectPath_.path() + '/' + newName);
	connect(ui_.projectPath_lineEdit, &QLineEdit::textChanged, this, &NewProjectDialog::projectPathEdited);

	checkInputErrors();
}

void NewProjectDialog::projectPathEdited(const QString& newPath)
{
	qsizetype lastSlash = newPath.lastIndexOf('\\');
	if (lastSlash != -1)
	{
		ui_.projectPath_lineEdit->setText(QDir::cleanPath(newPath));
		return;
	}

	lastSlash = newPath.lastIndexOf('/');
	if (lastSlash == -1)
	{
		raiseInputError("the directory cannot be empty", false, true);
		return;
	}

	projectPath_ = newPath.left(lastSlash);
	projectName_ = newPath.sliced(lastSlash + 1);

	ui_.projectName_lineEdit->disconnect();
	ui_.projectName_lineEdit->setText(projectName_);
	connect(ui_.projectName_lineEdit, &QLineEdit::textChanged, this, &NewProjectDialog::projectNameEdited);

	checkInputErrors();
}

bool NewProjectDialog::checkInputErrors()
{
	// Reset and assume everything is alright
	removeInputError();

	bool bError = false;

	if (!boost::filesystem::portable_file_name(projectName_.toLocal8Bit().constData()))
	{
		raiseInputError("project name is invalid (hover over it, to see its tooltip)", true);
		bError = true;
	}

	if (projectName_.trimmed().isEmpty())
	{
		raiseInputError("project name cannot be empty", true);
		bError = true;
	}

	if (!QDir::isAbsolutePath(projectPath_.path()))
	{
		raiseInputError("project path must be an absolute path", false, true);
		bError = true;
	}

	if (!bError && !projectPath_.exists())
		raiseInputWarning("project path doesn't exist, it will be created if possible", false, true);

	return bError;
}

void NewProjectDialog::raiseInputError(const QString& error, bool bNameError, bool bPathError)
{
	ui_.error_label->setText("Error: " + error + '!');
	QPushButton* okButton = ui_.dialogButtons_buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	if (okButton)
	{
		okButton->setDisabled(true);
		okButton->clearFocus(); // todo: doesn't work, find out why
	}

	if (bNameError)
	{
		ui_.projectName_label->setStyleSheet("QLabel { color: red; }");
		ui_.projectName_lineEdit->setStyleSheet("QLineEdit { border: 2px solid red; }");
	}

	if (bPathError)
	{
		ui_.projectPath_label->setStyleSheet("QLabel { color: red; }");
		ui_.projectPath_lineEdit->setStyleSheet("QLineEdit { border: 2px solid red; }");
	}
}

void NewProjectDialog::raiseInputWarning(const QString& warning, bool bNameWarning, bool bPathWarning)
{
	ui_.error_label->setText("Warning: " + warning + '!');

	if (bNameWarning)
	{
		ui_.projectName_label->setStyleSheet("QLabel { color: orange; }");
		ui_.projectName_lineEdit->setStyleSheet("QLineEdit { border: 2px solid orange; }");
	}

	if (bPathWarning)
	{
		ui_.projectPath_label->setStyleSheet("QLabel { color: orange; }");
		ui_.projectPath_lineEdit->setStyleSheet("QLineEdit { border: 2px solid orange; }");
	}
}

void NewProjectDialog::removeInputError()
{
	ui_.error_label->setText("");
	QPushButton* okButton = ui_.dialogButtons_buttonBox->button(QDialogButtonBox::StandardButton::Ok);
	if (okButton)
		okButton->setDisabled(false);

	ui_.projectName_label->setStyleSheet("QLabel { }");
	ui_.projectName_lineEdit->setStyleSheet("QLineEdit {}");
	ui_.projectPath_label->setStyleSheet("QLabel { }");
	ui_.projectPath_lineEdit->setStyleSheet("QLineEdit { }");
}