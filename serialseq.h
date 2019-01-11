#ifndef SERIALSEQ_H
#define SERIALSEQ_H

#include <QList>
#include <QString>
#include <QThread>
#include <QTextBrowser>

#define MAX_SERIAL_SEQ_COUNT 10

enum serialDataType
{
    SERIAL_SEQ_TYPE_ASCII = 0,
    SERIAL_SEQ_TYPE_HEX,
    SERIAL_SEQ_TYPE_BIN,
};

struct serialSequenceElem
{
    int seqId;  // sequence id
    QString data;
    QString seqName;
    int period = 0;
};

class serialSeq
{
public:
    serialSeq();
    void addSeqToList(int id, int period, const QString &seqName, const QString &seqData);
    struct serialSequenceElem* findSerialSeq(int seqId);

private:
    QList<struct serialSequenceElem> serialSeqList;

};

#endif // SERIALSEQ_H
