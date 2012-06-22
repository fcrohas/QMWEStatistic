#include "CMainWindow.h"
#include "ui_CMainWindow.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#ifdef WIN32
#define DATA_PATH "D:\\usr\\fcr\\QMWEStatistic\\data\\"
#else
#define DATA_PATH "/home/fcr/projects/QMWEStatistic/data/""
#endif

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    connect(ui->buildNetwork,SIGNAL(clicked()),this,SLOT(slotBuildNetwork()));
    connect(ui->testNetwork,SIGNAL(clicked()),this,SLOT(slotTestNetwork()));
    //connect(ui->errorLevel,SIGNAL(changed())),this,SLOT(slotErrorLevelChanged());
    //connect(ui->errorPercent,SIGNAL(changed())),this,SLOT(slotErrorPercentChanged());
    network = new CFannNetwork();

    plotter = new CPlotter();
    plotter->setRangeY(0,141.0);
    ui->plotterLayout->setContentsMargins( 0, 0, 0, 0 );
    ui->plotterLayout->addWidget( plotter );
    plotter->qwtPlot->setAxisTitle(QwtPlot::xBottom, "Expression");
    plotter->qwtPlot->setAxisTitle(QwtPlot::yLeft  , "Output possibility");

    plotter->setColorMap(CPlotter::standard);

}

CMainWindow::~CMainWindow()
{
    delete network;
    delete ui;
}

void CMainWindow::slotBuildNetwork()
{
    ui->outputText->insertPlainText(tr("Starting working on datas.\n"));
    network->setTrainFile(QString(DATA_PATH "data_MWE_symetric.txt"));
    float result = network->buildNetwork();
    ui->outputText->insertPlainText(tr("Fann build network result is %1.\n").arg(result));
    ui->outputText->insertPlainText(tr("Saving network.\n"));
    network->saveNetwork(QString( DATA_PATH "MWE.net"));
    ui->outputText->insertPlainText(tr("Cleanup.\n"));
}

void CMainWindow::slotTestNetwork()
{
    QStringList expressions = loadExpression(DATA_PATH "sequencesTest.txt");
    QStringList outCriteria = loadExpression(DATA_PATH "labelOutCriteria.txt");
    network->setTestFile(QString(DATA_PATH "dataTest_MWE.txt"));

    ui->outputText->insertPlainText(tr("Loading network.\n"));
    network->loadNetwork(QString(DATA_PATH "MWE.net"));

    QList<CFannNetwork::record> results = network->testNetwork();
    double * error = new double[ results.count() * 142]; // result line * ouput per line
    float * errorPerCriteria = new float[142]; // result line is between 0.0 and 2.0
    plotter->setRangeX(0,results.count());
    for (int i=0; i < results.count(); i++ ) {
        CFannNetwork::record result = results.at(i);
        for (int y=0; y < 142;y++) {

             if (result.difference[y]>1.0)
                error[y+142*i] = result.difference[y];

             errorPerCriteria[y] = (errorPerCriteria[y] + ((float)result.difference[y]*50))/(2);
        }
        if (result.difference[58]>1.0) {
        //ui->outputText->insertPlainText(tr("Line with expression ( %1 )").arg(result.difference[y]*50));

            ui->outputText->insertPlainText(tr("Line with expression ( %1 ) give result %2 for %3 but should be %4\n")
                                            .arg(expressions[result.line])
                                            .arg(result.output[58])
                                            .arg(outCriteria[58])
                                            .arg(result.want[58]));

        }

    }

    for (int i=0; i < 142; i++) {
        if (errorPerCriteria[i]>10.0)
            ui->outputText->insertPlainText(tr("For criteria  ( %1 )  error is %2%\n").arg(outCriteria[i]).arg(errorPerCriteria[i]));
       //qDebug() << "For criteria " << outCriteria[i] << " ouput is around " << QString("%1").arg(errorPerCriteria[i]) << " percent\n" ;
    }

    plotter->showSpectrogram(true);
    plotter->setData(error,results.count(),142);
    plotter->qwtPlot->replot();
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
