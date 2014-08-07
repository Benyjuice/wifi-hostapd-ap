#include "disactiveap.h"


DisActiveAP::DisActiveAP(QObject *parent)
{
    moveToThread(this);
}

void DisActiveAP::run()
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    char tmp[400],  command[150] = "ifconfig ", *temp;
    bool starting = true;
    QString temp_QT;
    QDateTime TimeNow;
    QString dateUNIXNow;
    QString WhoIAm = tr("Disactivate system");
    bool StatusTest = false;
    int step = 0;
// Последовательность дезактивации:

    qDebug()<< "Start disactivation.\n";

    if(starting) {
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 1: отключить интерфейс
temp_QT = GlobalSettings->value("AP/Iface", "wlan0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " down");
console(command, tmp);

    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("The device is successfully disabled"), QString("1")));
    emit send_step(tr("The device is successfully disabled"));
    emit setPercent(25);
    qDebug()<<"Stoping device "<< " ... " << "OK";
    }
////////////////////////////////////////////////////////////////////////////////////////////

        if(starting) {
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 2: отключить DNSMASQ
            CheckProcess CheckDNSMASQ("dnsmasq");
            StatusTest = CheckDNSMASQ.init();
            step = 0;
            if(StatusTest) {
                while(true) {
                    system(QString(Platform.CommandToStoptDNSMASQ).toLocal8Bit().data());
                    sleep(1);
                    StatusTest = CheckDNSMASQ.init();

                    if(!StatusTest || step >2)
                        break;
                    step++;
                }
            }
StatusTest = CheckDNSMASQ.init();

if(StatusTest) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to disable the DNSMASQ!"), QString("2")));
    emit send_step(tr("Failed to disable the DNSMASQ!"));
    qDebug()<<"Stoping DNSMASQ "<< " ... "<< "FAIL";
    starting = false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("DNSMASQ is disabled successfully"), QString("1")));
    emit send_step(tr("DNSMASQ is disabled successfully"));
    emit setPercent(50);
    qDebug()<<"Stoping DNSMASQ "<< " ... "<< "OK";
        }
        }
////////////////////////////////////////////////////////////////////////////////////////////

            if(starting) {
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 3: отключить HOSTAPD
////////////////////////////////////////////////////////////////////////////////////////////

                StatusTest = false;
                CheckProcess CheckHostapd("hostapd");
                StatusTest = CheckHostapd.init();
                step = 0;
                if(StatusTest) {
                    while(true) {
                        system(QString(Platform.CommandToStoptHostapd).toLocal8Bit().data());
                        sleep(2);
                        StatusTest = CheckHostapd.init();

                        if(!StatusTest || step>4)
                            break;
                        step++;
                    }
                }

StatusTest = CheckHostapd.init();

if(StatusTest) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to disable the Hostapd!"), QString("2")));
    emit send_step(tr("Failed to disable the Hostapd!"));
    qDebug()<<"Stoping Hostapd "<< " ... "<< "FAIL";
    starting = false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Hostapd is disabled successfully"), QString("1")));
    emit send_step(tr("Hostapd is disabled successfully"));
    emit setPercent(75);
    qDebug()<<"Stoping Hostapd "<< " ... "<< "OK";
}
            }
////////////////////////////////////////////////////////////////////////////////////////////

    if(starting) {
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 4: отключаем форвардинг
////////////////////////////////////////////////////////////////////////////////////////////
console("sysctl -w net.ipv4.ip_forward=\"0\"", tmp);

// Проверяем
CheckProcess CheckIPForwarding(2, "");

if(CheckIPForwarding.init()) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to disable the IP Forward!"), QString("2")));
    emit send_step(tr("Failed to disable the IP Forward!"));
    qDebug()<<"Disabled IPForwarding "<< " ... "<< "FAIL";
    starting = false;

} else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("IP Forward successfully disabled"), QString("1")));
    emit send_step(tr("IP Forward successfully disabled"));
    emit setPercent(95);
    qDebug()<<"Disabled IPForwarding "<< " ... "<< "OK";
}
    }

////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 5: проверка и разбор полёта
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
// Это означает, что все шаги выполнены и точка доступа остановлена
 qDebug()<<"AP is stoped!\n";
 emit setPercent(100);
 sleep(0.5);
 emit send_step(tr("AP successfully deactivated"));
 emit reset_status_disactive();
} else {
 // А это означает, что точка доступа не остановлена!
 qDebug()<<"AP is NOT stoped!\n";
 sleep(0.25);
 emit reset_status_disactive();
 }
////////////////////////////////////////////////////////////////////////////////////////////
dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Shutdown sequence is completed"), QString("0")));

    exec();
    delete GlobalSettings;
}

void DisActiveAP::console(const char cmd[], char *ret) {

    char buf[BUFSIZ];
         FILE *ptr;

         if ((ptr = popen(cmd, "r")) != NULL)
              while (fgets(buf, BUFSIZ, ptr) != NULL)
                  strcat(ret, buf);
                 pclose(ptr);
}
