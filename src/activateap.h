#ifndef ACTIVATEAP_H
#define ACTIVATEAP_H

#include <QObject>
#include <QThread>
#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QString>
#include <cstdio>
#include <QDebug>
#include "CheckProcess.cpp"
#include "PlatformVariable.cpp"

class ActivateAP : public QThread
{
    Q_OBJECT

    public:
        ActivateAP(QObject *parent = 0);
        void console(const char cmd[], char *ret);
    protected:
        void run();

    signals:
        void reset_status_active();
        void setPercent(const int);
        void send_step(const QString);
        void toLog(const QString);

    private:
        PlatformVariable Platform;
};

#endif // ACTIVATEAP_H
