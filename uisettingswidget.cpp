#include "uisettingswidget.h"
#include "ui_UiSettingsWidget.h"

UiSettingsWidget::UiSettingsWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::UiSettingsWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->colorThemeComboBox, &QComboBox::currentIndexChanged, this, &UiSettingsWidget::settingsChanged);
}

UiSettingsWidget::~UiSettingsWidget() {
    delete ui;
}

void UiSettingsWidget::setSettings(SettingsStruct settings) {
    this->ui->colorThemeComboBox->setCurrentIndex(settings.ui_colortheme_index);
}

QMap<QString, QVariant> UiSettingsWidget::getSettings() {
    QMap<QString, QVariant> res;
    res["ui_colortheme_index"] = this->ui->colorThemeComboBox->currentIndex();
    return res;
}

void UiSettingsWidget::updateColorTheme(SettingsStruct settings) {
    this->setStyleSheet(settings.ui_colortheme_index == 0 ? settings.QSS->value("FloatingWhite") : settings.ui_colortheme_index == 1 ? settings.QSS->value("FloatingBlack") : "");
    this->shadowEffect.setOffset(QPoint(8, 11));
    this->shadowEffect.setBlurRadius(12);
    this->shadowEffect.setColor(settings.ui_colortheme_index == 0 ? QColor(200, 200, 200) : settings.ui_colortheme_index == 1 ? QColor(20, 20, 20) : QColor(0, 0, 0));
    this->setGraphicsEffect(&this->shadowEffect);
}
