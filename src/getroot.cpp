#include "getroot.h"
#include "ui_getroot.h"


GetRoot::GetRoot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GetRoot)
{
    ui->setupUi(this);
}

GetRoot::~GetRoot()
{
    delete ui;
}

void GetRoot::on_pushButton_clicked()
{

    QSettings *LocalSettings = new QSettings(QString("%1%2").arg(QDir::homePath(), "/.WiFiHostapdAP/WiFi_Hostapd_AP.conf"), QSettings::NativeFormat);
    QString command;
    if(ui->comboBox->currentIndex()==0)
         command = "gksudo ";
    else
         command = "kdesudo ";

    LocalSettings->setValue("Programm/rootCommand", command);
    LocalSettings->setValue("Programm/suCommand", false);

    delete LocalSettings;
    this->close();
}
