#include "tunnel.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDataStream>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include "delay.h"

Tunnel::Tunnel(QObject *parent, QString type)
    : QObject{parent}
{
    this->parent = parent;
    this->type = type;
    // Params
    this->chunksize = 2 * Mb;
    this->threads = 1; // 16
    this->RAM = 64 * Mb;
    this->path = QString(".");
    this->filename = QString("img.png");
    this->totallength = 0;
    this->files = 0;

    // DELETE LATER
    QFile file(this->filename);
    this->totallength = file.size();
    //

    //this->paths;
    //this->filenames;
    //this->totallengths;
    this->multifile = false;
    if (!this->multifile)
        this->files = 1;
    //
    this->server = "http://127.0.0.1:8000";
    //this->storage;
    //this->storagelist;

    //this->hashed;
    //this->crc32;
    //this->uploaded;
    //this->downloaded;
    //this->bytesuploaded;
    //this->bytesdownloaded;
    //this->written;

    this->status = 1;
    this->rnum = -1;
    this->dnum = -1;
    this->token = "00000000";

    this->send_info_flag = false;
    this->get_info_flag = false;

    //reg
    this->regmanager = new QNetworkAccessManager();
    this->regworker.moveToThread(&this->regthread);
    QObject::connect(this->regmanager, &QNetworkAccessManager::finished, &this->regworker, &TunnelWorker::inRegSignal);
    QObject::connect(&this->regworker, &TunnelWorker::outRegSignal, this, &Tunnel::registrationHandler);

    //sendinfo
    this->sendinfomanager = new QNetworkAccessManager();
    this->sendinfoworker.moveToThread(&this->sendinfothread);
    QObject::connect(this->sendinfomanager, &QNetworkAccessManager::finished, &this->sendinfoworker, &TunnelWorker::inSendInfoSignal);
    QObject::connect(&this->sendinfoworker, &TunnelWorker::outSendInfoSignal, this, &Tunnel::sendinfoHandler);

    //start
    this->readerworker.moveToThread(&this->readerthread);
    QObject::connect(&this->readerthread, &QThread::started, &this->readerworker, &TunnelWorker::readfiles);
    QObject::connect(&this->readerworker, &TunnelWorker::outChunkSignal, this, &Tunnel::chunkHandler);

    QObject::connect(&this->readerworker, &TunnelWorker::finished, this, [this]() {qDebug() << "finished";this->readerthread.quit();});
    QObject::connect(&this->renderSpeedTimer, &QTimer::timeout, this, [this]() {this->renderSpeed(false);});
}

