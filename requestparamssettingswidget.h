#ifndef REQUESTPARAMSSETTINGSWIDGET_H
#define REQUESTPARAMSSETTINGSWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RequestParamsSettingsWidget; }
QT_END_NAMESPACE

class RequestParamsSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    RequestParamsSettingsWidget(QWidget *parent = nullptr);
    ~RequestParamsSettingsWidget();
    QMap<QString, QVariant> getSettings();
    Ui::RequestParamsSettingsWidget *ui;
signals:
    void settingsChanged();
};

#endif // REQUESTPARAMSSETTINGSWIDGET_H
