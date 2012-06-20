#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "CFannNetwork.h"

namespace Ui {
    class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private:
    Ui::CMainWindow *ui;
    QStringList loadExpression(QString file);
    CFannNetwork *network;

private slots:
    void slotBuildNetwork();
    void slotTestNetwork();
};

#endif // CMAINWINDOW_H
