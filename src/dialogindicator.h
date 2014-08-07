#ifndef DIALOGINDICATOR_H
#define DIALOGINDICATOR_H

#include <QDialog>
#include "disactiveap.h"
#include "activateap.h"
#include "mainwindow.h"
#include <QMessageBox>

namespace Ui {
    class DialogIndicator;
}


class DialogIndicator : public QDialog
{
    Q_OBJECT
private:
    Ui::DialogIndicator *ui;


public:
    explicit DialogIndicator(QWidget *parent = 0);
    ~DialogIndicator();
    void SetDo(int i);

private slots:
    void FromIndicator();
    void PercentGet(int val);
    void getSrep(QString text);
    void FromThreadToLog(QString str);

signals:
    void to_Mainwindow();
    void toLogInMainWindow(QString);


};

#endif // DIALOGINDICATOR_H
