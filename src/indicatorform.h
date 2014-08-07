#ifndef INDICATORFORM_H
#define INDICATORFORM_H

#include <QWidget>
#include <disactiveap.h>
#include <activateap.h>
#include <mainwindow.h>

namespace Ui {
    class IndicatorForm;
}

class IndicatorForm : public QWidget
{
    Q_OBJECT

public:
    explicit IndicatorForm(QWidget *parent = 0);
    ~IndicatorForm();
    void SetDo(int i);

private slots:
    void aas(QString ll);
    void FromIndicator();

private:
    Ui::IndicatorForm *ui;

signals:
    void to_Mainwindow();
    void ShowMainWindow();

};

#endif // INDICATORFORM_H
