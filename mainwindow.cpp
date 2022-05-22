#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "uploadwidget.h"
#include <QFile>
#include <QDebug>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QWidget{parent}, ui(new Ui::MainWindow)
{
    QFile White(":/gui/White.qss");
    White.open(QFile::ReadOnly);
    this->StyleSheet->insert("White", White.readAll());
    White.close();

    QFile FloatingWhite(":/gui/FloatingWhite.qss");
    FloatingWhite.open(QFile::ReadOnly);
    this->StyleSheet->insert("FloatingWhite", FloatingWhite.readAll());
    FloatingWhite.close();

    QFile Black(":/gui/Black.qss");
    Black.open(QFile::ReadOnly);
    this->StyleSheet->insert("Black", Black.readAll());
    Black.close();

    QFile FloatingBlack(":/gui/FloatingBlack.qss");
    FloatingBlack.open(QFile::ReadOnly);
    this->StyleSheet->insert("FloatingBlack", FloatingBlack.readAll());
    FloatingBlack.close();

    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/gui/Icons/P2P.png"));
    this->setWindowTitle(this->APPLICATION_NAME);
    this->ssw = new ServerSettingsWidget(this);
    ui->SettingsLayout->addWidget(this->ssw);
    this->rpsw = new RequestParamsSettingsWidget(this);
    ui->SettingsLayout->addWidget(this->rpsw);
    this->uisw = new UiSettingsWidget(this);
    ui->SettingsLayout->addWidget(this->uisw);
    this->cvw = new ClientVersionWidget(this);
    ui->SettingsLayout->addWidget(this->cvw);
    this->cvw->setVersion(this->APPLICATION_VERSION);
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->SettingsLayout->addItem(spacer);

    QObject::connect(ui->AddUploadBtn, &QPushButton::clicked, this, [=](){this->createUploadWidget();});
    QObject::connect(this->ssw, &ServerSettingsWidget::settingsChanged, this, &MainWindow::settingsHandler);
    QObject::connect(this->rpsw, &RequestParamsSettingsWidget::settingsChanged, this, &MainWindow::settingsHandler);
    QObject::connect(this->uisw, &UiSettingsWidget::settingsChanged, this, &MainWindow::settingsHandler);

    this->settingsInit();
}

void MainWindow::settingsInit() {
    quint64 KB = 1024;
    quint64 MB = 1024 * KB;
    this->settings = new QSettings(this->COMPANY_NAME, this->INTERNAL_APPLICATION_NAME);
    this->appsettings.server_index = this->settings->value("settings/settingsTab/server_index", QVariant(0)).toInt();

    this->appsettings.another_server_ip = this->settings->value("settings/settingsTab/another_server_ip", QVariant("")).toString();
    this->appsettings.another_server_port = this->settings->value("settings/settingsTab/another_server_port", QVariant(8001)).toInt();

    this->appsettings.request_RAM = this->settings->value("settings/settingsTab/request_RAM", QVariant((unsigned long long)64 * MB)).toULongLong();
    this->appsettings.request_chunksize = this->settings->value("settings/settingsTab/request_chunksize", QVariant((unsigned long long)2 * MB)).toULongLong();
    this->appsettings.request_threads = this->settings->value("settings/settingsTab/request_threads", QVariant((uint16_t)16)).toUInt();

    this->appsettings.ui_colortheme_index = this->settings->value("settings/settingsTab/ui_colortheme_index", QVariant(0)).toInt();

    this->appsettings.QSS = this->StyleSheet;
    this->settingsColorThemeSet();
}

void MainWindow::settingsSave() {
    QMap<QString, QVariant> sswS = this->ssw->getSettings();
    QMap<QString, QVariant> rpswS = this->rpsw->getSettings();
    QMap<QString, QVariant> uiswS = this->uisw->getSettings();

    this->appsettings.server = this->ssw->getServer();

    this->appsettings.server_index = sswS["server_index"].toInt();
    this->settings->setValue("settings/settingsTab/server_index", QVariant(this->appsettings.server_index));

    this->appsettings.another_server_ip = sswS["another_server_ip"].toString();
    this->appsettings.another_server_port = sswS["another_server_port"].toInt();
    this->settings->setValue("settings/settingsTab/another_server_ip", sswS["another_server_ip"]);
    this->settings->setValue("settings/settingsTab/another_server_port", sswS["another_server_port"]);

    this->appsettings.request_RAM = rpswS["request_RAM"].toULongLong();
    this->appsettings.request_chunksize = rpswS["request_chunksize"].toULongLong();
    this->appsettings.request_threads = rpswS["request_threads"].toUInt();
    this->settings->setValue("settings/settingsTab/request_RAM", rpswS["request_RAM"]);
    this->settings->setValue("settings/settingsTab/request_chunksize", rpswS["request_chunksize"]);
    this->settings->setValue("settings/settingsTab/request_threads", rpswS["request_threads"]);

    this->appsettings.ui_colortheme_index = uiswS["ui_colortheme_index"].toInt();
    this->settings->setValue("settings/settingsTab/ui_colortheme_index", uiswS["ui_colortheme_index"]);    
    //this->settings->sync();
}

void MainWindow::settingsColorThemeSet() {
    QString QSS;
    QString FloatingQSS;
    switch(this->appsettings.ui_colortheme_index) {
    case 0:
        QSS = this->appsettings.QSS->value("White");
        FloatingQSS = this->appsettings.QSS->value("FloatingWhite");
        break;
    case 1:
        QSS = this->appsettings.QSS->value("Black");
        FloatingQSS = this->appsettings.QSS->value("FloatingBlack");
        break;
    }
    this->setStyleSheet(QSS);
    this->ssw->setStyleSheet(FloatingQSS);
    this->rpsw->setStyleSheet(FloatingQSS);
    this->uisw->setStyleSheet(FloatingQSS);
    this->cvw->setStyleSheet(FloatingQSS);  // todo: remove Unknown property text
    for (quint64 i = 0; i < (quint64)this->UploadWidgetsList->length(); i++) {
        ((UploadWidget*)(*this->UploadWidgetsList)[i])->updateColorTheme(this->appsettings);
    }
}

void MainWindow::settingsHandler() {
    this->settingsSave();
    if (this->sender() == this->uisw)
        this->settingsColorThemeSet();
    else {
        for (quint64 i = 0; i < (quint64)this->UploadWidgetsList->length(); i++) {
            ((UploadWidget*)(*this->UploadWidgetsList)[i])->settingsHandler(this->appsettings);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createUploadWidget() {
    UploadWidget *upw = new UploadWidget(this);
    upw->setStyleSheet(this->StyleSheet->value("FloatingWhite"));
    upw->updateColorTheme(this->appsettings);
    upw->settingsHandler(this->appsettings);
    ui->UploadLayout->insertWidget(ui->UploadLayout->count() - 1, upw);
    this->UploadWidgetsList->append(upw);
}
