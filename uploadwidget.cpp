#include "uploadwidget.h"
#include "ui_UploadWidget.h"
#include <QPushButton>
#include <QDebug>
#include <QQmlEngine>

UploadWidget::UploadWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::UploadWidget)
{
    ui->setupUi(this);
    ui->MultiBtn->setText("Prepare");
    this->tunnel = new Tunnel(this, "upload");
    this->state = 0;
    QObject::connect(ui->MultiBtn, &QPushButton::clicked, this, [=](){this->MultiBtnHandler();});
    QObject::connect(this->tunnel, &Tunnel::registrationSignal, this, &UploadWidget::registrationHandler);
    QObject::connect(this->tunnel, &Tunnel::sendinfoSignal, this, &UploadWidget::sendinfoHandler);
    QObject::connect(this->tunnel, &Tunnel::errorhandlersignal, this, &UploadWidget::errorHandler);
    QObject::connect(this->tunnel, &Tunnel::renderingdone, this, &UploadWidget::render);
    QObject::connect(this->tunnel, &Tunnel::operationdone, this, &UploadWidget::uploadDoneHandler);
    this->addUploadFileWidget();
}

UploadWidget::~UploadWidget()
{
    delete ui;
}

void UploadWidget::render() {
    for (quint64 i = 0; i < this->tunnel->files; i++) {
        quint64 speed = (*this->tunnel->speed)[i];
        qint8 progress = (*this->tunnel->progress)[i];
        quint64 bytesuploaded = (*this->tunnel->bytesuploaded)[i];
        UploadFileWidget *ufw = (UploadFileWidget*)this->UploadFileWidgetsList->value(i);
        ufw->setSpeed(speed);
        ufw->setProgress(progress);
        ufw->setBytesProgress(bytesuploaded);
    }
}

void UploadWidget::switchInterfaceChangeability(bool status) {
    for (qint64 i = 0; i < this->UploadFileWidgetsList->length(); i++) {
        UploadFileWidget *ufw = (UploadFileWidget*)this->UploadFileWidgetsList->value(i);
        ufw->switchInterfaceChangeability(status);
    }
}

void UploadWidget::addUploadFileWidget() {
    UploadFileWidget *ufw = new UploadFileWidget(this);
    QObject::connect(ufw, &UploadFileWidget::selectedFileNames, this, &UploadWidget::selectedFileNamesHandler);
    this->UploadFileWidgetsList->append(ufw);
    this->ui->UploadFileWidgetsLayout->addWidget(ufw);
}

void UploadWidget::selectedFileNamesHandler(QStringList files) {
    if (files.length() == 0) {
        if (this->tunnel->rnum == -1)
            this->state = 0;
        else
            this->state = 1;
        return;
    }
    this->state = 2;
    if (files.length() > 1) {
        this->ui->UploadFileWidgetsLayout->removeWidget((UploadFileWidget*)this->sender());
        this->UploadFileWidgetsList->remove(this->UploadFileWidgetsList->indexOf((UploadFileWidget*)this->sender()));
        ((UploadFileWidget*)this->sender())->deleteLater();
        ((UploadFileWidget*)this->sender())->hide();
        for (qint64 i=0; i < files.length(); i++) {
            this->addUploadFileWidget();
            ((UploadFileWidget*)this->UploadFileWidgetsList->last())->setFile(files[i]);
        }
        this->addUploadFileWidget();
    } else {
        if (!((UploadFileWidget*)this->sender())->filled)
            this->addUploadFileWidget();
        ((UploadFileWidget*)this->sender())->setFile(files[0]);
    }
    QStringList allFiles;
    for (qint64 i = 0; i < this->UploadFileWidgetsList->length() - 1; i++) {
        UploadFileWidget *ufw = (UploadFileWidget*)this->UploadFileWidgetsList->value(i);
        allFiles.append(ufw->path);
    }
    this->tunnel->setFiles(allFiles);
}

void UploadWidget::setRnum(quint16 rnum) {
    this->ui->tokenLabel->setText(QString::number(rnum));
}

void UploadWidget::setStatusBarText(QString text) {
    this->ui->StatusBar->setText(text);
}

