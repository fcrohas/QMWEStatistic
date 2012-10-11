#include "CDialogSettings.h"
#include "ui_CDialogSettings.h"
#include <QSettings>

CDialogSettings::CDialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDialogSettings)
{
    ui->setupUi(this);
}

CDialogSettings::~CDialogSettings()
{
    delete ui;
}

void CDialogSettings::Load()
{
    ui->hiddenNeurons->setText(QString("%1").arg(num_neurons_hidden));
    ui->desiredError->setText(QString("%1").arg(desired_error));
    ui->numberLayers->setText(QString("%1").arg(num_layers));
    ui->maxEpochs->setText(QString("%1").arg(max_epochs));
    ui->epochsReports->setText(QString("%1").arg(epochs_between_reports));
    ui->hiddenFunction->setCurrentIndex(hidden_activation);
    ui->outputFunction->setCurrentIndex(output_activation);
    ui->stopFunction->setCurrentIndex(stop_function);
    ui->trainMethod->setCurrentIndex(train_algorithm);
}

void CDialogSettings::setFileName(QString file)
{
    this->file = file;
}

void CDialogSettings::Save()
{
    QSettings settings(file, QSettings::IniFormat);
    settings.setValue("Settings/hiddenNeurons",ui->hiddenNeurons->text());
    settings.setValue("Settings/desiredError",ui->desiredError->text());
    settings.setValue("Settings/numberLayers",ui->numberLayers->text());
    settings.setValue("Settings/maxEpochs",ui->maxEpochs->text());
    settings.setValue("Settings/epochsReports",ui->epochsReports->text());
    settings.setValue("Settings/hiddenFunction",ui->hiddenFunction->currentIndex());
    settings.setValue("Settings/outputFunction",ui->outputFunction->currentIndex());
    settings.setValue("Settings/stopFunction",ui->stopFunction->currentIndex());
    settings.setValue("Settings/trainMethod",ui->trainMethod->currentIndex());
}
