#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "CFannNetwork.h"
#include "CPlotter.h"
#include "dialognewproject.h"
#include "CDialogSettings.h"

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
    CPlotter *plotter;
    QString fannFile;
    QString labelInFile;
    QString labelOutFile;
    QString SequenceFile;
    QString activationLearnFile;
    QString activationTestFile;
    QString currentProject;
    void initializeGraphic(int y);
    int fannIn;
    int fannOut;

private slots:
    void slotBuildNetwork();
    void slotTestNetwork();
    void slotLoadProject();
    void slotLoadNewProject(QString val);
    void slotNewProject();
    void slotCloseProject();
    void slotSettings();
    void slotErrorPercentChanged(int val);
    void slotErrorLevelChanged(int val);
};

#endif // CMAINWINDOW_H
