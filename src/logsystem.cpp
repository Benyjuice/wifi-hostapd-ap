#include "logsystem.h"
#include "ui_logsystem.h"

LogSystem::LogSystem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogSystem)
{
    ui->setupUi(this);
///////////////////////////////////////////////////////////////////////////////////////////////
ui->DelLog->setEnabled(false);
InstallLogToday(); // Заполняем списки
}

LogSystem::~LogSystem()
{
    delete ui;
}

void LogSystem::on_DataChooser_textChanged(const QString &arg1)
{
//ui->LogView->horizontalHeader()->setVisible(true);
AddRowsToTable(arg1);
}

void LogSystem::InstallLogToday() {
// Переходим в папку с логами и получаем список всех логов, создаём сегодняшний.
////////////////////////////////////////////////////////////////////////////////////////
        QDir directory;
        QString TempString;
        QString temp_qstring;
        bool t=false;
        QDate dateNow;
        TempString  = dateNow.currentDate().toString("yyyy.MM.d");
        int i =0;
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
        ui->DataChooser->clear();
        QStringList listDir = directory.entryList(QDir::Dirs);
            foreach (QString subdir, listDir) {
                if (subdir == "." || subdir == "..") continue;
                else {
                    ui->DataChooser->addItem(subdir);
                    if(subdir==TempString) t = true;
                }
            }
            if(!t || !QFile::exists(QString("/root/.WiFiHostapdAP/logs/%1/log.txt").arg(TempString))) { directory.mkdir(TempString); ui->DataChooser->addItem(TempString);
//////////////////////////////////////////////////////////////////////////////////////////
// 3. Открываем файл, чтобы дозаписывать строки
//////////////////////////////////////////////////////////////////////////////////////////
        QFile LogFileNew(QString("%1%2%3").arg("/root/.WiFiHostapdAP/logs/", TempString, "/log.txt"));
        LogFileNew.open(QIODevice::Append | QIODevice::Text | QIODevice::Unbuffered);
        QTextStream LogFileOout(&LogFileNew);
//////////////////////////////////////////////////////////////////////////////////////////
// 4. Формируем лог и записываем в файл
//////////////////////////////////////////////////////////////////////////////////////////
        LogFileOout << "";
// Закрываем файл
        LogFileNew.close();
        }
//////////////////////////////////////////////////////////////////////////////////////////
// 5. Выбираем сегодняшний лог
//////////////////////////////////////////////////////////////////////////////////////////
while(1) { temp_qstring = ui->DataChooser->itemText(i);
if(temp_qstring.length()>0) { if(temp_qstring == TempString) {  ui->DataChooser->setCurrentIndex(i); } } else { break; } i++; }

//////////////////////////////////////////////////////////////////////////////////////////
// 6. Заполняем поле записями
//////////////////////////////////////////////////////////////////////////////////////////
AddRowsToTable(TempString);
}

void LogSystem::AddRowsToTable(QString data) {
// Заполняем из файла таблицу
    QDir directory;
    QString TempString;
    QDate dateNow;
    QStringList LineArray;
    QTableWidgetItem *item = 0;
    QString AllTextFromFile;
    int i = 0, size = 0;
    QDateTime *DateStep = 0;
    TempString  = dateNow.currentDate().toString("d.MM.yyyy");
    if(data.length()<2) data = TempString;

///////////////////////////////////////////////////////////////////////////////////////////
// Идём в папку, она есть точно, потому что эта функция вызывается из той, что её создаёт
///////////////////////////////////////////////////////////////////////////////////////////
if(directory.cd(QString("/root/.WiFiHostapdAP/logs/%1/").arg(data))) {
    QFile LogFile(QString("/root/.WiFiHostapdAP/logs/%1/log.txt").arg(data));
    if(LogFile.open(QIODevice::ReadOnly)) {
    ui->LogView->verticalHeader()->setVisible(true);
    ui->LogView->horizontalHeader()->setVisible(true);
    ui->LogView->setRowCount(0);


    while(!LogFile.atEnd()) {
    AllTextFromFile = LogFile.readLine();
    size = AllTextFromFile.length();
    LineArray = AllTextFromFile.split("|");
    if(LineArray.size()==4 && size >10) {
    ui->LogView->setRowCount(ui->LogView->rowCount()+1);
///////////////////////////////////////////////////////////////////////////////////////////
// Меняем местами всякую ересь
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// Тип
///////////////////////////////////////////////////////////////////////////////////////////
    item = new QTableWidgetItem();
    if(LineArray.at(3).toInt()==1){
        item->setIcon(QIcon(QPixmap(":/pic/icons/dialog-ok-apply.png")));
        item->setText(tr("OK"));
    }
    else if(LineArray.at(3).toInt()==2) {
        item->setIcon(QIcon(QPixmap(":/pic/icons/application-exit.png")));
        item->setText(tr("ERROR"));
    }
    else {
        item->setIcon(QIcon(QPixmap(":/pic/icons/dialog-information.png")));
        item->setText(tr("INFO"));   }

    ui->LogView->setItem(i,0,item);

///////////////////////////////////////////////////////////////////////////////////////////
// Отправитель
///////////////////////////////////////////////////////////////////////////////////////////
    item = new QTableWidgetItem(LineArray.at(0));
    ui->LogView->setItem(i,1,item);
    ui->LogView->item(i,1)->setToolTip(LineArray.at(0));

///////////////////////////////////////////////////////////////////////////////////////////
// Время
///////////////////////////////////////////////////////////////////////////////////////////
    DateStep = new QDateTime;
    DateStep->setTime_t(LineArray.at(1).toInt());
    item = new QTableWidgetItem(DateStep->toString("hh:mm:ss"));
    ui->LogView->setItem(i,2,item); // время
    ui->LogView->item(i,2)->setToolTip(QString("%1 %2").arg(ui->DataChooser->currentText(), DateStep->toString("hh:mm:ss")));

///////////////////////////////////////////////////////////////////////////////////////////
// Сообщение
///////////////////////////////////////////////////////////////////////////////////////////
    item = new QTableWidgetItem(LineArray.at(2));
    ui->LogView->setItem(i,3,item);
    ui->LogView->item(i,3)->setToolTip(LineArray.at(2));
    i++;
}
    }

LogFile.close();
    }

} else {
    ui->DataChooser->clear();
    InstallLogToday();
    //TempString  = dateNow.currentDate().toString("d.MM.yyyy");
    //AddRowsToTable(TempString);
}

}

void LogSystem::on_DelLog_clicked()
{
QString FileToDll;
FileToDll = ui->DataChooser->currentText();
QFile::remove(QString("/root/.WiFiHostapdAP/logs/%1/log.txt").arg(FileToDll));
QDir dir;
dir.rmdir(QString("/root/.WiFiHostapdAP/logs/%1/").arg(FileToDll));
InstallLogToday();
}

void LogSystem::on_DataChooser_currentIndexChanged(const QString &arg1)
{
    QDate dateNow;
    QString TempString = dateNow.currentDate().toString("yyyy.MM.d");;
    if(TempString == arg1)
        ui->DelLog->setEnabled(false);
    else
        ui->DelLog->setEnabled(true);
    AddRowsToTable(arg1);
}
