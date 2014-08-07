#include "mainwindow.h"
#include "ui_mainwindow.h"

///////////////////////////////////////////////////////////////////////////////
int Status = 0; // Статус точки доступа
QString ProgrammVersion = "1.1.1"; // Версия программы

unsigned long long GlobalTrafficIn = 0; // Входящий траффик новый
unsigned long long GlobalTrafficOut = 0; // Исходящий траффик новый

unsigned long long GlobalTrafficInOld = 0; // Входищий траффик старый
unsigned long long GlobalTrafficOutOld = 0; // Исходящий траффик старый

unsigned long long GlobalTrafficInSpeedINT = 0; // Входящая скорость в битах
unsigned long long GlobalTrafficOutSpeedINT = 0; // Исходящая скорость в битах

unsigned long long GlobalTrafficInMaxSpeedINT = 0; // Максимальная входящая скорость в Битах
unsigned long long GlobalTrafficOutMaxSpeedINT = 0; // Максимальная исходящая скорость в Битах

float GlobalTrafficInFloat = 0.1; // Входящий траффик в конечном результате
float GlobalTrafficOutFloat = 0.1; // Исходящий траффик в конечном результате

float GlobalTrafficInSpeed = 0.0; // Входящая скорость
float GlobalTrafficOutSpeed = 0.0; // Исходящая скорость

float GlobalTrafficInMaxSpeed = 0.0; // Максимальная входящая скорость
float GlobalTrafficOutMaxSpeed = 0.0; // Максимальная исходяшая скорость

QString InSI = "Byte"; // единицы измерения входящего траффика
QString OutSI = "Byte"; // единицы измерения сиходящего траффика

QString InSISpeed = "Byte"; // Единицы измерения входящей скорости
QString OutSISpeed = "Byte"; // Единицы измерения исходящей скорости

QString InSIMaxSSpeed = "Byte"; // Единицы измерения максимальной входящей скорости
QString OutSIMaxSpeed = "Byte"; // Единицы измерения максимальной исходящей скорости
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
    ui->setupUi(this);

    if(GlobalSettings->value("Programm/version", "").toString() != ProgrammVersion)
            GlobalSettings->setValue("Programm/version", ProgrammVersion); // Обновляем версию в конфиге
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(Platform.DisableDNSMASQinUbuntuPreciseInNM)
        CheckUbuntuPrecsisNM(); // Проверяем, не Ubuntu 12.04 ли это, где у меня забрали управление DNSMASQ?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(Platform.CheckHOSTAPDscript)
        CheckHostapdInitdScript(); // Проверяем, не Ubuntu ли это, где поломали скрипт запуска Hostapd?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    FirsStartDetector(); // Проверяем, не первый ли это запуск это программы? Может надо восстановить конфиги?
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QDateTime TimeNow;
    QString dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("WiFi Hostapd AP"), dateUNIXNow, tr("Programm loaded and ready"), QString("0")));
    checkStatus();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TrafficUpdate()));
    timer->start(GlobalSettings->value("Programm/UpdateTraffic", 1).toInt() * 1000);

    QTimer *ClientUpdate = new QTimer(this);
    connect(ClientUpdate, SIGNAL(timeout()), this, SLOT(ClientPriseUpdate()));
    ClientUpdate->start(GlobalSettings->value("Programm/ClientsPriceUpdateTime", 5).toInt() * 1000);
    // иконки?

    ui->Activate->setIcon(iconReturn("system-run"));
    ui->Disactive->setIcon(iconReturn("dialog-close"));
    ui->Exit->setIcon(iconReturn("application-exit"));
    ui->OptW->setIcon(iconReturn("configure"));
    ui->Log->setIcon(iconReturn("applications-system"));
    ui->EditorW->setIcon(iconReturn("document-edit"));
    ui->AboutW->setIcon(iconReturn("dialog-information"));
    delete GlobalSettings;
}

MainWindow::~MainWindow()
{
    QDateTime TimeNow;
    QString dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("WiFi Hostapd AP"), dateUNIXNow, tr("Programm closed"), QString("0")));
    delete ui;
}

