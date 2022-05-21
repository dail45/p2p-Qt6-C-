#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "uploadwidget.h"
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->AddUploadBtn, &QPushButton::clicked, this, [=](){this->createUploadWidget();});

    QFile White(":/gui/White.qss");
    White.open(QFile::ReadOnly);
    this->StyleSheet->insert("White", White.readAll());
    White.close();

    QFile FloatingWhite(":/gui/FloatingWhite.qss");
    FloatingWhite.open(QFile::ReadOnly);
    this->StyleSheet->insert("FloatingWhite", FloatingWhite.readAll());
    FloatingWhite.close();

    QFile Black(":/gui/White.qss");
    Black.open(QFile::ReadOnly);
    this->StyleSheet->insert("Black", Black.readAll());
    Black.close();

    QFile FloatingBlack(":/gui/FloatingBlack.qss");
    FloatingBlack.open(QFile::ReadOnly);
    this->StyleSheet->insert("FloatingBlack", FloatingBlack.readAll());
    FloatingBlack.close();

    this->setStyleSheet(this->StyleSheet->value("White"));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createUploadWidget() {
    UploadWidget *upw = new UploadWidget(this);
    upw->setStyleSheet(this->StyleSheet->value("FloatingWhite"));
    ui->UploadLayout->insertWidget(ui->UploadLayout->count() - 1, upw);
    this->UploadWidgetsList->append(upw);
}
