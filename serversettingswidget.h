#ifndef SERVERSETTINGSWIDGET_H
#define SERVERSETTINGSWIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include "settings_struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerSettingsWidget; }
QT_END_NAMESPACE

class ServerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    ServerSettingsWidget(QWidget *parent = nullptr);
    ~ServerSettingsWidget();
    void setSettings(SettingsStruct settings);
    QMap<QString, QVariant> getSettings();
    QString getServer();
    Ui::ServerSettingsWidget *ui;
    QGraphicsDropShadowEffect shadowEffect;
    void updateColorTheme(SettingsStruct settings);
signals:
    void settingsChanged();
};

#endif // MAINWINDOW_H
