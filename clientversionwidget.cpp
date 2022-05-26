#include "clientversionwidget.h"
#include "ui_ClientVersionWidget.h"

ClientVersionWidget::ClientVersionWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::ClientVersionWidget)
{
    ui->setupUi(this);
}

ClientVersionWidget::~ClientVersionWidget() {
    delete ui;
}

void ClientVersionWidget::setVersion(QString version) {
    this->ui->CurrentClientVersionLabel->setText(version);
}

void ClientVersionWidget::updateColorTheme(SettingsStruct settings) {
    this->setStyleSheet(settings.ui_colortheme_index == 0 ? settings.QSS->value("FloatingWhite") : settings.ui_colortheme_index == 1 ? settings.QSS->value("FloatingBlack") : "");
    this->shadowEffect.setOffset(QPoint(8, 11));
    this->shadowEffect.setBlurRadius(12);
    this->shadowEffect.setColor(settings.ui_colortheme_index == 0 ? QColor(200, 200, 200) : settings.ui_colortheme_index == 1 ? QColor(20, 20, 20) : QColor(0, 0, 0));
    this->setGraphicsEffect(&this->shadowEffect);
}
