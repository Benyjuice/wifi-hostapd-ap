#include "dialogindicator.h"
#include "ui_dialogindicator.h"


int Do = 0; // активация по-умолчанию
int Progress = 0;

DialogIndicator::DialogIndicator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIndicator)
{

    ui->setupUi(this);
    // выключаем кнопку выхода
    ui->ExitDialog->setEnabled(false);
    ui->Log->clear();
}

DialogIndicator::~DialogIndicator()
{
    delete ui;
}

void DialogIndicator::SetDo(int i)
{


    if(i==0) {
        // Активируем
         ActivateAP *t1 = new ActivateAP();
         connect(t1, SIGNAL(reset_status_active()), this, SLOT(FromIndicator()));
         connect(t1, SIGNAL(setPercent(int)), this, SLOT(PercentGet(int)));
         connect(t1, SIGNAL(send_step(QString)), this, SLOT(getSrep(QString)));
         connect(t1, SIGNAL(toLog(QString)), this, SLOT(FromThreadToLog(QString)));
        t1->start();
        t1->exit();


    } else {
        // диактивируем
         DisActiveAP *t2 = new DisActiveAP();
         connect(t2, SIGNAL(reset_status_disactive()), this, SLOT(FromIndicator()));
         connect(t2, SIGNAL(setPercent(int)), this, SLOT(PercentGet(int)));
         connect(t2, SIGNAL(send_step(QString)), this, SLOT(getSrep(QString)));
         connect(t2, SIGNAL(toLog(QString)), this, SLOT(FromThreadToLog(QString)));
         t2->start();
        t2->exit();


    }

}

void DialogIndicator::getSrep(QString text) {
// Получаем сообщение из потока
// устанавлием его в лейбел
ui->Log->setText(text);
text.clear();
}

void DialogIndicator::PercentGet(int val) {
    ui->ProgressDo->setValue(val);
}

void DialogIndicator::FromIndicator(){
emit to_Mainwindow();
ui->ExitDialog->setEnabled(true);
}

void DialogIndicator::FromThreadToLog(QString str){
    emit toLogInMainWindow(str);
}
