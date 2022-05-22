#ifndef SERVERSETTINGSWIDGET_H
#define SERVERSETTINGSWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerSettingsWidget; }
QT_END_NAMESPACE

class ServerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    ServerSettingsWidget(QWidget *parent = nullptr);
    ~ServerSettingsWidget();
    QMap<QString, QVariant> getSettings();
    QString getServer();
    Ui::ServerSettingsWidget *ui;
signals:
    void settingsChanged();
};

#endif // MAINWINDOW_H
