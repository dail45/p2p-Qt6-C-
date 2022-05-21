#ifndef UPLOADFILEWIDGET_H
#define UPLOADFILEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class UploadFileWidget; }
QT_END_NAMESPACE

class UploadFileWidget: public QWidget
{
    Q_OBJECT
public:
    UploadFileWidget(QWidget *parent=nullptr);
    ~UploadFileWidget();

private:
    Ui::UploadFileWidget *ui;

signals:


};

#endif // UPLOADFILEWIDGET_H
