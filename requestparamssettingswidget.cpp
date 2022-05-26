#include "requestparamssettingswidget.h"
#include "ui_RequestParamsSettingsWidget.h"
#include "math.h"

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

void RequestParamsSettingsWidget::setSettings(SettingsStruct settings) {
    quint64 RAMInt = settings.request_RAM / pow(2, (10 * (int)(logb(settings.request_RAM) / 10)));
    quint64 RAMInd = (int)(logb(settings.request_RAM) / 10);

    quint64 chunksizeInt = settings.request_chunksize / pow(2, (10 * (int)(logb(settings.request_chunksize) / 10)));
    quint64 chunksizeInd = (int)(logb(settings.request_chunksize) / 10);

    quint64 threads = settings.request_threads;

    this->ui->serverRAMSpinBox->setValue(RAMInt);
    this->ui->serverRAMTypeComboBox->setCurrentIndex(RAMInd);

    this->ui->chunkSizeSpinBox->setValue(chunksizeInt);
    this->ui->chunkSizeTypeComboBox->setCurrentIndex(chunksizeInd);

    this->ui->threadsSpinBox->setValue(threads);
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

void RequestParamsSettingsWidget::updateColorTheme(SettingsStruct settings) {
    this->setStyleSheet(settings.ui_colortheme_index == 0 ? settings.QSS->value("FloatingWhite") : settings.ui_colortheme_index == 1 ? settings.QSS->value("FloatingBlack") : "");
    this->shadowEffect.setOffset(QPoint(8, 11));
    this->shadowEffect.setBlurRadius(12);
    this->shadowEffect.setColor(settings.ui_colortheme_index == 0 ? QColor(200, 200, 200) : settings.ui_colortheme_index == 1 ? QColor(20, 20, 20) : QColor(0, 0, 0));
    this->setGraphicsEffect(&this->shadowEffect);
}
