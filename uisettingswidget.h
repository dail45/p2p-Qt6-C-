#ifndef UISETTINGSWIDGET_H
#define UISETTINGSWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QGraphicsDropShadowEffect>
#include "settings_struct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UiSettingsWidget; }
QT_END_NAMESPACE

class UiSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    UiSettingsWidget(QWidget *parent = nullptr);
    ~UiSettingsWidget();
    void setSettings(SettingsStruct settings);
    QMap<QString, QVariant> getSettings();
    Ui::UiSettingsWidget *ui;
    QGraphicsDropShadowEffect shadowEffect;
    void updateColorTheme(SettingsStruct settings);
signals:
    void settingsChanged();
};

#endif // UISETTINGSWIDGET_H
