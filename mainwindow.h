#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QDesktopWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QScreen>

#include <stdio.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGridLayout *layout;
    long unsigned getWorkspaceCount();
    void setThumbnail(int i);
    int getWorkspace();
    void setWorkspace(int i);
    QLabel *thumbnails[20] = {};
};

#endif // MAINWINDOW_H