void MainWindow::FirsStartDetector() {
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
QDateTime TimeNow;
QString dateUNIXNow;

if(GlobalSettings->value("Programm/FirstLoad", true).toBool()) {
    OptionsWindow options;
    options.on_APSave_clicked(true);
    options.on_DHCPSave_clicked(true);

dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("Configs created succesfully."), QString("1")));
GlobalSettings->setValue("Programm/FirstLoad", false);
}
delete GlobalSettings;
}

void MainWindow::ClientPriseUpdate() {
// Слот обновления списка клиентов через заданный промежуток времени
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
QFile leaseIP("/var/lib/misc/dnsmasq.leases");
QString LineRead;
QStringList fields;
QStringList horizontal;
QStringList vertical;
QTableWidgetItem *item = 0;
QDateTime *date = 0;
int i = 0;
int f = 1;
int timeconnect;

horizontal << tr("Name")<<tr("IP") << tr("Connected in") << tr("Device") << tr("Rent to");
ui->Clients->setHorizontalHeaderLabels(horizontal);
ui->Clients->verticalHeader()->setVisible(true);

if(leaseIP.open(QIODevice::ReadOnly)) {
// файл успешно открыт. Теперь читаем построчно
ui->Clients->setRowCount(0);

    while(!leaseIP.atEnd()) { // пока не достигнут конец файла
    LineRead = leaseIP.readLine();
    fields = LineRead.split(" ");
    if(fields.size()>0) {
     // Если эта строка не пуста, то заполняем поля в виджите
    ui->Clients->setRowCount(ui->Clients->rowCount()+1);
    vertical << QString("%1").arg(f);
    f++;
    ui->Clients->setVerticalHeaderLabels(vertical);
    item = new QTableWidgetItem(fields.at(3));
    ui->Clients->setItem(i,0,item);//Name

    item = new QTableWidgetItem(fields.at(2));
    ui->Clients->setItem(i,1,item);//IP

    timeconnect = fields.at(0).toInt()+12*60*60;
    date = new QDateTime;
    date->setTime_t(timeconnect);
    item = new QTableWidgetItem(date->toString("hh:mm:ss"));
    ui->Clients->setItem(i,2,item);

    item = new QTableWidgetItem(fields.at(1));
    ui->Clients->setItem(i,3,item);

    timeconnect = fields.at(0).toInt()+12*60*60;

    switch(GlobalSettings->value("DHCP/IP_time", 2).toInt()) {
        case(0): timeconnect += 10*60; break;
        case(1): timeconnect += 30*60; break;
        case(2): timeconnect += 60*60; break;
        case(3): timeconnect += 120*60; break;
        case(4): timeconnect += 6*60*60; break;
        case(5): timeconnect += 12*60*60;break;
    }

    date = new QDateTime;
    date->setTime_t(timeconnect);
    item = new QTableWidgetItem(date->toString("hh:mm:ss"));
    ui->Clients->setItem(i,4,item);

    i++;
} // если строка пуста, чтож... очищаем список и идём дальше
fields.clear();
}
} // не удалось открыть файл
leaseIP.close();
delete GlobalSettings;
}

void MainWindow::TrafficUpdate() {
QString tempt_QT;
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
QString line;
QStringList Input;
QStringList Elements;
int i = 0;
tempt_QT = GlobalSettings->value("AP/Iface", "wlan0").toString().toLocal8Bit();
ui->TrafficTable->horizontalHeader()->setVisible(true);

QFile file_to_open("/proc/net/dev");
file_to_open.open(QIODevice::ReadOnly);

if(file_to_open.isOpen()) {

QTextStream in(&file_to_open);
line = in.read(102400);
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");
line.replace("  ", " ");

Input = line.split("\n");
line.clear();

for(i=0;i<Input.count();i++) {
 // Проверяем, чтобы строка не начиналась с пробела
 if(Input.at(i).startsWith(" "))
     line = Input.at(i).mid(1, Input.at(i).length()-1);
 else
     line = Input.at(i);

 // проверяем, относится ли данная строка к нашему интерфейсу
 if(line.startsWith(tempt_QT)) {
 // Да, относится
 Elements = line.split(" ");

 // Меняем местами траффик
 GlobalTrafficInOld = GlobalTrafficIn;
 GlobalTrafficOutOld = GlobalTrafficOut;

 // Устанавливаем новый траффик
 GlobalTrafficIn = Elements.at(1).toLongLong();
 GlobalTrafficOut = Elements.at(9).toLongLong();
 }
}

} else { qDebug() <<"error!"; }

file_to_open.close();


SetTrafficInformation();

delete GlobalSettings;
}

