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

void ServerSettingsWidget::setSettings(SettingsStruct settings) {
    switch(settings.server_index) {
    case 0:
        this->ui->localHostRadio->click();
        break;
    case 1:
        this->ui->herokuRadio->click();
        break;
    case 2:
        this->ui->customRadio->click();
        break;
    }
    this->ui->localHostPortSpinBox->setValue(settings.localhost_server_port);
    this->ui->customHostComboBox->setCurrentText(settings.another_server_ip);
    this->ui->customPortSpinBox->setValue(settings.another_server_port);
}

QMap<QString, QVariant> ServerSettingsWidget::getSettings() {
    QMap<QString, QVariant> res;
    res["server_index"] = this->ui->localHostRadio->isChecked() ? 0 : this->ui->herokuRadio->isChecked() ? 1 : 2;
    res["another_server_ip"] = this->ui->customHostComboBox->currentText();
    res["another_server_port"] = this->ui->customPortSpinBox->value();
    res["localhost_server_port"] = this->ui->localHostPortSpinBox->value();
    return res;
}

QString ServerSettingsWidget::getServer() {
    QMap<QString, QVariant> settings = this->getSettings();
    int server_index = settings.value("server_index").toInt();
    QString res = "";
    res += server_index == 0 ? this->ui->localHostLabel->text() :
                               server_index == 1 ? this->ui->herokuLabel->text() :
                                                   server_index == 2 ? this->ui->customHostComboBox->currentText() : "";
    QRegularExpression re("http[s]*://[0-9]{1,}[.][0-9]{1,}[.][0-9]{1,}[.][0-9]{1,}");
    if ((server_index == 0) || (server_index == 2 && re.match(this->ui->customHostComboBox->currentText()).hasMatch()))
        res += server_index == 0 ? QString(":") + QString::number(this->ui->localHostPortSpinBox->value()) :
                                server_index == 2 ? QString(":") + QString::number(this->ui->customPortSpinBox->value()) : "";
    return res;
}

void ServerSettingsWidget::updateColorTheme(SettingsStruct settings) {
    this->setStyleSheet(settings.ui_colortheme_index == 0 ? settings.QSS->value("FloatingWhite") : settings.ui_colortheme_index == 1 ? settings.QSS->value("FloatingBlack") : "");
    this->shadowEffect.setOffset(QPoint(8, 11));
    this->shadowEffect.setBlurRadius(12);
    this->shadowEffect.setColor(settings.ui_colortheme_index == 0 ? QColor(200, 200, 200) : settings.ui_colortheme_index == 1 ? QColor(20, 20, 20) : QColor(0, 0, 0));
    this->setGraphicsEffect(&this->shadowEffect);
}
