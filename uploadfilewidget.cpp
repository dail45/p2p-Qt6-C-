#include "uploadfilewidget.h"
#include "ui_UploadFileWidget.h"

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget{parent}, ui(new Ui::UploadFileWidget)
{
    ui->setupUi(this);
    ui->sizeLabel->hide();
    ui->pathLabel->hide();
    ui->progressBar->hide();
}

UploadFileWidget::~UploadFileWidget()
{
    delete ui;
}
