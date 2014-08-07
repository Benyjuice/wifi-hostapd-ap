#include "editorconfig.h"
#include "ui_editorconfig.h"

EditorConfig::EditorConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditorConfig)
{
    ui->setupUi(this);
    update_mask(0);
    ui->textEdit->clear();
    setTextCode(0,0, "");
}

EditorConfig::~EditorConfig()
{
    delete ui;
}

int EditorConfig::update_mask(int page = 0) {
QDir dir;
QFileInfoList list;
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
QString temp_qstring;
int i = 0;

ui->mask_chooser->clear();
switch(page) {
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
        /* Если шаблонов нет, то создаём шаблон по-умолчанию */
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

  /* в цикле выводим сведения о файлах */
for (i = 0; i < list.size(); ++i) {
QFileInfo fileInfo = list.at(i);
ui->mask_chooser->addItem(qPrintable(QString("%1").arg(fileInfo.fileName())));
}

i=0;
while(1) { temp_qstring = ui->mask_chooser->itemText(i);
if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("AP/ConfigMask", "default.conf")) {  ui->mask_chooser->setCurrentIndex(i); } } else { break; } i++; }

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

    QString path = "/root/.WiFiHostapdAP/Mask/dnsmasq/";
        for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName() == GlobalSettings->value("AP/ConfigMask1", "default.conf").toString())
            temp_qstring = GlobalSettings->value("AP/ConfigMask1", "default.conf").toString();
        }
        path += temp_qstring;



// в цикле выводим сведения о файлах
for (i = 0; i < list.size(); ++i) {
QFileInfo fileInfo = list.at(i);

ui->mask_chooser->addItem(qPrintable(QString("%1").arg(fileInfo.fileName())));
}
i=0;
while(1) { temp_qstring = ui->mask_chooser->itemText(i);
if(temp_qstring.length()>0) { if(temp_qstring == GlobalSettings->value("DHCP/ConfigMask1", "default.conf")) {  ui->mask_chooser->setCurrentIndex(i); } } else { break; } i++; }

break; }

delete GlobalSettings;
return 0;
}


void EditorConfig::on_Type_Config_1_clicked(bool checked)
{
    if(checked) {
    // Если активировано, то обновляем список
    update_mask(0);
    // Проверяем статус вторичных элементов
    if(ui->Config_Set_View_1->isChecked()) {
    // Показываем конфигурационный файл
        ui->textEdit->clear();
        setTextCode(0,0, "");
    }
    if(ui->Config_Set_View_2->isChecked()) {
    // Показываем шаблон
        ui->textEdit->clear();
        setTextCode(0,1, "");
    }
    }
}

void EditorConfig::on_Type_Config_2_clicked(bool checked)
{
    if(checked) {
    // Если активировано, то обновляем список
    update_mask(1);
    // Проверяем статус вторичных элементов
    if(ui->Config_Set_View_1->isChecked()) {
    // Показываем конфигурационный файл
        ui->textEdit->clear();
        setTextCode(1,0, "");
    }
    if(ui->Config_Set_View_2->isChecked()) {
    // Показываем шаблон
        ui->textEdit->clear();
        setTextCode(1,1, "");
    }
    }
}



