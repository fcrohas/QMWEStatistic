#include "CMainWindow.h"
#include "ui_CMainWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#ifdef WIN32
#define DATA_PATH "data\\"
#else
#define DATA_PATH "/home/fcr/projects/QMWEStatistic/data/""
#endif

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
  ,currentProject("")
{
    ui->setupUi(this);
    connect(ui->buildNetwork,SIGNAL(clicked()),this,SLOT(slotBuildNetwork()));
    connect(ui->testNetwork,SIGNAL(clicked()),this,SLOT(slotTestNetwork()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(slotNewProject()));
    connect(ui->actionLoad,SIGNAL(triggered()),this,SLOT(slotLoadProject()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(slotCloseProject()));
    connect(ui->errorLevel,SIGNAL(sliderMoved(int)),this,SLOT(slotErrorLevelChanged(int)));
    connect(ui->errorPercent,SIGNAL(sliderMoved(int)),this,SLOT(slotErrorPercentChanged(int)));
    connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(slotSettings()));
    ui->buildNetwork->setEnabled(false);
    ui->testNetwork->setEnabled(false);
    slotErrorLevelChanged(10);
    slotErrorPercentChanged(10);
    network = 0;
    plotter = 0;
}

CMainWindow::~CMainWindow()
{
    if (network)
        delete network;
    if (ui)
        delete ui;
}

void CMainWindow::slotBuildNetwork()
{
    if (currentProject == "")
        return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ui->outputText->clear();
    ui->outputText->insertPlainText(QString("Starting working on datas "+activationLearnFile+".\n"));
    network->setTrainFile(QString(activationLearnFile));
    float result = network->buildNetwork();
    ui->outputText->insertPlainText(QString("Fann build network result is %1.\n").arg(result));
    ui->outputText->insertPlainText(QString("Saving network to "+fannFile+".\n"));
    network->saveNetwork(QString( fannFile));
    ui->outputText->insertPlainText(tr("Cleanup.\n"));
    QApplication::restoreOverrideCursor();
}

void CMainWindow::slotTestNetwork()
{
    if (currentProject == "")
        return;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ui->outputText->clear();
    QStringList expressions = loadExpression(SequenceFile);
    QStringList outCriteria = loadExpression(labelOutFile);
    int selectedOutput = ui->cbCriteriaOut->currentIndex();
    network->setTestFile(QString(activationTestFile));

    ui->outputText->insertPlainText(QString("Loading network "+fannFile+".\n"));
    network->loadNetwork(QString(fannFile));

    QList<CFannNetwork::record> results = network->testNetwork();
    double * error = new double[ results.count() * fannOut]; // result line * ouput per line
    float * errorPerCriteria = new float[fannOut]; // result line is between 0.0 and 2.0
    plotter->setRangeX(0,results.count());
    double errorLevel = ui->errorLevel->value()/10.0;
    for (int i=0; i < results.count(); i++ ) {
        CFannNetwork::record result = results.at(i);
        for (int y=0; y < fannOut;y++) {

             if (result.difference[y]>errorLevel)
                error[y+fannOut*i] = result.difference[y];

             errorPerCriteria[y] = (errorPerCriteria[y] + ((float)result.difference[y]*50))/(2);
        }
        if (result.difference[selectedOutput]>errorLevel) {
        //ui->outputText->insertPlainText(tr("Line with expression ( %1 )").arg(result.difference[y]*50));

            ui->outputText->insertPlainText(tr("Line with expression ( %1 ) give result %2 for %3 but should be %4\n")
                                            .arg(expressions[result.line])
                                            .arg(result.output[selectedOutput])
                                            .arg(outCriteria[selectedOutput])
                                            .arg(result.want[selectedOutput]));

        }

    }

    for (int i=0; i < fannOut; i++) {
        if (errorPerCriteria[i]>ui->errorPercent->value())
            ui->outputText->insertPlainText(tr("For criteria  ( %1 )  error is %2%\n").arg(outCriteria[i]).arg(errorPerCriteria[i]));
       //qDebug() << "For criteria " << outCriteria[i] << " ouput is around " << QString("%1").arg(errorPerCriteria[i]) << " percent\n" ;
    }

    plotter->showSpectrogram(true);
    plotter->setData(error,results.count(),fannOut);
    plotter->qwtPlot->replot();
    QApplication::restoreOverrideCursor();
}

QStringList CMainWindow::loadExpression(QString file)
{
    QStringList listExpression;
    QFile textFile(file);
    if (!textFile.open(QFile::ReadOnly | QFile::Text)) {
                QMessageBox::warning(this, tr("Application"),
                                     tr("Cannot read file %1:\n%2.")
                                     .arg(file)
                                     .arg(textFile.errorString()));
     }

    QTextStream textStream(&textFile);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
            listExpression.append(line);
    }
    return listExpression;
}

