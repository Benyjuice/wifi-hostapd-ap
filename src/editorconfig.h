#ifndef EDITORCONFIG_H
#define EDITORCONFIG_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>


namespace Ui {
    class EditorConfig;
}

class EditorConfig : public QDialog
{
    Q_OBJECT

public:
    explicit EditorConfig(QWidget *parent = 0);
    ~EditorConfig();

    int update_mask(int page);

    void replaseTags(QString &str);

private slots:
    void on_Type_Config_1_clicked(bool checked);

    void on_Type_Config_2_clicked(bool checked);

    void setTextCode(int state, int type, const  QString name);

    void on_Config_Set_View_1_clicked(bool checked);

    void on_Config_Set_View_2_clicked(bool checked);

    void on_save_mask_clicked();

    void on_mask_chooser_currentIndexChanged(QString );

    void on_delet_mask_clicked();

    void on_load_mask_clicked();

    void on_saveConfig_clicked();

private:
    Ui::EditorConfig *ui;
};

#endif // EDITORCONFIG_H