void EditorConfig::setTextCode(int state, int type, const QString name) {
// state - вкладка (hostapd, dhcp, interface)
// type - маска или конфигурационный файл (0 - файл, 1 - маска)
// name - имя маски на всякий пожарный
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
QString temp;
if(type == 0) {
// Открываем конфигурационные файлы:
if(state == 0) {
QFile file_to_open("/etc/hostapd/hostapd.conf");
file_to_open.open(QIODevice::ReadOnly);
QTextStream in(&file_to_open);
QString file_code;
file_code = in.read(10240);
ui->textEdit->setText(file_code);
file_to_open.close();
}

if(state == 1) {
QFile file_to_open1("/etc/dnsmasq.conf");
file_to_open1.open(QIODevice::ReadOnly);
QTextStream in1(&file_to_open1);
QString file_code1;
file_code1 = in1.read(10240);
ui->textEdit->setText(file_code1);
file_to_open1.close();
}



}
else {
// Открываем маски:
QString path_to_open_file;
QString  file_code_mask;
if(state == 0 && name.length()==0) { path_to_open_file = "/root/.WiFiHostapdAP/Mask/hostapd/" + GlobalSettings->value("AP/HOSTAPD_MASK", "default.conf").toString(); }
if(state == 1 && name.length()==0) { path_to_open_file = "/root/.WiFiHostapdAP/Mask/dnsmasq/" + GlobalSettings->value("AP/DHCP_MASK", "default.conf").toString(); }
if(name.length() > 0) {
    if(state == 0) { path_to_open_file = "/root/.WiFiHostapdAP/Mask/hostapd/" + name; }
    if(state == 1) { path_to_open_file = "/root/.WiFiHostapdAP/Mask/dnsmasq/" + name; }
}

QFile file_to_read(path_to_open_file);
if(!file_to_read.open(QIODevice::ReadOnly)) {
    QMessageBox msgBox;
    msgBox.setText(tr("Ошибка!"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(tr("Указанный файл не найден. Будет открыт файл по-умолчанию."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    QString temp1;
    if(state == 0) { temp1 = "/root/.WiFiHostapdAP/Mask/hostapd/defaul.conf"; }
    if(state == 0) { temp1 = "/root/.WiFiHostapdAP/Mask/dnsmasq/defaul.conf"; }

    QFile file_to_read_default(temp1);
    file_to_read_default.open(QIODevice::ReadOnly);
    QTextStream in_default(&file_to_read_default);
    file_code_mask = in_default.read(10240);
    ui->textEdit->setText(file_code_mask);

    file_to_read_default.close();
} else {
QTextStream in(&file_to_read);
file_code_mask = in.read(10240);
ui->textEdit->setText(file_code_mask);
file_to_read.close(); }
}



delete GlobalSettings;
}

void EditorConfig::on_Config_Set_View_1_clicked(bool checked)
{
    checked = true;
    ui->textEdit->clear();
    if(ui->Type_Config_1->isChecked()) setTextCode(0,0, "");
    if(ui->Type_Config_2->isChecked()) setTextCode(1,0, "");
}

void EditorConfig::on_Config_Set_View_2_clicked(bool checked)
{
    checked = true;
    ui->textEdit->clear();
    if(ui->Type_Config_1->isChecked()) setTextCode(0,1, "");
    if(ui->Type_Config_2->isChecked()) setTextCode(1,1, "");
}


void EditorConfig::on_save_mask_clicked()
{
    QString path_to_save_file;
    if(ui->Type_Config_1->isChecked()) path_to_save_file = "/root/.WiFiHostapdAP/Mask/hostapd/" + ui->mask_chooser->currentText();
    if(ui->Type_Config_2->isChecked()) path_to_save_file = "/root/.WiFiHostapdAP/Mask/dnsmasq/" + ui->mask_chooser->currentText();

    QFile file_to_write(path_to_save_file);
    file_to_write.open(QIODevice::WriteOnly);
    QTextStream out(&file_to_write);
    out << ui->textEdit->toPlainText();

    if(ui->Type_Config_1->isChecked()) update_mask(0);
    if(ui->Type_Config_2->isChecked()) update_mask(1);

}

void EditorConfig::on_mask_chooser_currentIndexChanged(QString )
{
    ui->textEdit->clear();
    if(ui->Type_Config_1->isChecked()) setTextCode(0,1, ui->mask_chooser->currentText());
    if(ui->Type_Config_2->isChecked()) setTextCode(1,1, ui->mask_chooser->currentText());
}

void EditorConfig::on_delet_mask_clicked()
{
    QString path_to_delete;
    if(ui->Type_Config_1->isChecked()) path_to_delete = "/root/.WiFiHostapdAP/Mask/hostapd/" + ui->mask_chooser->currentText();
    if(ui->Type_Config_2->isChecked()) path_to_delete = "/root/.WiFiHostapdAP/Mask/dnsmasq/" + ui->mask_chooser->currentText();
    QFile::remove(path_to_delete);

    if(ui->Type_Config_1->isChecked()) update_mask(0);
    if(ui->Type_Config_2->isChecked()) update_mask(1);
}


void EditorConfig::on_load_mask_clicked()
{
    // сохраняем конфигурационный файл по текущей маске, делая её активной
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat); // создание нового объекта
    QString path_to_save_config;
    QString text_of_mask = ui->textEdit->toPlainText();
    QMessageBox msgBox;
    msgBox.setText(tr("Конфигурационный файл обновлён."));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(tr("Настройки применены. Для того, чтобы они вступили в силу, перезапустите точку доступа."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

    if(ui->Type_Config_1->isChecked()) { path_to_save_config = "/etc/hostapd/hostapd.conf"; GlobalSettings->setValue("AP/ConfigMask", ui->mask_chooser->currentText()); }
    if(ui->Type_Config_2->isChecked()) { path_to_save_config = "/etc/dnsmasq.conf"; GlobalSettings->setValue("DHCP/ConfigMask1", ui->mask_chooser->currentText()); }

/* Заменяем все элементы в маске на экиваленты */
    replaseTags(text_of_mask);

    QFile::remove(path_to_save_config);
    // Создаём новый конфигурационный файл
    QFile file_to_write(path_to_save_config);
    file_to_write.open(QIODevice::WriteOnly);
    QTextStream out(&file_to_write);
    out << text_of_mask;
    msgBox.exec();
    file_to_write.close();
    delete GlobalSettings;
}


void EditorConfig::replaseTags(QString &str) {
QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
QString temp1 = "";
/////////////////////////////////////////////////////////////////////////
// HOSTAPD
/////////////////////////////////////////////////////////////////////////
str.replace("[INTERFACE]", GlobalSettings->value("AP/Iface", "wlan0").toString());
str.replace("[DRIVER]", GlobalSettings->value("AP/drive", "nl80211").toString());
str.replace("[SSID]", GlobalSettings->value("AP/SSID", "MyWiFI_AP").toString());
str.replace("[COUNTRY_CODE]", GlobalSettings->value("AP/CountryCode", "RU").toString());
str.replace("[CHANNEL]", GlobalSettings->value("AP/Channels", "7").toString());
/////////////////////////////////////////////////////////////////////////
// Тип точки доступа
/////////////////////////////////////////////////////////////////////////
if(GlobalSettings->value("AP/TypeAP", "g").toString()=="n")
    temp1 = "hw_mode=g\nwme_enabled=1\nieee80211n=1\nht_capab=[HT40+][SHORT-GI-40][DSSS_CCK-40]";
 else if(GlobalSettings->value("AP/TypeAP", "g").toString()=="g")
    temp1 = "hw_mode=g";
 else
    temp1 = "\n";

str.replace("[TYPE_AP]", temp1);
temp1.clear();

/////////////////////////////////////////////////////////////////////////
// Разбираемся с защитой
/////////////////////////////////////////////////////////////////////////
if(GlobalSettings->value("AP/Protection", "WPA3").toString()=="WPA3") {
    // Составляем маску для WPA
    temp1 = "wpa=3\nwpa_key_mgmt=WPA-PSK\nwpa_pairwise=TKIP CCMP\nwpa_passphrase=";
    temp1.append(GlobalSettings->value("AP/Password", "MyWiFI_AP").toString()); }
/////////////////////////////////////////////////////////////////////////
else if(GlobalSettings->value("AP/Protection", "WPA3").toString()=="WEP") {
    // Составляем маску для WPA
    temp1 = "wep_default_key=0\nwep_key_len_broadcast=13\nwep_key_len_unicast=13\nwep_rekey_period=300\nwep_key0=";
    temp1.append(GlobalSettings->value("AP/Password", "MyWiFI_AP").toString()); }
/////////////////////////////////////////////////////////////////////////
else temp1 = "";
/////////////////////////////////////////////////////////////////////////
str.replace("[PROTECT]", temp1);
temp1.clear();
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// Скрытая точка доступа
/////////////////////////////////////////////////////////////////////////
if(GlobalSettings->value("AP/ShowSSID", false).toBool())
    str.replace("[HIDEAP]", "ignore_broadcast_ssid=2");
else str.replace("[HIDEAP]", "");

/////////////////////////////////////////////////////////////////////////
// DNSMASQ
/////////////////////////////////////////////////////////////////////////
str.replace("[RANGE_1]", GlobalSettings->value("DHCP/IP_CLIENT1", "192.168.0.2").toString());
str.replace("[RANGE_2]", GlobalSettings->value("DHCP/IP_CLIENT2", "192.168.0.225").toString());
str.replace("[INTERFACE_INTERNET]", GlobalSettings->value("DHCP/Internet_iface", "eth0").toString());

if(GlobalSettings->value("DHCP/UseOpenDNS", true).toBool())
    str.replace("[OpenDNS]", "server=/www.google.com/8.8.8.8");
else
    str.replace("[OpenDNS]", "");

switch(GlobalSettings->value("DHCP/IP_time", 2).toInt()) {
    case(0): str.replace("[IP_TIME]", "10m"); break;
    case(1): str.replace("[IP_TIME]", "30m"); break;
    case(2): str.replace("[IP_TIME]", "1h"); break;
    case(3): str.replace("[IP_TIME]", "2h"); break;
    case(4): str.replace("[IP_TIME]", "6h"); break;
    case(5): str.replace("[IP_TIME]", "12h"); break;
}
/////////////////////////////////////////////////////////////////////////
delete GlobalSettings;
/////////////////////////////////////////////////////////////////////////
}

void EditorConfig::on_saveConfig_clicked()
{
    QSettings *GlobalSettings = new QSettings("/root/.WiFiHostapdAP/WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
    QString path_to_file_save;
    if(ui->Type_Config_1->isChecked())  path_to_file_save = "/etc/hostapd/hostapd.conf";
    if(ui->Type_Config_2->isChecked())  path_to_file_save = "/etc/dnsmasq.conf";

    QFile::remove(path_to_file_save);
    QFile file_to_save(path_to_file_save);
    file_to_save.open(QIODevice::WriteOnly);
    QTextStream out(&file_to_save);
    out << ui->textEdit->toPlainText();
    file_to_save.close();
    delete GlobalSettings;
    path_to_file_save.clear();

    QMessageBox msgBox;
    msgBox.setText(tr("Конфигурационный файл обновлён."));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(tr("Настройки применены. Для того, чтобы они вступили в силу, перезапустите точку доступа."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}