void CMainWindow::slotNewProject()
{
    DialogNewProject dialog;
    QString file;
    if (dialog.exec() == QDialog::Accepted) {
        // Save project file
        file = QFileDialog::getSaveFileName(this, tr("Save Project"), "C:\\myproject.prj", tr("Images (*.prj)"));
        dialog.setFileName(file);
        dialog.Save();
        slotLoadNewProject(file);

    }
    dialog.close();
}

void CMainWindow::slotLoadNewProject(QString val = "")
{
    if (val == "") {
        currentProject = QFileDialog::getOpenFileName(this, tr("Project File"), "C:\\", tr("Image Files (*.prj)"));
    } else {
        currentProject = val;
    }

    QSettings settings(currentProject,QSettings::IniFormat);
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    labelInFile = settings.value("Project/LabelInputFile").toString();
    labelOutFile = settings.value("Project/LabelOutputFile").toString();
    activationLearnFile =settings.value("Project/LearnFile").toString();
    activationTestFile = settings.value("Project/TestFile").toString();
    SequenceFile = settings.value("Project/LabelActivationFile").toString();
    fannFile = settings.value("Project/FannFile").toString();

    if (network)
        delete network;
    network = new CFannNetwork();

    network->num_neurons_hidden = settings.value("Settings/hiddenNeurons",100).toInt();
    network->desired_error = settings.value("Settings/desiredError",0).toInt();
    network->num_layers = settings.value("Settings/numberLayers",3).toInt();
    network->max_epochs = settings.value("Settings/maxEpochs",1000).toInt();
    network->epochs_between_reports = settings.value("Settings/epochsReports",10).toInt();
    network->hidden_activation = settings.value("Settings/hiddenFunction",5).toInt();
    network->output_activation = settings.value("Settings/outputFunction",5).toInt();
    network->stop_function = settings.value("Settings/stopFunction",1).toInt();
    network->train_algorithm = settings.value("Settings/trainMethod",2).toInt();

    ui->cbCriteriaOut->clear();
    // Reading Neural network input data number
    QStringList expressions = loadExpression(QString(labelInFile));
    ui->fannIn->setText(QString("%1").arg(expressions.count()));
    fannIn = expressions.count();
    // Reading Neural network output data number
    QStringList outCriteria = loadExpression(QString(labelOutFile));
    ui->cbCriteriaOut->addItems(outCriteria);
    ui->fannOut->setText(QString("%1").arg(outCriteria.count()));
    fannOut = outCriteria.count();

    network->num_input = fannIn;
    network->num_output = fannOut;
    network->initializeNetwork();

    initializeGraphic(fannOut);
    ui->buildNetwork->setEnabled(true);
    ui->testNetwork->setEnabled(true);

    QApplication::restoreOverrideCursor();
}

void CMainWindow::slotLoadProject()
{
    currentProject = QFileDialog::getOpenFileName(this, tr("Project File"), "C:\\", tr("Image Files (*.prj)"));
    if (currentProject=="") {
        return;
    }
    slotLoadNewProject(currentProject);
}

void CMainWindow::initializeGraphic(int y)
{
    if (plotter)
            delete plotter;
    plotter = new CPlotter();
    plotter->setRangeY(0,y);
    plotter->setDataRange(0.0,2.0);
    ui->plotterLayout->setContentsMargins( 0, 0, 0, 0 );
    ui->plotterLayout->addWidget( plotter );
    plotter->qwtPlot->setAxisTitle(QwtPlot::xBottom, "Expression");
    plotter->qwtPlot->setAxisTitle(QwtPlot::yLeft  , "Output possibility");
    plotter->setColorMap(CPlotter::standard);
}

void CMainWindow::slotCloseProject()
{
    close();
}

void CMainWindow::slotErrorLevelChanged(int val)
{
    ui->labelErrorDecision->setText(QString("Decision error level (%1 )").arg(val/10.0));
}

void CMainWindow::slotErrorPercentChanged(int val)
{
    ui->labelErrorCriteria->setText(QString("Error Percent Criteria (%1 %)").arg(val));
}

void CMainWindow::slotSettings()
{
    CDialogSettings dialog;
    dialog.num_neurons_hidden =  network->num_neurons_hidden;
    dialog.desired_error = network->desired_error;
    dialog.num_layers = network->num_layers;
    dialog.max_epochs = network->max_epochs;
    dialog.epochs_between_reports = network->epochs_between_reports;
    dialog.hidden_activation = network->hidden_activation;
    dialog.output_activation = network->output_activation;
    dialog.stop_function = network->stop_function;
    dialog.train_algorithm = network->train_algorithm;
    dialog.Load();
    if (dialog.exec() == QDialog::Accepted) {
        // Save project file
        dialog.setFileName(currentProject);
        dialog.Save();
    }
    dialog.close();
}
