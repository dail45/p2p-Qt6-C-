#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    //ui->setupUi(this);
    this->ui->setupUi(this);
    this->tunnel = new Tunnel(this);
}

Widget::~Widget()
{
    delete ui;
}

