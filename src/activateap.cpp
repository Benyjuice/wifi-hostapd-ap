#include "activateap.h"

ActivateAP::ActivateAP(QObject *parent)
 {
    moveToThread(this);
}

void ActivateAP::run()
{
   QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    char tmp[400], command[150] = "ifconfig ", *temp;
    bool starting = true;    
    int i = 0;
    QString WhoIAm = tr("Activate system");
    QDateTime TimeNow;
    QString dateUNIXNow;


// Последовательность активации:

    qDebug()<< "Start activation.\n";

////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 1: отключить интерфейс
QString temp_QT = GlobalSettings->value("AP/Iface", "wlan0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " down");

    console(command, tmp);
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Preparation of the device has successfully completed"), QString("1")));
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit send_step(tr("Preparation of the device has successfully completed"));
    emit setPercent(10);
    qDebug()<< "Preparation device" << " ... " << "OK";
////////////////////////////////////////////////////////////////////////////////////////////
    sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 2: установить адреса
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
strcpy(command, "ifconfig ");
strcpy(temp, "");
temp_QT = GlobalSettings->value("AP/Iface", "wlan0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " ");
temp_QT = GlobalSettings->value("AP/IP_SERVER", "192.168.0.1").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " netmask ");
strcpy(temp, "");
temp_QT = GlobalSettings->value("AP/MASK", "255.255.255.0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " up");
console(command, tmp);
strcpy(temp, "");
sleep(0.5);

    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Address setting is completed"), QString("1")));
    emit send_step(tr("Address setting is completed"));
    emit setPercent(20);
    qDebug()<< "Check device" << " ... "<< "OK";

}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 3: поднимаем интерфейс
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
strcpy(command, "ifconfig ");
temp_QT = GlobalSettings->value("AP/Iface", "wlan0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, " up");
console(command, tmp);
sleep(1);

/* Проверяем */
CheckProcess CheckInterface(1, GlobalSettings->value("AP/Iface", "wlan0").toString());
if(!CheckInterface.init()) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("The device is not ready for use!"), QString("2")));
    emit send_step(tr("The device is not ready for use!"));
    qDebug()<< "Starting device" << " ... "<< "FAIL";
    starting = false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("The device is ready for use"), QString("1")));
    emit send_step(tr("The device is ready for use"));
    emit setPercent(30);
    qDebug()<<"Starting device"<< " ... " << "OK"; }
}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 4: включаем фовардинг для IPv4
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
console("sysctl -w net.ipv4.ip_forward=\"1\"", tmp);
sleep(0.5);
/* Проверяем */
CheckProcess CheckIPForwarding(2, "");

if(!CheckIPForwarding.init()) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to activate IP Forvard!"), QString("2")));
    qDebug()<<"Enable IP Forwardng"<< " ... "<< "FAIL";
    starting = false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("IP Forward is successfully activated"), QString("1")));
    emit send_step(tr("IP Forward is successfully activated"));
emit setPercent(45);
qDebug()<<"Enable IP Forwardng"<< " ... " << "OK"; }
}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 5: включаем NAT
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
strcpy(command, "iptables -t nat -A POSTROUTING -o  ");
temp_QT = GlobalSettings->value("DHCP/Internet_iface", "eth0").toString().toLocal8Bit();
temp = temp_QT.toAscii().data();
strcat(command, temp);
strcat(command, "  -j MASQUERADE");
sleep(0.5);
system(command);

    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("NAT is successfully activated"), QString("1")));
    emit send_step(tr("NAT is successfully activated"));
    emit setPercent(65);
    qDebug()<<"Set rules to IPTABLES"<< " ... " << "OK";
}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 6: поднимаем DNSMASQ
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {

    system(QString(Platform.CommandToStartDNSMASQ).toLocal8Bit().data());

/* Проверяем */
sleep(3);
CheckProcess CheckDNSMASQ("dnsmasq");
sleep(0.1);

if(!CheckDNSMASQ.init()) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to start DNSMASQ!"), QString("2")));
    emit send_step(tr("Failed to start DNSMASQ!"));
    qDebug()<< "Starting DNSMASQ" << " ... "<< "FAIL";
    starting=false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("DNSMASQ successfully launched"), QString("1")));
    emit send_step(tr("DNSMASQ successfully launched"));
    emit setPercent(80);
    qDebug()<< "Starting DNSMASQ" << " ... "<< "OK";
}
}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 7: поднимаем HOSTAPD
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
    system(QString(Platform.CommandToStartHostapd).toLocal8Bit().data());
sleep(4);
/* Проверяем */
CheckProcess CheckHostapd("hostapd");
sleep(0.1);

if(!CheckHostapd.init()) {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
     emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Failed to start Hostapd!"), QString("2")));
    emit send_step(tr("Failed to start Hostapd!"));
    qDebug()<<command<< " ... "<< "FAIL";
    starting = false;
}
else {
    dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, tr("Hostapd successfully launched"), QString("1")));
    emit send_step(tr("Hostapd successfully launched"));
    emit setPercent(95);
    qDebug()<<command<< " ... " << "OK"; }
}
////////////////////////////////////////////////////////////////////////////////////////////
sleep(0.5);
////////////////////////////////////////////////////////////////////////////////////////////
// Шаг 8: проверка и разбор полёта
////////////////////////////////////////////////////////////////////////////////////////////
if(starting==true) {
// Это означает, что все шаги выполнены и точка доступа запущена
 qDebug()<< tr("AP is running!\n");
 emit setPercent(100);
 emit send_step(tr("AP successfully activated"));
 emit reset_status_active();
} else {
// А это означает, что точка доступа не запущена!
qDebug()<< tr("AP is NOT running!\n");
emit reset_status_active();
}
////////////////////////////////////////////////////////////////////////////////////////////

dateUNIXNow.clear(); dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
emit toLog(QString("%1|%2|%3|%4").arg(WhoIAm, dateUNIXNow, QString(tr("Activation sequence is completed")), QString("0")));
delete GlobalSettings;


exec();
}

void ActivateAP::console(const char cmd[], char *ret) {

    char buf[BUFSIZ];
    FILE *ptr;

    if ((ptr = popen(cmd, "r")) != NULL)
        while (fgets(buf, BUFSIZ, ptr) != NULL)
            strcat(ret, buf);
    pclose(ptr);
}
