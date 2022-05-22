#include "requestparamssettingswidget.h"
#include "ui_RequestParamsSettingsWidget.h"

RequestParamsSettingsWidget::RequestParamsSettingsWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::RequestParamsSettingsWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->serverRAMSpinBox, &QSpinBox::valueChanged, this, &RequestParamsSettingsWidget::settingsChanged);
    QObject::connect(ui->serverRAMTypeComboBox, &QComboBox::currentIndexChanged, this, &RequestParamsSettingsWidget::settingsChanged);
    QObject::connect(ui->chunkSizeSpinBox, &QSpinBox::valueChanged, this, &RequestParamsSettingsWidget::settingsChanged);
    QObject::connect(ui->chunkSizeTypeComboBox, &QComboBox::currentIndexChanged, this, &RequestParamsSettingsWidget::settingsChanged);
    QObject::connect(ui->threadsSpinBox, &QSpinBox::valueChanged, this, &RequestParamsSettingsWidget::settingsChanged);
}

RequestParamsSettingsWidget::~RequestParamsSettingsWidget() {
    delete ui;
}

QMap<QString, QVariant> RequestParamsSettingsWidget::getSettings() {
    QMap<QString, QVariant> res;
    res["request_RAM"] = this->ui->serverRAMSpinBox->value() *
            (pow(2, 10 * this->ui->serverRAMTypeComboBox->currentIndex()));
    res["request_chunksize"] = this->ui->chunkSizeSpinBox->value() *
            (pow(2, 10 * this->ui->chunkSizeTypeComboBox->currentIndex()));
    res["request_threads"] = this->ui->threadsSpinBox->value();
    return res;
}
