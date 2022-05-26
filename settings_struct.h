#include <QObject>
#include <QMap>

#ifndef SETTINGS_STRUCT_H
#define SETTINGS_STRUCT_H
struct SettingsStruct {
    QString server;
    quint8 server_index;
    quint16 localhost_server_port = 8001;

    QString another_server_ip;
    quint16 another_server_port = 8001;

    quint64 request_RAM = 64 * 1024 * 1024;
    quint64 request_chunksize = 2 * 1024 * 1024;
    quint16 request_threads = 16;

    quint16 ui_colortheme_index;

    QMap<QString, QString> *QSS = new QMap<QString, QString>;
};
#endif // SETTINGS_STRUCT_H
