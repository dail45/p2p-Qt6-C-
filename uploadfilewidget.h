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
    struct FormatedSize {
        quint64 unformated = 0;
        qreal formated = 0;
        QString value = "B";
        qint8 intValue = 0;
    };
    FormatedSize totalLength;
    QString path;
    QString filename;
    bool filled = false;
    FormatedSize formatSize(quint64 unformatSize);

    void selectFiles();
    void setFile(QString filename);
    void switchInterfaceChangeability(bool status);

    void setProgress(qint8 progress);
    void setSpeed(quint64 speed);
    void setBytesProgress(quint64 bytesProgress);

private:
    Ui::UploadFileWidget *ui;

signals:
    void selectedFileNames(QStringList filenames);

};

#endif // UPLOADFILEWIDGET_H
