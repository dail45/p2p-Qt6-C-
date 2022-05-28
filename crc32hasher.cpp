#include "crc32hasher.h"

CRC32Hasher::CRC32Hasher()
{

}

quint32 CRC32Hasher::crc32(QByteArray data, quint32 startvalue) {
    if (data.length() == 0) {
        return startvalue;
    }
    quint32 crc = 0xFFFFFFFF;
    if (startvalue != 0) {
        crc ^= startvalue;
    }
    for (quint64 i = 0; i < (quint64)data.length(); i++) {
        crc = (crc >> 8) ^ CRC32Table[(crc ^ data[i]) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}
