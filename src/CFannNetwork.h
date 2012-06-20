#ifndef CFANNNETWORK_H
#define CFANNNETWORK_H

#include <QObject>
#include <QList>
#include <QString>

// Include Neural network library
#include "fann.h"


class CFannNetwork : public QObject
{
    Q_OBJECT
public:
    explicit CFannNetwork(QObject *parent = 0);
    ~CFannNetwork();
    unsigned int num_input;
    unsigned int num_output;
    unsigned int num_layers;
    unsigned int num_neurons_hidden;
    float desired_error;
    unsigned int max_epochs;
    unsigned int epochs_between_reports;

    float buildNetwork(void);

    void setTrainFile(QString &file);
    void setTestFile(QString &file);
    void loadNetwork(QString &file);
    void saveNetwork(QString &file);

    struct record{
        int line;
        float output;
        float want;
        float difference;
    };

    QList<record> testNetwork(void);

signals:

public slots:

private:
    struct fann *ann;
    struct fann_train_data *data;
    fann_type *calc_out;
};

#endif // CFANNNETWORK_H