void MainWindow::SetTrafficInformation() {
// Эта функция создана для расчёта траффика
// Во-первых, она устанавливает значения траффика в окне
// Во-вторых, подсчитывает среднюю скорость
// В-третьих, в будущем будет писать в логи траффик за сеанс
// В-четвертых, будет рисовать граффики, или не будет, не знаю пока...
////////////////////////////////////////////////////////////////////
// Считаем траффик
////////////////////////////////////////////////////////////////////
QString Temp;
QTableWidgetItem *item = 0;
QString a;
int s=0, len=0;
////////////////////////////////////////////////////////////////////
// Вызываем функции
////////////////////////////////////////////////////////////////////
GetSI(0); // Входящий траффик
GetSI(1); // Исходящий траффик

GetSI(2); // Входящая скорость
GetSI(3); // Исходящая скорость

GetSI(4); // Входящая максимальная скорость
GetSI(5); // Исходящая максимальная скорость
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 0
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficInFloat);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(InSI);
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(0,0,item);
a.clear();
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// 1
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficOutFloat);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(OutSI);
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(0,1,item);
a.clear();
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 2
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficInSpeed);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(InSISpeed);
a.append(tr("/S"));
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(1,0,item);
a.clear();
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// 3
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficOutSpeed);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(OutSISpeed);
a.append(tr("/S"));
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(1,1,item);
a.clear();
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// 4
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficInMaxSpeed);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(InSIMaxSSpeed);
a.append(tr("/S"));
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(2,0,item);
a.clear();
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// 5
////////////////////////////////////////////////////////////////////
a = QString("%1").arg(GlobalTrafficOutMaxSpeed);
s = a.lastIndexOf(".");
len = a.length()-2;
if(s <= len) { len = a.lastIndexOf(".")+3; a = a.mid(0, len); }
a.append(" ");
a.append(OutSIMaxSpeed);
a.append(tr("/S"));
item = new QTableWidgetItem(a);
ui->TrafficTable->setItem(2,1,item);
a.clear();
////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////
// Конец этой функции
////////////////////////////////////////////////////////////////////
}

void MainWindow::GetSI(int N) {
// Эта функция позвращает единицы измерения и цифру
float PP;
float Test = 0;
QString Label;

if(N==0) {
// Имеем дело со входищим траффиком
PP = GlobalTrafficIn; }
else if(N==1) {
// Имеем дело с исходящим траффиком
PP = GlobalTrafficOut; }
else if(N==2) {
// Имеем дело с входящей скоростью
PP = GlobalTrafficIn - GlobalTrafficInOld;
GlobalTrafficInSpeedINT = GlobalTrafficIn - GlobalTrafficInOld;
}
else if(N==3) {
// Имеем дело с исходяшей скоростью
PP = GlobalTrafficOut - GlobalTrafficOutOld;
GlobalTrafficOutSpeedINT = GlobalTrafficOut - GlobalTrafficOutOld;
}
else if(N==4) {
// Имеем дело с максимальной входящей скоростью
// Если нынешняя скорость больше максимальной, то делаем нынешнюю максимальной
    if(GlobalTrafficInMaxSpeedINT<=GlobalTrafficInSpeedINT && GlobalTrafficInOld>1) { GlobalTrafficInMaxSpeedINT = GlobalTrafficInSpeedINT; }
    PP = GlobalTrafficInMaxSpeedINT;
}
else if(N==5) {
// Имеем дело с максимальной исходящей скоростью
// Если нынешняя скорость больше максимальной, то делаем нынешнюю максимальной
    if(GlobalTrafficOutMaxSpeedINT<=GlobalTrafficOutSpeedINT && GlobalTrafficOutOld>1) { GlobalTrafficOutMaxSpeedINT = GlobalTrafficOutSpeedINT; }
    PP = GlobalTrafficOutMaxSpeedINT;
}

////////////////////////////////////////////////////
// Получаем обозначения и значения
////////////////////////////////////////////////////
Label = tr("Byte");

Test = PP / 1024.0;
if(Test>=0.98) {
// Следующая единица измерения
Label = tr("KB");
PP = Test;
}

Test = PP / (1024.0);
if(Test>=0.98) {
// Следующая единица измерения
Label = tr("MB");
PP = Test;
}

Test = PP / (1024.0);
if(Test>=0.98) {
// Следующая единица измерения
Label = tr("GB");
PP = Test;
}

Test = PP / (1024.0);
if(Test>=0.98) {
// Следующая единица измерения
Label = tr("TB");
PP = Test;
}

//////////////////////////////////////////////////
// Возвращаем значения назад в глобальные переменные
//////////////////////////////////////////////////
if(N==0) {
// Имеем дело со входищим траффиком
GlobalTrafficInFloat = PP;
InSI = Label; }

else if(N==1) {
// Имеем дело с исходящим траффиком
    GlobalTrafficOutFloat = PP;
    OutSI = Label;
}
else if(N==2) {
// Имеем дело с входящей скоростью
GlobalTrafficInSpeed = PP;
InSISpeed = Label;
}
else if(N==3) {
// Имеем дело с исходяшей скоростью
GlobalTrafficOutSpeed = PP;
OutSISpeed = Label;
}
else if(N==4) {
// Имеем дело с максимальной входящей скоростью
GlobalTrafficInMaxSpeed = PP;
InSIMaxSSpeed = Label;
}
else if(N==5) {
// Имеем дело с максимальной исходящей скоростью
GlobalTrafficOutMaxSpeed = PP;
OutSIMaxSpeed = Label;
}
}

