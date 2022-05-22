#include "serversettingswidget.h"
#include "ui_ServerSettingsWidget.h"

ServerSettingsWidget::ServerSettingsWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::ServerSettingsWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->localHostRadio, &QRadioButton::clicked, this, &ServerSettingsWidget::settingsChanged);
    QObject::connect(ui->herokuRadio, &QRadioButton::clicked, this, &ServerSettingsWidget::settingsChanged);
    QObject::connect(ui->customRadio, &QRadioButton::clicked, this, &ServerSettingsWidget::settingsChanged);
    QObject::connect(ui->localHostPortSpinBox, &QSpinBox::valueChanged, this, &ServerSettingsWidget::settingsChanged);
    QObject::connect(ui->customHostComboBox, &QComboBox::currentTextChanged, this, &ServerSettingsWidget::settingsChanged);
    QObject::connect(ui->customPortSpinBox, &QSpinBox::valueChanged, this, &ServerSettingsWidget::settingsChanged);
}


ServerSettingsWidget::~ServerSettingsWidget()
{
    delete ui;
}

QMap<QString, QVariant> ServerSettingsWidget::getSettings() {
    QMap<QString, QVariant> res;
    res["server_index"] = this->ui->localHostRadio->isChecked() ? 0 : this->ui->herokuRadio->isChecked() ? 1 : 2;
    res["another_server_ip"] = this->ui->customHostComboBox->currentText();
    res["another_server_port"] = this->ui->customPortSpinBox->value();
    return res;
}

QString ServerSettingsWidget::getServer() {
    QMap<QString, QVariant> settings = this->getSettings();
    int server_index = settings.value("server_index").toInt();
    QString res = "";
    res += server_index == 0 ? this->ui->localHostLabel->text() :
                               server_index == 1 ? this->ui->herokuLabel->text() :
                                                   server_index == 2 ? this->ui->customHostComboBox->currentText() : "";
    res += server_index == 0 ? QString(":") + QString::number(this->ui->localHostPortSpinBox->value()) :
                               server_index == 2 ? QString(":") + QString::number(this->ui->customPortSpinBox->value()) : "";
    return res;
}
