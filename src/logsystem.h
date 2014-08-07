#ifndef LOGSYSTEM_H
#define LOGSYSTEM_H

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QDate>
#include <QSettings>
#include <QTextIStream>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>

namespace Ui {
    class LogSystem;
}

class LogSystem : public QDialog
{
    Q_OBJECT

public:
    explicit LogSystem(QWidget *parent = 0);
    ~LogSystem();

private slots:
    void on_DataChooser_textChanged(const QString &arg1);
    void InstallLogToday();
    void AddRowsToTable(QString data);
    void on_DelLog_clicked();

    void on_DataChooser_currentIndexChanged(const QString &arg1);

private:
    Ui::LogSystem *ui;
};

#endif // LOGSYSTEM_H
