#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <string>
#include <fstream>

#include <iostream>
#include <vector>

namespace Ui {
class MainWindow;
}

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString path();
    void setPath(QString path);
    void setOutPath(QString path);
    
private:
    Ui::MainWindow *ui;

private:
    void addActions();
    void addShortcuts();
    void status(QString path);
    
    QString formatPath(QString path);

private slots:
    void bRunClick();
    void bBrowseInClick();
    void bBrowseOutClick();

};

#endif // MAINWINDOW_H
