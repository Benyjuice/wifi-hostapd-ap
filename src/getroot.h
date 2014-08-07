#ifndef GETROOT_H
#define GETROOT_H

#include <QDialog>
#include <QDir>
#include <QSettings>

namespace Ui {
    class GetRoot;
}

class GetRoot : public QDialog
{
    Q_OBJECT

public:
    explicit GetRoot(QWidget *parent = 0);
    ~GetRoot();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GetRoot *ui;
};

#endif // GETROOT_H
