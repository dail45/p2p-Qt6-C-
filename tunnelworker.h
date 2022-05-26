#ifndef TUNNELWORKER_H
#define TUNNELWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QCryptographicHash>

struct TunnelWorkerReaderParams {
    QString path;
    QString filename;
    qulonglong totallength;

    QList<QString> *paths;
    QList<QString> *filenames;
    QList<quint64> *totallengths;

    quint64 files;

    quint64 chunksize;
    quint64 freememory;
    bool multifile;
};

class TunnelWorker : public QObject
{
    Q_OBJECT
public:
    explicit TunnelWorker(QObject *parent = nullptr, quint64 id=0);
    QMutex *mutex;
    void registration(QNetworkReply *reply);
    void sendinfo(QNetworkReply *reply);
    void readfiles();
    void addFreeMemory(quint64 memory);
    bool running;
    quint64 Id;
    QMutex readermutex;
    QWaitCondition waitreader;
    TunnelWorkerReaderParams readparams;

    void sendStart();
    void statusReplyHandler(QNetworkReply *reply);
    void sendChunkReplyHandler(QNetworkReply *reply);
    void sendChunkProgressHandler(quint64 bytesSend, quint64 bytesTotal);

    void setChunk(quint64 findex, quint64 index, QByteArray chunk);
    void errorHandler(qint64 statusCode);
    QByteArray chunk;
    QByteArray hash;
    qint64 findex = -1;
    qint64 index = -1;
    quint64 bytesLast = 0;

signals:
    void finished();
    //reg
    void inRegSignal(QNetworkReply *reply);
    void outRegSignal(qint16 rnum, qint16 code);
    //sendinfo
    void inSendInfoSignal(QNetworkReply *reply);
    void outSendInfoSignal(qint16 code);
    //start

    void requestStatus(qint64 findex=-1, qint64 index=-1);
    void inStatusSignal(QNetworkReply *reply);
    void removeChunk(quint64 findex=-1, quint64 index=-1);
    void returnChunk(quint64 findex=-1, quint64 index=-1);

    void requestChunkSignal(); // emit this signal to get chunk in inChunk();
    // void errorChunkSignal(); remove
    void inChunkSignal(quint64 findex, quint64 index, QByteArray chunk); // Tunnel >>> TunnelWorker
    void outChunkSignal(quint64 findex, quint64 index, QByteArray chunk); // TunnelWorker >>> Tunnel

    void sendChunkSignal(quint64 findex, quint64 index, QByteArray chunk, QString hash);
    void addUploadProgressSignal(quint64 bytesToAdd, quint64 findex);
    void errorHandlerSignal(qint64 statusCode);

};

#endif // TUNNELWORKER_H
