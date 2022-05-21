#include <QThread>
#include "tunnel.h"
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QEvent>
#include <QNetworkReply>

#ifndef TUNNELTHREAD_H
#define TUNNELTHREAD_H


class TunnelThread: public QThread
{
    Q_OBJECT
public:
    TunnelThread(Tunnel *parent);
    Tunnel *parent;
    QList<QEvent *> *queue = new QList<QEvent *>;
    QMap<quint16, QNetworkAccessManager> *network = new QMap<quint16, QNetworkAccessManager>;
    void pushEvent(QEvent *e) {
        QMutexLocker locker(&this->mutex);
        this->queue->append(e);
        this->newEvent.wakeOne();
    }

    enum EventType {
        RegistrationType = 1,
        SetUploadTokenType = 2,
        SetDownloadTokenType = 3,
        SendInfoType = 4,
        GetInfoType = 5,
        UploadWorkerType = 6,
        DownloadWorkerType = 7,
        UploaderStarterType = 8,
        DownloaderStarterType = 9
    };

    class RegistrationEvent: public QEvent {
    public:
        RegistrationEvent(qint16 state=0)
            :QEvent((QEvent::Type)RegistrationType) {
            this->state = state;
        };
        qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
    };

    class SetUploadTokenEvent: public QEvent {
    public:
        SetUploadTokenEvent(qint16 state=0)
            :QEvent((QEvent::Type)SetUploadTokenType) {
            this->state = state;
        };
        qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
    };

    class SetDownloadTokenEvent: public QEvent {
    public:
        SetDownloadTokenEvent(qint16 state=0)
            :QEvent((QEvent::Type)SetDownloadTokenType) {
            this->state = state;
        };
        qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
    };

    class SendInfoEvent: public QEvent {
        public:
            SendInfoEvent(qint16 state=0)
                :QEvent((QEvent::Type)SendInfoType) {
                this->state = state;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
        };

    class GetInfoEvent: public QEvent {
        public:
            GetInfoEvent(qint16 state=0)
                :QEvent((QEvent::Type)GetInfoType) {
                this->state = state;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
        };

    class UploaderStarterEvent: public QEvent {
        public:
            UploaderStarterEvent(qint16 state=0)
                :QEvent((QEvent::Type)UploaderStarterType) {
                this->state = state;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
        };

    class UploadWorkerEvent: public QEvent {
        public:
            UploadWorkerEvent(qint16 state=0, qint16 i=0)
                :QEvent((QEvent::Type)UploadWorkerType) {
                this->state = state;
                this->i = i;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
            qint16 i;
        };

    class DownloaderStarterEvent: public QEvent {
        public:
            DownloaderStarterEvent(qint16 state=0)
                :QEvent((QEvent::Type)DownloaderStarterType) {
                this->state = state;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
        };

    class DownloadWorkerEvent: public QEvent {
        public:
            DownloadWorkerEvent(qint16 state=0, qint16 i=0)
                :QEvent((QEvent::Type)DownloadWorkerType) {
                this->state = state;
                this->i = i;
            };
            qint16 state; // 0 - Starting // 1 - Done // -1 - Unknown Error // -2 - Access Denied // -500 - Application Error
            qint16 i;
        };

    void run();
    bool event(QEvent *e);
    bool running;
    QMutex mutex;
    QWaitCondition newEvent;
    void registration(RegistrationEvent *e);
};

#endif // TUNNELTHREAD_H
