#ifndef _CheckProcess_
#define _CheckProcess_

#include <stdio.h>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QRegExp>

class CheckProcess {
private:
int pidproc;
int Doing;
QString name;
bool Status;
bool debugThisClass;

public:
    CheckProcess() { pidproc = -1; Status = false; Doing = -1; }
    CheckProcess(int a)
    {   pidproc = a; Status = false;  Doing = -1;  }
    CheckProcess(QString t)
    {   name = t; Status = false; pidproc = -1;  Doing = -1; }
    CheckProcess(int b, QString c)
    { name = c;  Doing = b; Status = false; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
    void GetWhichPID() {
        QString temp = QString::number(pidproc);
        Status = false;
        QString fileLine;
        QRegExp nameReg(name);
        QRegExp exitState("Name");

        QFile StateFile(QString("/proc/%1/status").arg(temp));

        if(debugThisClass) qDebug() << "ФАЙЛ : " << QString("/proc/%1/status").arg(temp);

        if(StateFile.open(QFile::ReadOnly)) { // 0
            if(debugThisClass) qDebug() << "Открыт файл";

         // Теперь получаем строку, делим её, отбрасываем пробелы и смотрим имя процесса
        // если имя совпало с тем, что хранится у нас в классе, то возвращаем true

            while(StateFile.atEnd()) { // 1.0
            fileLine = StateFile.readLine(1024);

            if(fileLine.contains(nameReg)>0)
                    Status = true;

            if(fileLine.contains(exitState)>0)
                    break;

            } // 1.0

} // 0
        StateFile.close();
} // GetWhichPID
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    void getPIDWhichName() {
        QString path;
        QString PIDString;
    /////////////////////////////////////////////////////
        if(name == "hostapd") {
            path = "/var/run/hostapd.pid";
            if(debugThisClass) qDebug() << "Проверяем Hostapd";
        }
        else  {
            path = "/var/run/dnsmasq/dnsmasq.pid";
            if(debugThisClass) qDebug() << "Проверяем DNSMAQ";
        }
    ///////////////////////////////////////////////////
        QFile FileToRead(path);
        if(FileToRead.open(QFile::ReadOnly)) {
        PIDString = FileToRead.readAll();
        pidproc = PIDString.toInt();
        }
        else {
            Status = false;
            pidproc = -1;
        }
        FileToRead.close();
        if(debugThisClass) qDebug() << "PID  = " << pidproc;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool getStatusIPForwarding() {
// Простой и !!!КРИВОЙ!!! метод, возвращающий статус форвардинга в системе
        bool t = false;
        int p = 0;
        QString test;

        QFile ipforwarding("/proc/sys/net/ipv4/ip_forward");
        if(ipforwarding.open(QFile::ReadOnly)) {

            test = ipforwarding.readAll();
            p = test.toInt();
            if(p==1) t = true;

        }

        ipforwarding.close();
        return t;
    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool getStatusInterface() {
// Метод, возвращающий статус интернет интерфейса в данный момент времени
        QFile PriceInterface("/proc/net/dev_mcast");
        QString line;
        QStringList listArray;
        QStringList listArrayLine;
        int i, l;

        if(PriceInterface.open(QFile::ReadOnly)) {
            QTextStream in(&PriceInterface);
            line = in.read(102400);
            listArray = line.split("\n");
            for(l=0;l<listArray.count();l++) {
                line = listArray.at(l);
                listArrayLine = line.split(" ");
                for(i=0;i<listArrayLine.count();i++)  {
                    if(listArrayLine.at(i)==name) {
                           Status = true;
                           break;
                    }
                }
                if(Status)
                        break;
}
        }
PriceInterface.close();
return Status;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool init() {
    // Инициализация класса - проверяем, что у нас есть (имя или PID) и получаем статус
        // Doing > -1 и есть name, то делаем:
        // Doing  = 1 - Проверяем интерфейс
        // Doing = 2 - проверяем форвардинг
        // Doing == -1, тогда смотрим, если есть PID, то получаем статус по нему
        // если есть имя, но нет PID, то получаем PID pid-файлу и проверяем статус оттуда
    debugThisClass = false;
    Status = false;

        if(Doing> -1) {
            if(Doing==1) {
                  Status = getStatusInterface();
                  if(debugThisClass) qDebug() << "Проверяем интерфейс...";
            }
            if(Doing==2) {
                Status = getStatusIPForwarding();
                if(debugThisClass) qDebug() << "Проверяем IPForwarding...";
            }
            }
        else {
                    if(pidproc>0) {
                    GetWhichPID();
                    if(debugThisClass) qDebug() << "Получаем статус по PID... ";
                    }
                    else if(name.length() > 0) {
                            getPIDWhichName();
                            if(debugThisClass) qDebug() << "Получаем PID по имени (" << name << ")";
                            if(pidproc>0) { GetWhichPID();
                             if(debugThisClass) qDebug() << "А теперь получаем статус по PID";
                            }
                    }
        }

return Status;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////

};
#endif
