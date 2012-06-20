#include "CMainwindow.h"
#include "ui_CMainwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    connect(ui->buildNetwork,SIGNAL(clicked()),this,SLOT(slotBuildNetwork()));
    connect(ui->testNetwork,SIGNAL(clicked()),this,SLOT(slotTestNetwork()));
    network = new CFannNetwork();
}

CMainWindow::~CMainWindow()
{
    delete network;
    delete ui;
}

void CMainWindow::slotBuildNetwork()
{
    ui->outputText->insertPlainText(tr("Starting working on datas.\n"));
    network->setTrainFile(QString("D:\\usr\\fcr\\QMWEStatistic\\data\\data_MWE_symetric.txt"));
    float result = network->buildNetwork();
    ui->outputText->insertPlainText(tr("Fann build network result is %1.\n").arg(result));
    ui->outputText->insertPlainText(tr("Saving network.\n"));
    network->saveNetwork(QString("D:\\usr\\fcr\\QMWEStatistic\\data\\MWE.net"));
    ui->outputText->insertPlainText(tr("Cleanup.\n"));
}

void CMainWindow::slotTestNetwork()
{
    QStringList expressions = loadExpression("D:\\usr\\fcr\\QMWEStatistic\\data\\data_exp.txt");
    network->setTestFile(QString("D:\\usr\\fcr\\QMWEStatistic\\data\\data_MWE_symetric.txt"));
    ui->outputText->insertPlainText(tr("Loading network.\n"));
    network->loadNetwork(QString("D:\\usr\\fcr\\QMWEStatistic\\data\\MWE.net"));
    QList<CFannNetwork::record> results = network->testNetwork();
    for (int i=0; i < results.count(); i++ ) {
        CFannNetwork::record result = results.at(i);
        if (result.difference>1.0) {
            ui->outputText->insertPlainText(tr("Line (%1) with expression %2 give result %3 but should be %4\n")
                                            .arg(result.line)
                                            .arg(expressions[result.line])
                                            .arg(result.output)
                                            .arg(result.want));
        }
    }
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
