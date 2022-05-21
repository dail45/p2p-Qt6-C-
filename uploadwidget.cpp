#include "uploadwidget.h"
#include "ui_UploadWidget.h"
#include <QPushButton>
#include <QDebug>

UploadWidget::UploadWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::UploadWidget)
{
    ui->setupUi(this);
    this->tunnel = new Tunnel(this, "upload");
    this->state = 0;
    QObject::connect(ui->MultiBtn, &QPushButton::clicked, this, [=](){this->MultiBtnHandler();});
    QObject::connect(this->tunnel, &Tunnel::registrationSignal, this, &UploadWidget::registrationHandler);
    QObject::connect(this->tunnel, &Tunnel::sendinfoSignal, this, &UploadWidget::sendinfoHandler);
    QObject::connect(this->tunnel, &Tunnel::errorhandlersignal, this, &UploadWidget::errorHandler);

}

UploadWidget::~UploadWidget()
{
    delete ui;
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
        return;
    } else if (this->state == 2) {
        this->tunnel->sendinfo();
    } else if (this->state == 3) {
        this->tunnel->start();
    }
}

void UploadWidget::registrationHandler(qint16 state) {
    this->ui->MultiBtn->setEnabled(true);
    if (state == 1) {
        this->setStatusBarText("registration done");
        this->setRnum(this->tunnel->rnum);
        this->ui->MultiBtn->setText("Prepare");
        ++this->state;
        ++this->state; // ToDo deleteLater after adding choiceFile
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
