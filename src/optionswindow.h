#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include <QDialog>
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QTextIStream>
#include <QDir>
#include <QDateTime>
#include "cstdio"

namespace Ui {
    class OptionsWindow;
}

class OptionsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsWindow(QWidget *parent = 0);
    ~OptionsWindow();
    void set_Settings();
    int xrand(int N);
    void SaveAConfig(int step,  bool nomesseage);
    QString getMaskToConfig(int step);

private slots:
    void on_RestoreProg_clicked();

    void on_RestoreAP_clicked();

    void on_RestoreDHCP_clicked();

    void on_ProgSave_clicked();

public slots:
    void on_APSave_clicked(bool nomesseage);

    void on_DHCPSave_clicked(bool nomesseage);

signals:
    void toMainLog(QString);




private:
    Ui::OptionsWindow *ui;
};

#endif // OPTIONSWINDOW_H
