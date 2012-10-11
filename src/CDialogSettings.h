#ifndef CDIALOGSETTINGS_H
#define CDIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class CDialogSettings;
}

class CDialogSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDialogSettings(QWidget *parent = 0);
    ~CDialogSettings();
    unsigned int num_layers;
    unsigned int num_neurons_hidden;
    float desired_error;
    unsigned int max_epochs;
    unsigned int epochs_between_reports;
    unsigned int hidden_activation;
    unsigned int output_activation;
    unsigned int stop_function;
    unsigned int train_algorithm;

    void setFileName(QString file);
    void Save();
    void Load();
private:
    Ui::CDialogSettings *ui;
    QString file;
};

#endif // CDIALOGSETTINGS_H
