#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QWidget *> *UploadWidgetsList = new QList<QWidget *>;
    QMap<QString, QString> *StyleSheet = new QMap<QString, QString>;
    void createUploadWidget();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