void MainWindow::on_Activate_clicked()
{
// Запустить активацию точки доступа

    DialogIndicator ActForm;
    ActForm.SetDo(0); // активируем
    ActForm.setWindowTitle(tr("Working..."));
    connect(&ActForm, SIGNAL(to_Mainwindow()), this, SLOT(FromIndicator()));
    connect(&ActForm, SIGNAL(toLogInMainWindow(QString)), this, SLOT(LogSystemAppend(QString)));
    ui->Disactive->setEnabled(false);
    ui->Activate->setEnabled(false);
    ActForm.exec();
}

void MainWindow::on_Disactive_clicked()
{
// Запустить дезактивацию точки доступа

    DialogIndicator ActForm;
    ActForm.SetDo(1); // Диактивируем
    ActForm.setWindowTitle(tr("Working..."));
    connect(&ActForm, SIGNAL(to_Mainwindow()), this, SLOT(FromIndicator()));
    connect(&ActForm, SIGNAL(toLogInMainWindow(QString)), this, SLOT(LogSystemAppend(QString)));
    ui->Disactive->setEnabled(false);
    ui->Activate->setEnabled(false);

    ActForm.exec();
}

void MainWindow::on_AboutW_activated()
{
   // Показать окно "О программе"
   AboutWindow about;
   about.exec();
}

void MainWindow::on_OptW_activated()
{
    OptionsWindow OptW;
    connect(&OptW, SIGNAL(toMainLog(QString)), this, SLOT(LogSystemAppend(QString)));
    OptW.exec();
}

void MainWindow::console( char cmd[], char *ret) {

    char buf[BUFSIZ];
         FILE *ptr;

         if ((ptr = popen(cmd, "r")) != NULL)
              while (fgets(buf, BUFSIZ, ptr) != NULL)
                  strcat(ret, buf);
                 pclose(ptr);
}

