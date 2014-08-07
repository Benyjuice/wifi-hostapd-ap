#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aboutwindow.h"
#include "optionswindow.h"
#include "activateap.h"
#include "editorconfig.h"
#include <QTextCodec>
#include <QDebug>
#include "disactiveap.h"
#include <QTimer>
#include <QTime>
#include <QSettings>
#include "dialogindicator.h"
#include "getroot.h"
#include <QStringList>
#include <QFile>
#include <QDir>
#include "logsystem.h"
#include <QTextIStream>
#include "iostream"
#include <QDesktopServices>
#include <QUrl>
#include "CheckProcess.cpp"
#include "PlatformVariable.cpp"
#include <QAction>
#include <QMenu>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QIcon iconReturn(QString name);
    void FirsStartDetector();

public slots:
    void FromIndicator();
    void SetTrafficInformation();
    void GetSI(int N);
    void LogSystemAppend( QString );

private slots:
    void CheckUbuntuPrecsisNM();

    void CheckHostapdInitdScript();

    void ClientPriseUpdate();

    void on_Activate_clicked();

    void on_AboutW_activated();

    void on_OptW_activated();

    void console( char cmd[], char *ret);

    void checkStatus();

    void on_Disactive_clicked();

    void on_EditorW_activated();

    void TrafficUpdate();

    void on_Log_activated();
private:
Ui::MainWindow *ui;
PlatformVariable Platform;

};

#endif // MAINWINDOW_H