void Tunnel::delay(quint64 ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Tunnel::setRnum(quint16 rnum) {
    this->rnum = rnum;
}

void Tunnel::registration() {
    this->regthread.start();
    this->regmanager->get(QNetworkRequest(QUrl(this->server + "/reg")));
}

void Tunnel::registrationHandler(qint16 rnum, qint16 code) {
    if (code == 1) {
        this->setRnum(rnum);
        this->state = 1;
    }
    emit this->registrationSignal(code);
}

void Tunnel::sendinfo() {
    QString args = this->infoToString();
    this->sendinfothread.start();
    this->sendinfomanager->get(QNetworkRequest(QUrl(this->server + "/start/" + QString::number(this->rnum) + args)));
}

void Tunnel::sendinfoHandler(qint16 code) {
    if ((int)code == 1)
        this->state = 2;
    emit this->sendinfoSignal(code);
}


QString Tunnel::infoToString() {
    QString res("");
    res += "?chunksize=" + QString::number(this->chunksize);
    res += "&threads=" + QString::number(this->threads);
    res += "&RAM=" + QString::number(this->RAM);
    res += "&multifile=" + QString::number(this->multifile);
    if (this->multifile) {

    } else {
        res += "&filename=" + this->filename;
        res += "&totallength=" + QString::number(this->totallength);
    }
    return res; // ToDo
}

void Tunnel::chunkHandler(qint64 findex, qint64 index, QByteArray chunk) {
    qDebug() << "T: ChunkHandler";
    (*this->storage)[findex][index] = chunk;
    this->storagelist->append(QList<qint64>({findex, index}));
}

void Tunnel::errorHandler(qint64 statusCode) {
    if ((int)statusCode == -1) {
        qDebug() << "Connection Error";
    } else if ((int)statusCode == -2) {
        qDebug() << "Access Denied";
    } else if ((int)statusCode == -500) {
        qDebug() << "Internal Server Error";
    }
    emit this->errorhandlersignal(statusCode);
    this->renderSpeedTimer.stop();
    this->renderSpeed(true);
}

void Tunnel::renderSpeed(bool criticalFlag) {
    QList<bool> res;
    for (quint64 i = 0; i < this->files; i++) {
        quint64 bytes = 0;
        if (type == "upload") {
            bytes = (*this->bytesuploaded)[i];
        } else if (type == "download") {
            bytes = (*this->bytesdownloaded)[i];
        }
        quint64 tl = 0;
        if (!this->multifile and (int)i == 0) {
            tl = this->totallength;
        } else {
            tl = (*this->totallengths)[i];
        }
        if (criticalFlag) {
            (*this->speedlist)[i].clear();
            (*this->lastsize)[i] = bytes;
        }
        qint8 progress = (qint8)floor((double)(bytes / (tl)) * 100);
        qDebug() << "Progress (" << QString::number(i) << "):"  << QString::number(progress);
        quint64 delta = bytes - (*this->lastsize)[i];
        (*this->lastsize)[i] = bytes;
        quint64 speed = delta / 0.25;
        (*this->speedlist)[i].append(speed);
        if ((*this->speedlist)[i].length() > 12)
            (*this->speedlist)[i].removeFirst();
        qreal rspeed = 0;
        for (int j = 0; j < (*this->speedlist)[i].length(); j++)
            rspeed += (*this->speedlist)[i].value(j);
        rspeed /= (*this->speedlist)[i].length();
        (*this->speed)[i] = (quint64)rspeed;
        qDebug() << "Speed (" << QString::number(i) << "):"  << QString::number((quint64)rspeed) << "B/s";
        if (bytes == tl)
            res.append(true);
        else
            res.append(false);
    }
    emit this->renderingdone();
    for (quint64 i = 0; i < this->files; i++) {
        if (!res[i])
            return;
    }
    this->renderSpeedTimer.stop();
    qDebug() << "finished rendering";
}

void Tunnel::start() {
    if ((int)this->state < 3) {
        if (!this->multifile) {
            this->readerworker.readparams.filename = this->filename;
            this->readerworker.readparams.path = this->path;
            this->readerworker.readparams.totallength = this->totallength;
            this->readerworker.readparams.chunksize = this->chunksize;
            this->readerworker.readparams.freememory = this->RAM;
            this->readerworker.readparams.multifile = false;
        } else {

        }
        this->readerthread.start();
        this->renderSpeedTimer.start(250);
        if (!this->multifile) {
            (*this->bytesuploaded)[0] = 0;
            (*this->bytesdownloaded)[0] = 0;
            (*this->uploaded)[0] = 0;
            (*this->downloaded)[0] = 0;
        } else {
            for (quint64 i = 0; i < this->files; i++) {
                (*this->bytesuploaded)[i] = 0;
                (*this->bytesdownloaded)[i] = 0;
                (*this->uploaded)[i] = 0;
                (*this->downloaded)[i] = 0;
                (*this->lastsize)[i] = 0;
            }
        }
        for (int i=0;i<this->threads;i++) {
            TunnelWorker *worker = new TunnelWorker(nullptr, i);
            QThread *thread = new QThread();
            worker->moveToThread(thread);
            this->Workers->insert(i, worker);
            this->Threads->insert(i, thread);
            QObject::connect(worker, &TunnelWorker::requestStatus, this, &Tunnel::getstatus);
            QObject::connect(worker, &TunnelWorker::requestChunkSignal, this, &Tunnel::getChunk);
            QObject::connect(worker, &TunnelWorker::addUploadProgressSignal, this, &Tunnel::addUploadProgress);
            QObject::connect(worker, &TunnelWorker::sendChunkSignal, this, &Tunnel::sendChunk);
            QObject::connect(worker, &TunnelWorker::errorHandlerSignal, this, &Tunnel::errorHandler);
            QObject::connect(worker, &TunnelWorker::removeChunk, this, &Tunnel::removeChunk);
            QObject::connect(thread, &QThread::started, worker, &TunnelWorker::sendStart);
            QObject::connect(worker, &TunnelWorker::finished, thread, &QThread::quit);
            QNetworkAccessManager *statusManager = new QNetworkAccessManager();
            QNetworkAccessManager *sendChunkManager = new QNetworkAccessManager();
            (*this->Managers)[i][0] = statusManager;
            (*this->Managers)[i][1] = sendChunkManager;
            QObject::connect(statusManager, &QNetworkAccessManager::finished, worker, &TunnelWorker::inStatusSignal);
            QObject::connect(sendChunkManager, &QNetworkAccessManager::finished, worker, &TunnelWorker::sendChunkReplyHandler);
            thread->start();
        }
        this->state = 3;
    } else {
        for (int i=0;i<this->threads;i++) {
            this->Workers->value(i)->sendStart();
        }
    }
}

void Tunnel::getstatus(qint64 findex, qint64 index) {
    qDebug() << "T: getstatus";
    quint64 id = ((TunnelWorker*)this->sender())->Id;
    QNetworkAccessManager *manager = (*this->Managers)[id][0];
    QString url = this->server + "/uploadawait/" + QString::number(this->rnum) + "?token=" + this->token;
    if ((int)index != -1) {
        url += "&findex=" + QString::number(findex) + "&index=" + QString::number(index);
    }
    manager->get(QNetworkRequest(QUrl(url)));
}

void Tunnel::getChunk() {
    qDebug() << "T: getChunk:" << this->sender();
    QMutexLocker(&this->getMutex);
    TunnelWorker *worker = (TunnelWorker*)this->sender();
    while (this->storagelist->empty()) {
        this->delay(100);
    }
    QList<qint64> indexes = this->storagelist->takeFirst();
    quint64 findex = indexes.takeFirst();
    quint64 index = indexes.takeFirst();
    QByteArray chunk = (*this->storage)[findex][index];
    emit worker->inChunkSignal(findex, index, chunk);
}

void Tunnel::returnChunk(qint64 findex, qint64 index) {
    qDebug() << "T: returnChunk";
    this->storagelist->append(QList<qint64>({findex, index}));
}

void Tunnel::removeChunk(qint64 findex, qint64 index) {
    qDebug() << "T: removeChunk";
    for (qint64 i = 0; i < (qint64)this->storagelist->length(); i++) {
        QList<qint64> sl = this->storagelist->value(i);
        qint64 a = sl.first();
        qint64 b = sl.last();
        if (a == findex && b == index) {
            this->storagelist->remove(i);
            break;
        }
    }
    (*this->storage)[findex].remove(index);
    this->readerworker.readparams.freememory += this->chunksize;
}

void Tunnel::sendChunk(qint64 findex, qint64 index, QByteArray chunk, QString hash) {
    qDebug() << "T: sendChunk";
    TunnelWorker *worker = (TunnelWorker*)this->sender();
    quint64 id = worker->Id;
    QNetworkAccessManager *manager = (*this->Managers)[id][1];
    QString url = this->server + "/uploadChunk/" + QString::number(this->rnum) + "?token=" + this->token;
    if ((int)index != -1) {
        url += "&findex=" + QString::number(findex) + "&index=" + QString::number(index) + "&hash=" + hash;
    }
    QNetworkRequest req = QNetworkRequest(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    QNetworkReply *reply = manager->post(req, chunk);
    QObject::connect(reply, &QNetworkReply::uploadProgress, worker, &TunnelWorker::sendChunkProgressHandler);
}

void Tunnel::addUploadProgress(quint64 bytesToAdd, qint64 findex) {
    qDebug() << "T: addUploadProgress" << QString::number(bytesToAdd);
    (*this->bytesuploaded)[findex] += bytesToAdd;
    qDebug() << (*this->bytesuploaded);
}
