#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>
#include <QThread>
#include "tunnel.h"
#include "uploadfilewidget.h"
#include "settings_struct.h"
#include <QGraphicsDropShadowEffect>

QT_BEGIN_NAMESPACE
namespace Ui { class UploadWidget; }
QT_END_NAMESPACE

class UploadWidget : public QWidget
{
    Q_OBJECT
public:
    UploadWidget(QWidget *parent = nullptr);
    ~UploadWidget();
    Tunnel *tunnel;
    void addUploadFileWidget();
    void selectedFileNamesHandler(QStringList files);
    void setRnum(quint16 rnum);
    void MultiBtnHandler();
    quint8 state;
    void setStatusBarText(QString text);
    void registrationHandler(qint16 state);
    void sendinfoHandler(qint16 state);
    void uploadDoneHandler();
    void errorHandler(qint64 statusCode);
    void render();
    void switchInterfaceChangeability(bool status);
    void updateColorTheme(SettingsStruct settings);
    void settingsHandler(SettingsStruct settings);
    QGraphicsDropShadowEffect shadowEffect;
    QList<QWidget *> *UploadFileWidgetsList = new QList<QWidget *>;

private:
    Ui::UploadWidget *ui;
};

#endif // UPLOADWIDGET_H
