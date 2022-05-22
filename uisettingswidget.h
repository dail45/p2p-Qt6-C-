#ifndef UISETTINGSWIDGET_H
#define UISETTINGSWIDGET_H

#include <QWidget>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui { class UiSettingsWidget; }
QT_END_NAMESPACE

class UiSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    UiSettingsWidget(QWidget *parent = nullptr);
    ~UiSettingsWidget();
    QMap<QString, QVariant> getSettings();
    Ui::UiSettingsWidget *ui;

signals:
    void settingsChanged();
};

#endif // UISETTINGSWIDGET_H
