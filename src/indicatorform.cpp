#include "indicatorform.h"
#include "ui_indicatorform.h"

int Do = 0; // активация по-умолчанию
int Progress = 0;

IndicatorForm::IndicatorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndicatorForm)
{
    ui->setupUi(this);
    // выключаем кнопку выхода
    ui->ExitDialog->setEnabled(false);
    ui->Log->clear();

}

IndicatorForm::~IndicatorForm()
{
    //emit ShowMainWindow();
    delete ui;

}

void IndicatorForm::SetDo(int i)
{


    if(i==0) {
        // Активируем
         ActivateAP *t1 = new ActivateAP();
         connect(t1, SIGNAL(gotten_text(const QString)), this, SLOT(aas(const QString)));
         connect(t1, SIGNAL(reset_status_active()), this, SLOT(FromIndicator()));
        t1->start();
        t1->exit();


    } else {
        // диактивируем
         DisActiveAP *t2 = new DisActiveAP();
         connect(t2, SIGNAL(gotten_text(const QString)), this, SLOT(aas(const QString)));
         connect(t2, SIGNAL(reset_status_disactive()), this, SLOT(FromIndicator()));
         t2->start();
        t2->exit();


    }

}

void IndicatorForm::aas(QString ll) {
QString a;


if(ll.startsWith("PERCENT") && ll!="exec:YES" && ll!="exec:FAIL") {
// Тут содержется процент
QStringList t = ll.split(":");
a = t.at(1);
if(a.toInt()>Progress) { Progress = a.toInt(); ui->ProgressDo->setValue(Progress); } // Вдруг там бред?

} else {
/* В этой строке содержется команда */
    if(ll!="exec:YES" && ll!="exec:FAIL")
        ui->Log->setText(ll); // Добавляем в лог
    if(a.toInt()>Progress) { Progress = a.toInt(); ui->ProgressDo->setValue(Progress); } // Вдруг там бред?
}


QMessageBox msgBox;
if(ll=="exec:YES") {
    ui->ProgressDo->setValue(100);
    sleep(1);
    ui->Log->setText("Done!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(tr("Complite sucessfully!"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

if(ll.startsWith("exec") && ll.endsWith("Error")) {

    ui->Log->setText("Error!");
    sleep(1);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(tr("Error! Please, see log file to more information!"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();

}



}

void IndicatorForm::FromIndicator(){
emit to_Mainwindow();
ui->ExitDialog->setEnabled(true);
}
