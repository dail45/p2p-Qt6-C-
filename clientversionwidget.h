#ifndef CLIENTVERSIONWIDGET_H
#define CLIENTVERSIONWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientVersionWidget; }
QT_END_NAMESPACE

class ClientVersionWidget : public QWidget
{
    Q_OBJECT
public:
    ClientVersionWidget(QWidget *parent = nullptr);
    ~ClientVersionWidget();
    void setVersion(QString version);
    Ui::ClientVersionWidget *ui;

signals:

};

#endif // CLIENTVERSIONWIDGET_H
