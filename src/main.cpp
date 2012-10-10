#include <QtGui/QApplication>
#include "CMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("QMWEStatistic");
    QCoreApplication::setOrganizationDomain("lilisoft.com");
    QCoreApplication::setApplicationName("Neural Network Datas Tester");
    CMainWindow w;
    w.show();

    return a.exec();
}
