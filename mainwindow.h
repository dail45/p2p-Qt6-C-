#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QSettings>
#include "serversettingswidget.h"
#include "requestparamssettingswidget.h"
#include "uisettingswidget.h"
#include "clientversionwidget.h"
#include "settings_struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QWidget *> *UploadWidgetsList = new QList<QWidget *>;
    QMap<QString, QString> *StyleSheet = new QMap<QString, QString>;
    void createUploadWidget();

private:
    void settingsInit();
    void settingsSave();
    void settingsColorThemeSet();
    void settingsHandler();
    QString COMPANY_NAME = "dail45";
    QString COMPANY_DOMAIN = "dail45.ru";
    QString APPLICATION_NAME = "p2p";
    QString INTERNAL_APPLICATION_NAME = "p2p (C++)";
    QString APPLICATION_VERSION = "0.3";
    SettingsStruct appsettings;
    QSettings *settings;
    Ui::MainWindow *ui;
    ServerSettingsWidget *ssw;
    RequestParamsSettingsWidget *rpsw;
    UiSettingsWidget *uisw;
    ClientVersionWidget *cvw;

};

#endif // MAINWINDOW_H