void MainWindow::checkStatus() {
    int a[4] = {0,0,0,0};
    int tempI = 0, i=0, tempI2 = 0;
    QDateTime TimeNow;
    QString dateUNIXNow;
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    bool starting = true;
    bool test;
    QString temp_QT;
    qDebug() << "Testing AP... ";
    ////////////////////////////////////////////////////////////////////////////////////////////
    // Активирована ли точка доступа?
    ////////////////////////////////////////////////////////////////////////////////////////////
    // точка доступа активирована, если
    // 1. Статус  устройства
    ////////////////////////////////////////////////////////////////////////////////////////////
    CheckProcess IFace(1,GlobalSettings->value("AP/Iface", "wlan0").toString());
    if(!IFace.init()) { starting=false; a[0]=0; qDebug() << "DEVICE - " << "OFF"; }
    else { qDebug() << "DEVICE - " << "ON"; a[0]=1; }

// 2. Статус DNSMASQ
 ////////////////////////////////////////////////////////////////////////////////////////////
        CheckProcess DNSMASQcheck("dnsmasq");
        test = DNSMASQcheck.init();
        if(test) { qDebug() << "DNSMASQ - " << "ON"; a[1]=1; }
        else { starting=false; a[1]=0; qDebug() << "DNSMASQ - " << "OFF"; }


// 3. Статус Hostapd
    ////////////////////////////////////////////////////////////////////////////////////////////
    CheckProcess HOSTAPDcheck("hostapd");
    test = HOSTAPDcheck.init();
    if(!test) { starting=false; a[2]=0; qDebug() << "HOSTAPD - " << "OFF"; }
        else { qDebug() << "HOSTAPD - " << "ON"; a[2]=1; }

// 4. Статус IP Forwarding
    ////////////////////////////////////////////////////////////////////////////////////////////
    CheckProcess IPForwarding(2, "");
    if(!IPForwarding.init())   { starting=false; a[3]=0; qDebug() << "IP FORWARDING - " << "OFF"; }
    else { qDebug() << "IP FORWARDING - " << "ON"; a[3]=1; }

    ////////////////////////////////////////////////////////////////////////////////////////////
    if(starting==true) {
        ui->identificator->setText(tr("<h3><font color=\"green\">Активировано</font></h3>"));
        ui->StatusIcon->setPixmap(QPixmap(":/pic/icons/dialog-ok-apply.png"));
        ui->Activate->setEnabled(false);
        ui->Disactive->setEnabled(true);
        qDebug() << "AP IS RUNNING \n";

        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, tr("AP online"), QString("0")));

    } else {
        ui->identificator->setText(tr("<h3><font color=\"red\">Отключена</font></h3>"));
        ui->StatusIcon->setPixmap(QPixmap(":/pic/icons/dialog-close.png"));
        ui->Activate->setEnabled(true);
        ui->Disactive->setEnabled(false);
        qDebug() << "AP IS NOT RUNNING\n";
    ////////////////////////////////////////////////////////////////////////////////////////////
    // Выясняем, что не так
    ////////////////////////////////////////////////////////////////////////////////////////////
        for(i=0;i<4;i++) if(a[i]==0) tempI++; // Считаем, сколько пунктов не активны

        temp_QT.clear(); temp_QT = tr("AP offline because ");
        if(a[0]==0) { temp_QT.append(tr("network interface is down")); tempI2++;
        if(tempI>tempI2) temp_QT.append(tr(" and ")); }
        if(a[1]==0) { temp_QT.append(tr("DNSMASQ offline")); tempI2++;
        if(tempI>tempI2) temp_QT.append(tr(" and ")); }
        if(a[2]==0) { temp_QT.append(tr("Hostapd offline")); tempI2++;
        if(tempI>tempI2) temp_QT.append(tr(" and ")); }
        if(a[3]==0) { temp_QT.append(tr("IP Forward disabled")); }
        temp_QT.append(tr("."));

        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, temp_QT, QString("0")));

    }
    ////////////////////////////////////////////////////////////////////////////////////////////
    delete GlobalSettings;

}

void MainWindow::on_EditorW_activated()
{
    // Показать окно "Редактор"
    EditorConfig editor;
    editor.exec();
}

void MainWindow::FromIndicator() {
    QTimer::singleShot(1000, this, SLOT(checkStatus()));
    //checkStatus();
}

void MainWindow::on_Log_activated()
{
    LogSystem logsystem;
    logsystem.exec();
}

QIcon MainWindow::iconReturn(QString name) {
    QString iconPath = ":/pic/icons/";
    QIcon icon = QIcon::fromTheme(name);
    if (icon.isNull())
        icon = QIcon(QString("%1%2%3").arg(iconPath, name, ".png"));
    return icon;
}

