#ifndef REQUESTPARAMSSETTINGSWIDGET_H
#define REQUESTPARAMSSETTINGSWIDGET_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include "settings_struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RequestParamsSettingsWidget; }
QT_END_NAMESPACE

class RequestParamsSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    RequestParamsSettingsWidget(QWidget *parent = nullptr);
    ~RequestParamsSettingsWidget();
    void setSettings(SettingsStruct settings);
    QMap<QString, QVariant> getSettings();
    Ui::RequestParamsSettingsWidget *ui;
    QGraphicsDropShadowEffect shadowEffect;
    void updateColorTheme(SettingsStruct settings);
signals:
    void settingsChanged();
};

#endif // REQUESTPARAMSSETTINGSWIDGET_H
