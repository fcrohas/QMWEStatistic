#ifndef DIALOGNEWPROJECT_H
#define DIALOGNEWPROJECT_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>

namespace Ui {
class DialogNewProject;
}

class DialogNewProject : public QDialog
{
    Q_OBJECT
    
public:
    void setFileName(QString file);
    explicit DialogNewProject(QWidget *parent = 0);
    ~DialogNewProject();
    void Save();

private:
    Ui::DialogNewProject *ui;
    QString browseFolder();
    QString file;

private slots:
    void slotBrowseInput();
    void slotBrowseOutput();
    void slotBrowseFann();
    void slotBrowseLearn();
    void slotBrowseTest();
    void slotBrowseSequence();
};

#endif // DIALOGNEWPROJECT_H