void MainWindow::LogSystemAppend(QString Input) {
////////////////////////////////////////////////////////////////////////////////////////
    QDir directory;
    QString TempString;
    bool t=false;
    QDate dateNow;
    TempString  = dateNow.currentDate().toString("yyyy.MM.d");
////////////////////////////////////////////////////////////////////////////////////////
// Логи лежат в /root/.WiFiHostapdAP/logs/сегодняшняя дата/log.txt
////////////////////////////////////////////////////////////////////////////////////////
// 1. Идём в папку /root/.WiFiHostapdAP/logs/
////////////////////////////////////////////////////////////////////////////////////////
   directory.cd("/root");
if(!directory.cd(".WiFiHostapdAP")) {
    directory.mkdir(QString("%1").arg(".WiFiHostapdAP"));
    directory.cd(".WiFiHostapdAP"); }
if(!directory.cd("logs")) {
    directory.mkdir(QString("%1").arg("logs"));
    directory.cd("logs"); }
////////////////////////////////////////////////////////////////////////////////////////
// 2. Смотрим список файлов и если там есть папка с сегодняшней датой, то всё отлично,
// если её нет, то создём ей
////////////////////////////////////////////////////////////////////////////////////////

    QStringList listDir = directory.entryList(QDir::Dirs);
        foreach (QString subdir, listDir) {
            if (subdir == "." || subdir == "..") continue;
            else {
                if(subdir==TempString) t = true;
            }
        }
   if(!t) directory.mkdir(TempString);
//////////////////////////////////////////////////////////////////////////////////////////
//// 3. Открываем файл, чтобы дозаписывать строки
//////////////////////////////////////////////////////////////////////////////////////////
    QFile LogFileNew(QString("%1%2%3").arg("/root/.WiFiHostapdAP/logs/", TempString, "/log.txt"));
    LogFileNew.open(QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered);
    QTextStream LogFileOout(&LogFileNew);
//////////////////////////////////////////////////////////////////////////////////////////
//// 4. Формируем лог и записываем в файл
//////////////////////////////////////////////////////////////////////////////////////////
    QString InputNew = Input.replace("\n","");
    LogFileOout << InputNew << "\n";
//// Закрываем файл
    LogFileNew.close();
}

void MainWindow::CheckUbuntuPrecsisNM() {
 // Функция создана с целью проверить, включен ли в качестве DNS DNSMASQ в настройках NetworkManager
 // Если это так, и если программа запущена в ОС Ubuntu 12.04, то она отключит её
//////////////////////////////////////////////////////////////////////////////////////////
// 1 шаг - проверка дистрибутива:
//////////////////////////////////////////////////////////////////////////////////////////
char temp[200];
QString qtTemp;
QDateTime TimeNow;
QString dateUNIXNow;
bool test = false;
// qDebug() << "Testing DNS... ";

QFile file_TEST("/etc/NetworkManager/NetworkManager.conf");
QFile file_LSB("/etc/lsb-release");
QFile file("/etc/NetworkManager/NetworkManager.conf");
QFile fileTo("/etc/NetworkManager/NetworkManager.conf.new");
QString Line;

if(file_LSB.open(QFile::ReadOnly)) {
    while(!file_LSB.atEnd()) {
        qtTemp = file_LSB.readLine();
        if(qtTemp.startsWith("DISTRIB_CODENAME") && qtTemp.endsWith("precise\n"))
            test = true;
    }
file_LSB.close();
qtTemp.clear();
}

if(test && file_TEST.exists()){
  //  qDebug() << "This is Ubuntu 12.04.";
//////////////////////////////////////////////////////////////////////////////////////////
// 2 шаг - Проверяем, не выключин ли ранее DNS
//////////////////////////////////////////////////////////////////////////////////////////
    if(file_TEST.open(QFile::ReadOnly)) {
        test = false;
        while(!file_TEST.atEnd()) {
        qtTemp= file_TEST.readLine();
        if(qtTemp.startsWith("dns=dnsmasq"))
                test = true;
        }
    file_TEST.close();
    qtTemp.clear();
    }

if(test && file_TEST.exists()) {
//////////////////////////////////////////////////////////////////////////////////////////
// 3 шаг - Запрашиваем у пользователя разрешение на отключение
// DNSMASQ в качестве локального DNS
//////////////////////////////////////////////////////////////////////////////////////////

  //  qDebug() << "DNS used by NM";
    QMessageBox msgBox;
    msgBox.setText(tr("DNSMAQS уже используется"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText(tr("1. Внимание! Похоже, что DNSMASQ уже используется в вашей системе, как локальный DNS. \nДля корректной работы программы необходимо отключить локальный DNS. \nПеред тем, как продолжить, пожалуйста, закройте все соединения с Интернет. \nПродолжить?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret==QMessageBox::Ok) {
//////////////////////////////////////////////////////////////////////////////////////////
// 3 шаг - Отключаем DNS и перезапускаем NM
//////////////////////////////////////////////////////////////////////////////////////////
// 1 - Отключаем DNSMASQ
        //system("/etc/init.d/dnsmasq stop")
        console("/etc/init.d/dnsmasq stop", temp);
       // qDebug() << "Stoping DNSMASQ ... " << temp ;
        strcpy(temp, "");
// 2 - Выключаем DNS
 if(file.open(QFile::ReadOnly) && fileTo.open(QFile::WriteOnly)) {
            QTextStream newFileTo(&fileTo);
       //     qDebug() << "Files is opened";

        while(!file.atEnd()) {
        Line = file.readLine();
        if(Line.startsWith("dns=dnsmasq"))
            Line.replace("dns=dnsmasq", "#dns=dnsmasq");
        newFileTo << Line;
  //      qDebug() << Line;
        }
        file.close();
        fileTo.close();
        // Меняем файлы местами
        QFile::remove("/etc/NetworkManager/NetworkManager.conf");
        QFile::rename("/etc/NetworkManager/NetworkManager.conf.new", "/etc/NetworkManager/NetworkManager.conf");
        console("chmod 755 /etc/NetworkManager/NetworkManager.conf", temp);
        strcpy(temp, "");
        // 3 - перезапускаем NM

        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
         LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, tr("DNS is disable succesfully"), QString("1")));

      // console("/etc/init.d/network-manager restart", temp);
     system("/etc/init.d/network-manager restart");
    }

} // 4
    else {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("Работа программы невозможна"));
        msgBox.setInformativeText(tr("2. Для работы программе нужно иметь полный доступ к DNSMASQ."));
        msgBox.exec();
        parentWidget()->close();
    }
} // 3
} // 2
}

