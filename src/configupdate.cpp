#include "configupdate.h"
#include <QFile>
#include <QSettings>
#include <QString>
#include <QThread>

class ConfigUpdate : public QThread
{     Q_OBJECT
  protected:
    void run();
  private:
      int MaskType, Config;
  public:
        ConfigUpdate()
        { MaskType = 0; /* Выполняем обновление по стандартной маске */
          Config = 0; /* Обновить все конфигурационные файлы */
        };
        ConfigUpdate(int Mask)
        { MaskType = Mask; /* Выполняем обновление указанной маске */
          Config = 0; /* Обновить все конфигурационные файлы */
        };
        ConfigUpdate(int Mask, int Set)
        { MaskType = Mask; /* Выполняем обновление указанной маске */
          Config = Set; /* Обновить только указанный конфигурационный файл */
        };
};

void ConfigUpdate::run() {
QSettings *GlobalSettings = new QSettings("WiFi_Hostapd_AP.conf",QSettings::NativeFormat);
// Класс отвечает за обновление конфигурационных файлов. Его цель - грамотно записать в файл данные
// Мы будем писать по шаблону. Шаблон задаётся пользователем. Его мы и используем.
// Шаблоны хранятся в области памяти, либо в файлах.
// Берём шаблон, читаем его. Заменяем, перезаписываем конфиш.




delete GlobalSettings;
    }
