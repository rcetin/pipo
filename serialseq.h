#ifndef SERIALSEQ_H
#define SERIALSEQ_H

#include <QList>
#include <QString>
#include <QThread>
#include <QTextBrowser>
#include "sequencesender.h"
#include <QPushButton>
#include <QLabel>

#define MAX_SERIAL_SEQ_COUNT 10
#define MAX_SENDABLE_DATA_LEN 255

enum serialDataType
{
    SERIAL_SEQ_TYPE_ASCII = 0,
    SERIAL_SEQ_TYPE_HEX = 1,
    SERIAL_SEQ_TYPE_BIN,
};

struct serialSequenceElem
{
    int seqId;  // sequence id
    char data[MAX_SENDABLE_DATA_LEN];
    int dataLen;
    QString textData;   // String version of data to print
    QString seqName;
    int period = 0;
    int status;
    int type; // Hex, Ascii, Binary
    int sendCount;

    sequenceSender *sender;
    QPushButton *button;
    QLabel *labelData, *labelName, *labelPeriod;
};

class serialSeq
{
public:
    serialSeq();
    void editSeq(struct serialSequenceElem *elem, int period, int sendCount, const QString &seqName, const char *seqData, int dataLen, const QString &textDat);
    void addSeqToList(int id, int period, int sendCount, const QString &seqName, const char *seqData, int dataLen, const QString &textDat, QPushButton *button, int type, QLabel *lName, QLabel *lData, QLabel *lPeriod);
    struct serialSequenceElem* findSerialSeq(int seqId);
    void stopAllSequences();

private:
    QList<struct serialSequenceElem> serialSeqList;

};

#endif // SERIALSEQ_H
