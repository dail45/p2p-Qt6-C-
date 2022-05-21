#include "tunnelworker.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

TunnelWorker::TunnelWorker(QObject *parent, quint64 id)
    : QObject{parent}
{
    this->Id = id;
    QObject::connect(this, &TunnelWorker::inRegSignal, this, &TunnelWorker::registration);
    QObject::connect(this, &TunnelWorker::inSendInfoSignal, this, &TunnelWorker::sendinfo);

    QObject::connect(this, &TunnelWorker::inStatusSignal, this, &TunnelWorker::statusReplyHandler);
    QObject::connect(this, &TunnelWorker::inChunkSignal, this, &TunnelWorker::setChunk);
    QObject::connect(this, &TunnelWorker::errorHandlerSignal, this, &TunnelWorker::errorHandler);

}

void TunnelWorker::registration(QNetworkReply *reply) {
    if (reply->error()) {
        emit this->outRegSignal(-1, -1);
        return;
    }
    QString res = reply->readAll();
    if (res.length() > 4) {
        emit this->outRegSignal(-1, -500);
        return;
    }
    emit this->outRegSignal(res.toInt(), 1);
    return;
}

void TunnelWorker::sendinfo(QNetworkReply *reply) {
    if (reply->error()) {
        emit this->outSendInfoSignal(-1);
        return;
    }
    QByteArray res = reply->readAll();
    try {
        QJsonDocument doc = QJsonDocument::fromJson(res);
        QJsonObject object = doc.object();
        if (object.keys().indexOf("status") != -1 and object.value("status") == "Access denied") {
            emit this->outSendInfoSignal(-2);
        }
    } catch (...) {
        emit this->outSendInfoSignal(-500);
    }
    emit this->outSendInfoSignal(1);
    return;
}

void TunnelWorker::addFreeMemory(quint64 memory) {
    this->readparams.freememory += memory;
}

void TunnelWorker::errorHandler(qint64 statusCode) {
    qDebug() << "TW: clean(errorHandler)";
    if (this->findex != -1 && this->index != -1)
        emit this->returnChunk(this->findex, this->index);
    this->findex = -1;
    this->index = -1;
    this->bytesLast = 0;
    this->chunk = NULL;
    this->hash = NULL;
    this->running = false;
}

void TunnelWorker::readfiles() {
    this->running = true;
    if (!this->readparams.multifile){
    QFile file(this->readparams.path + "/" + this->readparams.filename);
    file.open(QFile::ReadOnly);
    quint64 findex = 0;
    quint64 index = 0;
    while (this->running) {
        this->readermutex.lock();
        if (this->readparams.chunksize > this->readparams.freememory)
            this->waitreader.wait(&this->readermutex);
        QByteArray chunk = file.read(this->readparams.chunksize);
        if (chunk.length() == 0) {
            this->running = false;
            this->readermutex.unlock();
            break;
        }
        this->readparams.freememory -= this->readparams.chunksize;
        emit this->outChunkSignal(findex, index, chunk);
        ++index;
        this->readermutex.unlock();
    }} else {
    while (this->running) {

    }
    }
    emit this->finished();
}

void TunnelWorker::setChunk(quint64 findex, quint64 index, QByteArray chunk) {
    qDebug() << "TW: setChunk";
    this->findex = findex;
    this->index = index;
    this->chunk = chunk;
    this->hash = QCryptographicHash::hash(this->chunk, QCryptographicHash::Algorithm::Sha1);
    emit this->sendChunkSignal(this->findex, this->index, this->chunk, this->hash.toHex());
}

void TunnelWorker::sendStart() {
    qDebug() << "TW: sendStart";
    this->running = true;
    emit this->requestStatus(-1, -1);
}

void TunnelWorker::statusReplyHandler(QNetworkReply *reply) {
    qDebug() << "TW: statusReplyHandler";
    if (reply->error()) {
        emit this->errorHandlerSignal(-1);
        return;
    }
    QByteArray res = reply->readAll();
    try {
        QJsonDocument doc = QJsonDocument::fromJson(res);
        QJsonObject object = doc.object();
        if (object.keys().indexOf("status") != -1 && object.value("status") == "Access denied") {
            emit this->errorHandlerSignal(-2);
        }
        QJsonValue status = object.value("status");
        qDebug() << "TW: SRH:" << status;
        if (status == "dead" || status == "dead-timeout") {
            emit this->finished();
            return;
        } else if (status == "alive-timeout" || status == "ram-error") {
            emit this->requestStatus();
            return;
        } else if (status == "alive") {
            emit this->requestChunkSignal();
            return;
        } else if (status == "again") {
            emit this->sendChunkSignal(this->findex, this->index, this->chunk, this->hash.toHex());
            return;
        }


    } catch (...) {
        emit this->errorHandlerSignal(-500); // ToDo
    }
}

void TunnelWorker::sendChunkReplyHandler(QNetworkReply *reply) {
    qDebug() << "TW: sendChunkReplyHandler:" << "chunk uploaded";
    if (reply->error()) {
        emit this->errorHandlerSignal(-1); // ToDo
        return;
    }
    if ((int)this->findex != -1 && (int)this->index != -1)
        emit this->removeChunk(this->findex, this->index);
    emit this->requestStatus(this->findex, this->index);
}

void TunnelWorker::sendChunkProgressHandler(quint64 bytesSend, quint64 bytesTotal) {
    qDebug() << "TW: sendChunkProgressHandler";
    quint64 delta = bytesSend - this->bytesLast;
    this->bytesLast = bytesSend;
    emit addUploadProgressSignal(delta, this->findex);
    if (bytesSend == bytesTotal) {
        this->bytesLast = 0;
    }
}
