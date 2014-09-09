#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include<QtTest/QTestEventList>
#include<QTimer>

class NetworkEmulator;
struct CollectedStatistics;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addAdapterInterface(QString interfaceDescription);

private slots:
    void startButtonClick();
    void refreshInterfaceListButtonClick();
    void lossRateChanged();
    void bandWidthChanged();
    void receiveStatistics(CollectedStatistics* stats);
    void autoMouseClickStart();

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    NetworkEmulator* networkBridge;
    bool started;
    QTestEventList *eventList;
};

#endif // MAINWINDOW_H