void MainWindow::CheckHostapdInitdScript() {
    //////////////////////////////////////////////////////////////////////////////////////////
    // Метод устраняет проблему запуска Hostapd из-за кривого скрипта
    //////////////////////////////////////////////////////////////////////////////////////////
    QDateTime TimeNow;
    char temp[500];
    QString dateUNIXNow;
    QFile file("/etc/init.d/hostapd");
    QFile file_TEST("/etc/init.d/hostapd");
    QFile fileTo("/etc/init.d/hostapd.new");
    QString Line;
    bool test = true;

    if(file_TEST.open(QFile::ReadOnly)) {
        while(!file_TEST.atEnd()) {
            Line = file_TEST.readLine();
            if(Line.startsWith("DAEMON_CONF=/etc/hostapd/hostapd.conf\n"))
                test = false;
        }
        Line.clear();
    }
     file_TEST.close();


   if(test) {
        if(file.open(QFile::ReadOnly) && fileTo.open(QFile::WriteOnly)) {

            QTextStream newFileTo(&fileTo);

        while(!file.atEnd()) {
        Line = file.readLine();
        if(Line.startsWith("DAEMON_CONF=\n"))
            newFileTo << "DAEMON_CONF=/etc/hostapd/hostapd.conf\n";
        else
        newFileTo << Line;
        }

        file.close();
        fileTo.close();

        // Меняем файлы местами
        if(QFile::remove("/etc/init.d/hostapd") && QFile::rename("/etc/init.d/hostapd.new", "/etc/init.d/hostapd")) {

            console("chmod 755 /etc/init.d/hostapd", temp);
            strcpy(temp, "");
            console("chmod +x /etc/init.d/hostapd", temp);
            strcpy(temp, "");
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());

    LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, tr("Hostapd Startup Script will be edited."), QString("1")));
        } else {
            dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
            LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, tr("Error edit Hostapd file."), QString("2")));
        }
    } // Открыт ли файл
    else {
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        LogSystemAppend(QString("%1|%2|%3|%4").arg(tr("Programm"), dateUNIXNow, tr("Error edit Hostapd file."), QString("2")));

        QMessageBox arg;
        arg.setText(tr("Error edit Hostapd file."));
        arg.setIcon(QMessageBox::Warning);
        arg.exec();
    } // else

} // test
} // function
