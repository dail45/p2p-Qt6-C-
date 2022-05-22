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

QMap<QString, QVariant> UiSettingsWidget::getSettings() {
    QMap<QString, QVariant> res;
    res["ui_colortheme_index"] = this->ui->colorThemeComboBox->currentIndex();
    return res;
}
