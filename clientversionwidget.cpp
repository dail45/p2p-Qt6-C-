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
