#ifndef TUNNEL_H
#define TUNNEL_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QByteArray>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QEvent>
#include <QNetworkReply>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "tunnelworker.h"

struct Network {
    QNetworkAccessManager *manager;
    QNetworkRequest *request;
};

struct Uploader {
    quint32 findex;
    quint32 index;
    QString hash;
};

class Tunnel : public QObject
{
    Q_OBJECT
public:
    Tunnel(QObject *parent = nullptr, QString type="");
    QObject *parent;
    QString type;
    void delay(quint64 ms);
    QTimer renderSpeedTimer;
    qlonglong Kb = 1024;
    qlonglong Mb = 1024 * Kb;
    // Params
    qulonglong chunksize;
    qulonglong RAM;
    uint8_t threads;

    QString path;
    QString filename;
    qulonglong totallength;

    qint64 state = 0;

    QList<QString> *paths = new QList<QString>;
    QList<QString> *filenames = new QList<QString>;
    QList<quint64> *totallengths = new QList<quint64>;

    bool multifile;
    //
    QString server;

    QMap<quint64, QMap<quint64, QByteArray>> *storage = new QMap<quint64, QMap<quint64, QByteArray>>;
    QList<QList<qint64>> *storagelist = new QList<QList<qint64>>;

    QMap<quint64, quint64> *hashed = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *crc32 = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *uploaded = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *downloaded = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *bytesuploaded = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *bytesdownloaded = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *written = new QMap<quint64, quint64>;

    QMap<quint64, quint64> *speed = new QMap<quint64, quint64>;
    QMap<quint64, quint64> *lastsize = new QMap<quint64, quint64>;
    QMap<quint64, QList<quint64>> *speedlist = new QMap<quint64, QList<quint64>>;

    quint8 status;
    quint16 rnum;
    quint16 dnum;
    QString token;
    quint64 files;

    bool send_info_flag;
    bool get_info_flag;
    QEventLoop *loop;

    void setRnum(quint16 rnum);
    //
    QMap<quint16, QNetworkAccessManager> *network = new QMap<quint16, QNetworkAccessManager>;
    QMutex mutex;
    QString infoToString();
    void errorHandler(qint64 statusCode);

    //registr
    QNetworkAccessManager *regmanager;
    void registration();
    void registrationHandler(qint16 rnum, qint16 code);
    TunnelWorker regworker;
    QThread regthread;

    //sendInfo
    QNetworkAccessManager *sendinfomanager;
    void sendinfo();
    void sendinfoHandler(qint16 code);
    TunnelWorker sendinfoworker;
    QThread sendinfothread;

    //start
    void start();
    void renderSpeed(bool criticalFlag=false);
    TunnelWorker readerworker;
    QThread readerthread;
    void chunkHandler(qint64 findex, qint64 index, QByteArray chunk);
    QMap<int, TunnelWorker*> *Workers = new QMap<int, TunnelWorker*>;
    QMap<int, QThread*> *Threads = new QMap<int, QThread*>;
    QMap<int, QMap<int, QNetworkAccessManager*>> *Managers = new QMap<int, QMap<int, QNetworkAccessManager*>>;
    QMutex getMutex;
    QWaitCondition getContidion;
    void getChunk();
    void returnChunk(qint64 findex, qint64 index);
    void removeChunk(qint64 findex, qint64 index);
    void getstatus(qint64 findex, qint64 index);
    void sendChunk(qint64 findex, qint64 index, QByteArray chunk, QString hash);

    void addUploadProgress(quint64 bytesToAdd, qint64 findex);


signals:
    void registrationSignal(qint16);
    void sendinfoSignal(qint16);
    void renderingdone();
    void errorhandlersignal(qint64);

};

#endif // TUNNEL_H
