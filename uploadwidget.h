#ifndef UPLOADWIDGET_H
#define UPLOADWIDGET_H

#include <QWidget>
#include <QThread>
#include "tunnel.h"
#include "uploadfilewidget.h"

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
    void setRnum(quint16 rnum);
    void MultiBtnHandler();
    quint8 state;
    void setStatusBarText(QString text);
    void registrationHandler(qint16 state);
    void sendinfoHandler(qint16 state);
    void errorHandler(qint64 statusCode);
    QList<QWidget *> *UploadFileWidgetsList = new QList<QWidget *>;

private:
    Ui::UploadWidget *ui;
};

#endif // UPLOADWIDGET_H