void UploadWidget::MultiBtnHandler() {
    this->ui->MultiBtn->setDisabled(true);
    if (this->state == 0) {
        this->tunnel->registration();
    } else if (this->state == 2) {
        if (this->tunnel->rnum == -1) {
            this->tunnel->registration();
            return;
        }
        this->switchInterfaceChangeability(false);
        this->tunnel->sendinfo();
    } else if (this->state == 3) {
        this->switchInterfaceChangeability(false);
        this->tunnel->start();
    }
}

void UploadWidget::registrationHandler(qint16 state) {
    this->ui->MultiBtn->setEnabled(true);
    if (state == 1) {
        this->setStatusBarText("registration done");
        this->setRnum(this->tunnel->rnum);
        this->ui->MultiBtn->setText("Prepare");
        if (this->state != 2)
            this->state = 1;
        else {
            this->ui->MultiBtn->setDisabled(true);
            this->switchInterfaceChangeability(false);
            this->tunnel->sendinfo();
        }
        // ++this->state; // ToDo deleteLater after adding choiceFile
    }
    if (state == -1) {
        this->setStatusBarText("Connection Error");
    }
    if (state == -500) {
        this->setStatusBarText("Application Error");
    }
}

void UploadWidget::sendinfoHandler(qint16 state) {
    this->ui->MultiBtn->setEnabled(true);
    this->switchInterfaceChangeability(true);
    if (state == 1) {
        this->setStatusBarText("send info done");
        this->ui->MultiBtn->setText("Start");
        ++this->state;
    }
    if (state == -1) {
        this->setStatusBarText("Connection Error");
    }
    if (state == -2) {
        this->ui->MultiBtn->setDisabled(true);
        this->setStatusBarText("Access Denied");
    }
    if (state == -500) {
        this->setStatusBarText("Application Error");
    }
}

void UploadWidget::uploadDoneHandler() {
    this->ui->StatusBar->setText("Uploading Done");
}

void UploadWidget::errorHandler(qint64 state) {
    this->ui->MultiBtn->setEnabled(true);
    this->ui->MultiBtn->setText("Resume");
    if (state == -1) {
        this->setStatusBarText("Connection Error");
    }
    if (state == -2) {
        this->ui->MultiBtn->setDisabled(true);
        this->setStatusBarText("Access Denied");
    }
    if (state == -500) {
        this->setStatusBarText("Application Error");
    }
}

void UploadWidget::updateColorTheme(SettingsStruct settings) {
    this->setStyleSheet(settings.ui_colortheme_index == 0 ? settings.QSS->value("FloatingWhite") : settings.ui_colortheme_index == 1 ? settings.QSS->value("FloatingBlack") : "");
    this->shadowEffect.setOffset(QPoint(8, 11));
    this->shadowEffect.setBlurRadius(12);
    this->shadowEffect.setColor(settings.ui_colortheme_index == 0 ? QColor(200, 200, 200) : settings.ui_colortheme_index == 1 ? QColor(20, 20, 20) : QColor(0, 0, 0));
    this->setGraphicsEffect(&this->shadowEffect);
    this->ui->QRBtn->setIcon(QIcon(settings.ui_colortheme_index == 0 ? ":/gui/Icons/QR_black.png" : settings.ui_colortheme_index == 1 ? ":/gui/Icons/QR_white.png" : ""));
    this->ui->DelBtn->setIcon(QIcon(settings.ui_colortheme_index == 0 ? ":/gui/Icons/Trash_black.png" : settings.ui_colortheme_index == 1 ? ":/gui/Icons/Trash_white.png" : ""));
}

void UploadWidget::settingsHandler(SettingsStruct settings) {
    if (this->state == 0 or (this->state == 2 && this->tunnel->rnum == -1)) {
        this->tunnel->server = settings.server;
        this->ui->serverLabel->setText(settings.server);
    }
    if (this->state < 3) {
        this->tunnel->RAM = settings.request_RAM;
        this->tunnel->threads = settings.request_threads;
        this->tunnel->chunksize = settings.request_chunksize;
    }
}
