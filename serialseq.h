#ifndef SERIALSEQ_H
#define SERIALSEQ_H

#include <QList>
#include <QString>
#include <QThread>
#include <QTextBrowser>
#include "sequencesender.h"
#include <QPushButton>

#define MAX_SERIAL_SEQ_COUNT 10
#define MAX_SENDABLE_DATA_LEN 255

enum serialDataType
{
    SERIAL_SEQ_TYPE_ASCII = 0,
    SERIAL_SEQ_TYPE_HEX,
    SERIAL_SEQ_TYPE_BIN,
};

struct serialSequenceElem
{
    int seqId;  // sequence id
    char data[MAX_SENDABLE_DATA_LEN];
    int dataLen;
    QString seqName;
    int period = 0;
    int status;
    int type; // Hex, Ascii, Binary
    sequenceSender *sender;
    QPushButton *button;
};

class serialSeq
{
public:
    serialSeq();
    void addSeqToList(int id, int period, const QString &seqName, const char *seqData, int dataLen, QPushButton *button);
    struct serialSequenceElem* findSerialSeq(int seqId);
    void stopAllSequences();

private:
    QList<struct serialSequenceElem> serialSeqList;

};

#endif // SERIALSEQ_H
