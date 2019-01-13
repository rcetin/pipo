#include "serialseq.h"
#include <QDebug>

//QList<struct serialSequenceElem*> serialSeqList;

serialSeq::serialSeq()
{

}

void serialSeq::addSeqToList(int id, int period, const QString &seqName, const QString &seqData)
{
    struct serialSequenceElem seq;
    seq.seqId = id;
    seq.data = seqData;
    seq.seqName = seqName;
    seq.period = period;
    seq.status = 0;
    seq.sender = NULL;
    this->serialSeqList.append(seq);
}

struct serialSequenceElem* serialSeq::findSerialSeq(int seqId)
{
    int it = 0;
    qDebug() << "Size of list: " << this->serialSeqList.size();
    while(it != this->serialSeqList.size())
    {
        if(this->serialSeqList[it].seqId == seqId)
        {
            qDebug() << "sequence is found. Index: " << it;
            return &this->serialSeqList[it];
        }
        it++;
    }

    return NULL;
}
