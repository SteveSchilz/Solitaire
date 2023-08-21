#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Forward Declarations
class CardStack;
class myScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    myScene *mScene;
    CardStack *mHearts;
    CardStack *mDiamonds;
    CardStack *mClubs;
    CardStack *mSpades;
};
#endif // MAINWINDOW_H
