#ifndef DISACTIVEAP_H
#define DISACTIVEAP_H
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

class DisActiveAP : public QThread
{
    Q_OBJECT

    public:
        DisActiveAP(QObject *parent = 0);
        void console(const char cmd[], char *ret);
    protected:
        void run();

    signals:
        void reset_status_disactive();
        void setPercent(const int);
        void send_step(const QString);
        void toLog(const QString);

    private:
            PlatformVariable Platform;
};

#endif // DISACTIVEAP_H
