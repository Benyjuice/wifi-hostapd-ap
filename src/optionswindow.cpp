#include "optionswindow.h"
#include "ui_optionswindow.h"

OptionsWindow::OptionsWindow(QWidget *parent) :
    QDialog(parent), ui(new Ui::OptionsWindow) {
    ui->setupUi(this);
    // конки приложения:

    /* Настройки  */
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    int temp_int, i;
    bool TestItem = false;
    QString temp_qstring;

    /////////////////////////////////////////////
    // Обновление трафика (интервал)
    temp_int = GlobalSettings->value("Programm/UpdateTraffic", 1).toInt();
    ui->UpdateTraffic->setValue(temp_int);
    // Язык
    ui->Language->setCurrentIndex(GlobalSettings->value("Programm/Language", 3).toInt());
    // Клиенты
    ui->ClientPriceUpdate->setValue(GlobalSettings->value("Programm/ClientsPriceUpdateTime", 5).toInt());

    /////////////////////////////////////////////

    ui->IP_SERVER->setText(GlobalSettings->value("AP/IP_SERVER", "192.168.0.1").toString());
    ui->MASKA->setText(GlobalSettings->value("AP/MASK", "255.255.255.0").toString());
    ui->SSID->setText(GlobalSettings->value("AP/SSID", "MyWiFI_AP").toString());
    ui->ShowSSID->setChecked(GlobalSettings->value("AP/ShowSSID", false).toBool());
    ui->Password->setText(GlobalSettings->value("AP/Password", "My_WiFi_AP").toString());
    /////////////////////////////////////////////
    // Драйвер
    /////////////////////////////////////////////
    i=0;
    TestItem = false;
    while(1) { temp_qstring = ui->driver->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/drive", "nl80211")) {  ui->driver->setCurrentIndex(i); TestItem = true; } } else { break; } i++; }
    if(!TestItem) {
        ui->driver->addItem(GlobalSettings->value("AP/drive", "nl80211").toString(), QVariant());
        ui->driver->setCurrentIndex((ui->driver->count()-1));
    }

    /////////////////////////////////////////////
    // Интерфейс
    /////////////////////////////////////////////
    i=0;
    TestItem = false;
    while(1) { temp_qstring = ui->Iface->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/Iface", "wlan0")) {  ui->Iface->setCurrentIndex(i); TestItem = true; } } else { break; } i++; }
    if(!TestItem) {
        ui->Iface->addItem(GlobalSettings->value("AP/Iface", "wlan0").toString(), QVariant());
        ui->Iface->setCurrentIndex((ui->Iface->count()-1));
    }

    /////////////////////////////////////////////
    // Каналы
    /////////////////////////////////////////////
    i=0;
    while(1) { temp_qstring = ui->Channels->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/Channels", "7")) {  ui->Channels->setCurrentIndex(i);  } } else { break; } i++; }

    /////////////////////////////////////////////
    // Код страны
    /////////////////////////////////////////////
    i=0;
    while(1) { temp_qstring = ui->CountryCode->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/CountryCode", "RU")) {  ui->CountryCode->setCurrentIndex(i); } } else { break; } i++; }

    /////////////////////////////////////////////
    // Режим
    /////////////////////////////////////////////
    i=0;while(1) { temp_qstring = ui->TypeAP->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/TypeAP", "g")) {  ui->TypeAP->setCurrentIndex(i); } } else { break; } i++; }

    /////////////////////////////////////////////
    // Защита
    /////////////////////////////////////////////
    i=0;
    while(1) { temp_qstring = ui->Protection->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/Protection", "WPA2")) {  ui->Protection->setCurrentIndex(i); } } else { break; } i++; }
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    /////////////////////////////////////////////
    // третяя вкладка: dhcp
    /////////////////////////////////////////////
    ui->IP_CLIENT1->setText(GlobalSettings->value("DHCP/IP_CLIENT1", "192.168.0.2").toString());
    ui->IP_CLIENT2->setText(GlobalSettings->value("DHCP/IP_CLIENT2", "192.168.0.255").toString());
    ui->ip_time->setCurrentIndex(GlobalSettings->value("DHCP/IP_time", 2).toInt());
    ui->UseGoogle->setChecked(GlobalSettings->value("DHCP/UseOpenDNS", true).toBool());
    /////////////////////////////////////////////
    // Интернет
    /////////////////////////////////////////////
    i=0;
    TestItem = false;
    while(1) { temp_qstring = ui->Internet_iface->itemText(i);
        if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("DHCP/Internet_iface", "eth0")) {  ui->Internet_iface->setCurrentIndex(i); TestItem = true; } } else { break; } i++; }
    if(!TestItem) {
        ui->Internet_iface->addItem(GlobalSettings->value("DHCP/Internet_iface", "eth0").toString(), QVariant());
        ui->Internet_iface->setCurrentIndex((ui->Internet_iface->count()-1));
    }

    delete GlobalSettings;
}


OptionsWindow::~OptionsWindow() { delete ui; }

void OptionsWindow::set_Settings() {

}

int xrand(int N) { return random() % N; }


void OptionsWindow::on_RestoreProg_clicked()
{
    /* Обнуляем все значения на вкладке "Приложение" */
    QMessageBox msgBox;
    msgBox.setText(tr("Data Restore"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText(tr("Attention! All settings will be reset to the original. Continue?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        ui->UpdateTraffic->setValue(1);
        ui->Language->setCurrentIndex(0);
        ui->ClientPriceUpdate->setValue(5);

        QDateTime TimeNow;
        QString dateUNIXNow;
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("Programm settings set to default."), QString("1")));

        on_ProgSave_clicked();
        break;
    }
    OptionsWindow::on_ProgSave_clicked();
}


void OptionsWindow::on_RestoreAP_clicked()
{
    /* Обнуляем все значения на вкладке "Hostapd" */
    QMessageBox msgBox;
    msgBox.setText(tr("Сброс данных"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText(tr("Внимание! Все настройки будут сброшены до первоначальных. Продолжить?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        ui->IP_SERVER->setText("192.168.0.1");
        ui->MASKA->setText("255.255.255.0");
        ui->SSID->setText("MyWiFI_AP");
        ui->ShowSSID->setChecked(false);
        ui->driver->setCurrentIndex(1);
        ui->Iface->setCurrentIndex(0);
        ui->TypeAP->setCurrentIndex(0);
        ui->Protection->setCurrentIndex(1);
        ui->CountryCode->setCurrentIndex(0);
        ui->Channels->setCurrentIndex(6);

        QDateTime TimeNow;
        QString dateUNIXNow;
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("Hostapd settings set to default."), QString("1")));

        on_APSave_clicked(false);
        break; }
}


void OptionsWindow::on_RestoreDHCP_clicked()
{
    /* Обнуляем все значения на вкладке "DHCP" */
    QMessageBox msgBox;
    msgBox.setText(tr("Сброс данных"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText(tr("Внимание! Все настройки будут сброшены до первоначальных. Продолжить?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Ok:

        ui->IP_CLIENT1->setText("192.168.0.2");
        ui->IP_CLIENT2->setText("192.168.0.255");
        ui->Internet_iface->setCurrentIndex(0);
        ui->ip_time->setCurrentIndex(2);
        ui->UseGoogle->setChecked(true);
        QDateTime TimeNow;
        QString dateUNIXNow;
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("DNSMASQ settings set to default."), QString("1")));

        on_DHCPSave_clicked(false);
        break;
    }
}
// Сохраняем настройки из первой вкладки
void OptionsWindow::on_ProgSave_clicked()
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    QDateTime TimeNow;
    QString dateUNIXNow;

    GlobalSettings->setValue("Programm/UpdateTraffic", ui->UpdateTraffic->value());
    GlobalSettings->setValue("Programm/Language", ui->Language->currentIndex());
    GlobalSettings->setValue("Programm/ClientsPriceUpdateTime", ui->ClientPriceUpdate->value());

    dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
    emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("Programm settings succesfully update."), QString("1")));

    // Programm settings
    QMessageBox msgBoxProgramm;
    msgBoxProgramm.setText(tr("Настройки успешно обновлены"));
    msgBoxProgramm.setIcon(QMessageBox::Information);
    msgBoxProgramm.setInformativeText(tr("Изменения вступят в силу после перезапуска приложения."));
    msgBoxProgramm.setStandardButtons(QMessageBox::Ok);
    msgBoxProgramm.setDefaultButton(QMessageBox::Ok);
    msgBoxProgramm.exec();

    delete GlobalSettings;
}

// Сохраняем настройки из второй вкладки
void OptionsWindow::on_APSave_clicked(bool nomesseage = true)
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта

    GlobalSettings->setValue("AP/IP_SERVER", ui->IP_SERVER->text());
    GlobalSettings->setValue("AP/MASK", ui->MASKA->text());
    GlobalSettings->setValue("AP/SSID", ui->SSID->text());
    GlobalSettings->setValue("AP/ShowSSID",  ui->ShowSSID->checkState());
    GlobalSettings->setValue("AP/drive", ui->driver->currentText());
    GlobalSettings->setValue("AP/Iface", ui->Iface->currentText());
    GlobalSettings->setValue("AP/Channels", ui->Channels->currentText());
    GlobalSettings->setValue("AP/CountryCode", ui->CountryCode->currentText());
    GlobalSettings->setValue("AP/TypeAP", ui->TypeAP->currentText());
    GlobalSettings->setValue("AP/Protection", ui->Protection->currentText());
    if(ui->Password->text().length() > 7 && ui->Password->text().length() < 17) {
        GlobalSettings->setValue("AP/Password", ui->Password->text()); }
    else {  GlobalSettings->setValue("AP/Password", "My_WiFi_AP"); }
    ui->Password->setText(GlobalSettings->value("AP/Password", "My_WiFi_AP").toString());

    SaveAConfig(0, nomesseage);
    delete GlobalSettings;
}

// Сохраняем настройки из третьей вкладки
void OptionsWindow::on_DHCPSave_clicked(bool nomesseage = true)
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта

    /////////////////////////////////////////////
    GlobalSettings->setValue("DHCP/IP_CLIENT1", ui->IP_CLIENT1->text());
    GlobalSettings->setValue("DHCP/IP_CLIENT2", ui->IP_CLIENT2->text());
    GlobalSettings->setValue("DHCP/Internet_iface", ui->Internet_iface->currentText());
    GlobalSettings->setValue("DHCP/IP_time", ui->ip_time->currentIndex());
    GlobalSettings->setValue("DHCP/UseOpenDNS", ui->UseGoogle->checkState());

    SaveAConfig(1, nomesseage);
    delete GlobalSettings;
}

void OptionsWindow::SaveAConfig(int step, bool nomesseage = true) {
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    // Функция сохраения конфигурационных файлов по маске
    // №1 - что сохраняем - 0 = только HOSTAPD; 1 - DNSMASQ
    QString temp;
    QDateTime TimeNow;
    QString dateUNIXNow;
    if(step == 0) {
        //  Сохраняем только HOSTAPD
        temp = getMaskToConfig(0);
        QFile::remove("/etc/hostapd/hostapd.conf");
        QFile hostapd_config("/etc/hostapd/hostapd.conf");
        hostapd_config.open(QIODevice::WriteOnly);
        QTextStream out(&hostapd_config);
        out << temp;
        hostapd_config.close();


        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("Hostapd config succesfully update."), QString("1")));

        // Hostapd
        if(!nomesseage) {
            QMessageBox msgBoxHostapd;
            msgBoxHostapd.setText(tr("Конфигурационный файл обновлён"));
            msgBoxHostapd.setIcon(QMessageBox::Information);
            msgBoxHostapd.setInformativeText(tr("Настройки Hostapd успешно сохранены. Для вступления их в силу, пожалуйста, перезапустите точку доступа."));
            msgBoxHostapd.setStandardButtons(QMessageBox::Ok);
            msgBoxHostapd.setDefaultButton(QMessageBox::Ok);
            msgBoxHostapd.exec(); }
        temp.clear();
    } else {
        //  Сохраняемфайл DNSMASQ
        // 1
        temp = getMaskToConfig(1);
        QFile::remove("/etc/dnsmasq.conf");
        QFile firstDHCPConfig("/etc/dnsmasq.conf");
        firstDHCPConfig.open(QIODevice::WriteOnly);
        QTextStream out1(&firstDHCPConfig);
        out1 << temp;
        firstDHCPConfig.close();
        temp.clear();
        dateUNIXNow = QString::number(TimeNow.currentDateTime().toTime_t());
        emit  toMainLog(QString("%1|%2|%3|%4").arg(tr("Options"), dateUNIXNow, tr("DNSMASQ config succesfully update."), QString("1")));

        // DHCP
        if(!nomesseage) {
            QMessageBox msgBoxDHCP;
            msgBoxDHCP.setText(tr("Конфигурационный файл обновлён"));
            msgBoxDHCP.setIcon(QMessageBox::Information);
            msgBoxDHCP.setInformativeText(tr("Настройки DHCP успешно сохранены. Для вступления их в силу, пожалуйста, перезапустите точку доступа."));
            msgBoxDHCP.setStandardButtons(QMessageBox::Ok);
            msgBoxDHCP.setDefaultButton(QMessageBox::Ok);
            msgBoxDHCP.exec(); }
        temp.clear();
    }
    delete GlobalSettings;
}

