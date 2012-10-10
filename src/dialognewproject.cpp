#include "dialognewproject.h"
#include "ui_dialognewproject.h"

DialogNewProject::DialogNewProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewProject)
{
    ui->setupUi(this);
    connect(ui->browseFann,SIGNAL(clicked()),this,SLOT(slotBrowseFann()));
    connect(ui->browseInput,SIGNAL(clicked()),this,SLOT(slotBrowseInput()));
    connect(ui->browseOutput,SIGNAL(clicked()),this,SLOT(slotBrowseOutput()));
    connect(ui->browseLearn,SIGNAL(clicked()),this,SLOT(slotBrowseLearn()));
    connect(ui->browseTest,SIGNAL(clicked()),this,SLOT(slotBrowseTest()));
    connect(ui->browseDescription,SIGNAL(clicked()),this,SLOT(slotBrowseSequence()));

}

DialogNewProject::~DialogNewProject()
{
    delete ui;
}

QString DialogNewProject::browseFolder()
{
    return QFileDialog::getOpenFileName(this, tr("Open datas"), "C:\\", tr("Image Files (*.txt *.net)"));
}

void DialogNewProject::slotBrowseFann()
{
    ui->fannFile->setText(browseFolder());
}

void DialogNewProject::slotBrowseInput()
{
    ui->labelInCriteria->setText(browseFolder());
}

void DialogNewProject::slotBrowseOutput()
{
    ui->labelOutCriteria->setText(browseFolder());
}

void DialogNewProject::slotBrowseLearn()
{
    ui->activationLearnFile->setText(browseFolder());
}

void DialogNewProject::slotBrowseTest()
{
    ui->activationTestFile->setText(browseFolder());
}

void DialogNewProject::slotBrowseSequence()
{
    ui->sequenceTestFile->setText(browseFolder());
}

void DialogNewProject::Save()
{
    QSettings settings(file, QSettings::IniFormat);
    settings.setValue("Project/LabelInputFile",ui->labelInCriteria->text());
    settings.setValue("Project/LabelOutputFile",ui->labelOutCriteria->text());
    settings.setValue("Project/LearnFile",ui->activationLearnFile->text());
    settings.setValue("Project/TestFile",ui->activationTestFile->text());
    settings.setValue("Project/LabelActivationFile",ui->sequenceTestFile->text());
    settings.setValue("Project/FannFile",ui->fannFile->text());
}

void DialogNewProject::setFileName(QString file)
{
    this->file = file;
}
