#include "uploadfilewidget.h"
#include "ui_UploadFileWidget.h"
#include <QFileDialog>
#include <QStringList>
#include <QFileInfo>

#ifdef Q_OS_ANDROID
#include <QJniObject>
#endif

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::UploadFileWidget)
{
    ui->setupUi(this);
    ui->delBtn->hide();
    ui->sizeLabel->hide();
    ui->pathLabel->hide();
    ui->timeLabel->hide();
    ui->speedLabel->hide();
    ui->progressBar->hide();
    QObject::connect(ui->filenameBtn, &QPushButton::clicked, this, &UploadFileWidget::selectFiles);
}

UploadFileWidget::~UploadFileWidget()
{
    delete ui;
}

UploadFileWidget::FormatedSize UploadFileWidget::formatSize(quint64 unformatSize) {
    FormatedSize res;
    res.unformated = unformatSize;
    res.formated = unformatSize;
    if (res.formated > 1024) {
        res.formated /= (qreal)1024;
        res.value = "KB";
        res.intValue = 1;
    }
    if (res.formated > 1024) {
        res.formated /= (qreal)1024;
        res.value = "MB";
        res.intValue = 2;
    }
    if (res.formated > 1024) {
        res.formated /= (qreal)1024;
        res.value = "GB";
        res.intValue = 3;
    }
    return res;
}

void UploadFileWidget::setFile(QString filename) {
    qDebug() << "UFW: setFile";
    #ifdef Q_OS_ANDROID
    qDebug("================================================================================================");

    auto activity = QJniObject(QNativeInterface::QAndroidApplication::context());
    QJniObject MyJni = QJniObject("org/dail45/p2p/MyJNI",
                                  "(Landroid/app/Activity;)V",
                                  activity.object<jobject>());
    jint fd = MyJni.callMethod<jint>("getFdFromString", "(Ljava/lang/String;)I", QJniObject::fromString(filename).object<jobject>());
    QJniObject fname = MyJni.callObjectMethod("getFileNameFromString", "(Ljava/lang/String;)Ljava/lang/String;", QJniObject::fromString(filename).object<jobject>());
    QFile *file = new QFile();
    file->open(fd, QFile::ReadOnly);
    quint64 size = file->size();
    file->deleteLater();
    this->filename = fname.toString();
    this->path = filename;
    QString path = "content:/";

    qDebug("================================================================================================");
    //QJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
    #else
    QFile *file = new QFile(filename);
    quint64 size = file->size();
    this->filename = filename.split("/").last();
    this->path = filename;
    QStringList tpath = filename.split("/");
    tpath.removeLast();
    QString path = tpath.join("/");
    #endif

    FormatedSize fsize = this->formatSize(size);
    this->totalLength = fsize;

    QString ssize = "";
    ssize += "0 B /" + QString::number(fsize.formated, 'f', 2) + " " + fsize.value;
    ui->sizeLabel->setText(ssize);

    ui->pathLabel->setText(path);
    ui->pathLabel->setWordWrap(true);

    ui->filenameBtn->setText(this->filename);

    ui->progressBar->setValue(0);

    ui->delBtn->show();
    ui->sizeLabel->show();
    ui->pathLabel->show();
    ui->timeLabel->show();
    ui->speedLabel->show();
    ui->progressBar->show();
    ui->horizontalLayout->removeItem(ui->startSpacer);

    this->filled = true;
}

void UploadFileWidget::switchInterfaceChangeability(bool status) {
    this->ui->filenameBtn->setEnabled(status);
    this->ui->delBtn->setEnabled(status);
}

void UploadFileWidget::selectFiles() {
    qDebug() << "UFW: selectFiles";
    QFileDialog *dialog = new QFileDialog(this);
    QStringList files = dialog->getOpenFileNames();
    dialog->deleteLater();
    emit this->selectedFileNames(files);
}

void UploadFileWidget::setSpeed(quint64 speed) {
    FormatedSize fspeed = this->formatSize(speed);
    QString tspeed = "";
    tspeed += QString::number(fspeed.formated, 'f', 2) + " " + fspeed.value + "/s";
    this->ui->speedLabel->setText(tspeed);
}

void UploadFileWidget::setProgress(qint8 progress) {
    this->ui->progressBar->setValue(progress);
}

void UploadFileWidget::setBytesProgress(quint64 bytesProgress) {
    FormatedSize fbytesProgress = this->formatSize(bytesProgress);
    QString ssize = "";
    ssize += QString::number(fbytesProgress.formated, 'f', 2) + " " + fbytesProgress.value + " / " \
            + QString::number(this->totalLength.formated, 'f', 2) + " " + this->totalLength.value;
    ui->sizeLabel->setText(ssize);
}