QString OptionsWindow::getMaskToConfig(int step) {
    QString mask;
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // creating a new object
    QDir dir;
    QFileInfoList list;
    QString temp_qstring = "default.conf";
    QString path;

    switch(step) {
    case 0:

        dir.cd("/root");
        if(!dir.cd(".WiFiHostapdAP")) {
            dir.mkdir(QString("%1").arg(".WiFiHostapdAP"));
            dir.cd(".WiFiHostapdAP"); }

        if(!dir.cd("./Mask/")) {
            dir.mkdir(QString("%1").arg("Mask"));
            dir.cd("./Mask/"); }

        if(!dir.cd("./hostapd/")) {
            dir.mkdir(QString("%1").arg("hostapd"));
            dir.cd("./hostapd/"); }

        dir.setFilter(QDir::Files);
        list = dir.entryInfoList();

        if(list.size()==0) {

            QFile new_Default_Mask("/root/.WiFiHostapdAP/Mask/hostapd/default.conf");
            new_Default_Mask.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&new_Default_Mask);
            out << "#Name:Default\n";
            out << "#Type:Hostapd\n";
            out << "interface=[INTERFACE]\n";
            out << "driver=[DRIVER]\n";
            out << "ssid=[SSID]\n";
            out << "country_code=[COUNTRY_CODE]\n";
            out << "[TYPE_AP]\n";
            out << "channel=[CHANNEL]\n";
            out << "macaddr_acl=0\n";
            out << "[HIDEAP]\n";
            out << "[PROTECT]";
            new_Default_Mask.close();

            list.clear();
            dir.setFilter(QDir::Files);
            list = dir.entryInfoList();
        }
        path = "/root/.WiFiHostapdAP/Mask/hostapd/";

        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.fileName() == GlobalSettings->value("AP/ConfigMask", "default.conf").toString())
                temp_qstring = GlobalSettings->value("AP/ConfigMask", "default.conf").toString();
        }
        path += temp_qstring;

        break;
    case 1:

        dir.cd("/root");
        if(!dir.cd(".WiFiHostapdAP")) {
            dir.mkdir(QString("%1").arg(".WiFiHostapdAP"));
            dir.cd(".WiFiHostapdAP"); }

        if(!dir.cd("./Mask/")) {
            dir.mkdir(QString("%1").arg("Mask"));
            dir.cd("./Mask/"); }

        if(!dir.cd("./dnsmasq/")) {
            dir.mkdir(QString("%1").arg("dnsmasq"));
            dir.cd("./dnsmasq/"); }

        dir.setFilter(QDir::Files);
        list = dir.entryInfoList();

        if(list.size()==0) {
            // Если шаблонов нет, то создаём шаблон по-умолчанию
            QFile new_Default_Mask("/root/.WiFiHostapdAP/Mask/dnsmasq/default.conf");
            new_Default_Mask.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&new_Default_Mask);
            out << "#Name:Default\n";
            out << "#Type:DNSMASQ\n";
            out << "interface=[INTERFACE]\n";
            out << "dhcp-range=[RANGE_1],[RANGE_2],[IP_TIME]\n";
            out<< "[OpenDNS]";

            new_Default_Mask.close();

            list.clear();
            dir.setFilter(QDir::Files);
            list = dir.entryInfoList();
        }

        path = "/root/.WiFiHostapdAP/Mask/dnsmasq/";
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            if(fileInfo.fileName() == GlobalSettings->value("AP/ConfigMask1", "default.conf").toString())
                temp_qstring = GlobalSettings->value("AP/ConfigMask1", "default.conf").toString();
        }
        path += temp_qstring;

        break; }

    QFile file_mask(path);
    file_mask.open(QIODevice::ReadOnly);
    QTextStream in(&file_mask);

    mask = in.read(10240);

    QString temp1 = "";
    // HOSTAPD
    mask.replace("[INTERFACE]", GlobalSettings->value("AP/Iface", "wlan0").toString());
    mask.replace("[DRIVER]", GlobalSettings->value("AP/drive", "nl80211").toString());
    mask.replace("[SSID]", GlobalSettings->value("AP/SSID", "MyWiFI_AP").toString());
    mask.replace("[COUNTRY_CODE]", GlobalSettings->value("AP/CountryCode", "RU").toString());
    mask.replace("[CHANNEL]", GlobalSettings->value("AP/Channels", "7").toString());
    /////////////////////////////////////////////////////////////////////////
    // Тип точки доступа
    /////////////////////////////////////////////////////////////////////////
    if(GlobalSettings->value("AP/TypeAP", "g").toString()=="n")
        temp1 = "hw_mode=g\nwme_enabled=1\nieee80211n=1\nht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]";
    else if(GlobalSettings->value("AP/TypeAP", "g").toString()=="g")
        temp1 = "hw_mode=g";
    else
        temp1 = "\n";

    mask.replace("[TYPE_AP]", temp1);
    temp1.clear();

    /////////////////////////////////////////////////////////////////////////
    // Разбираемся с защитой
    /////////////////////////////////////////////////////////////////////////
    if(GlobalSettings->value("AP/Protection", "WPA3").toString()=="WPA3") {
        // Составляем маску для WPA
        temp1 = "wpa=3\nwpa_key_mgmt=WPA-PSK\nwpa_pairwise=TKIP\nrsn_pairwise=CCMP\nwpa_passphrase=";
        temp1.append(GlobalSettings->value("AP/Password", "MyWiFI_AP").toString()); }
    /////////////////////////////////////////////////////////////////////////
    else if(GlobalSettings->value("AP/Protection", "WPA3").toString()=="WEP") {
        // Составляем маску для WPA
        temp1 = "wep_default_key=0\nwep_key_len_broadcast=13\nwep_key_len_unicast=13\nwep_rekey_period=300\nwep_key0=";
        temp1.append(GlobalSettings->value("AP/Password", "MyWiFI_AP").toString());
    }

    else temp1 = "";
    mask.replace("[PROTECT]", temp1);

    if(GlobalSettings->value("AP/ShowSSID", false).toBool())
        mask.replace("[HIDEAP]", "ignore_broadcast_ssid=2");
    else mask.replace("[HIDEAP]", "");
    temp1.clear();

    // DNSMASQ
    mask.replace("[RANGE_1]", GlobalSettings->value("DHCP/IP_CLIENT1", "192.168.0.2").toString());
    mask.replace("[RANGE_2]", GlobalSettings->value("DHCP/IP_CLIENT2", "192.168.0.225").toString());
    mask.replace("[INTERFACE_INTERNET]", GlobalSettings->value("DHCP/Internet_iface", "eth0").toString());

    if(GlobalSettings->value("DHCP/UseOpenDNS", true).toBool())
        mask.replace("[OpenDNS]", "server=/www.google.com/8.8.8.8");
    else
        mask.replace("[OpenDNS]", "");

    switch(GlobalSettings->value("DHCP/IP_time", 2).toInt()) {
    case(0): mask.replace("[IP_TIME]", "10m"); break;
    case(1): mask.replace("[IP_TIME]", "30m"); break;
    case(2): mask.replace("[IP_TIME]", "1h"); break;
    case(3): mask.replace("[IP_TIME]", "2h"); break;
    case(4): mask.replace("[IP_TIME]", "6h"); break;
    case(5): mask.replace("[IP_TIME]", "12h"); break;
    }

    file_mask.close();
    delete GlobalSettings;
    return mask;
}
