#include "serialseq.h"
#include <QDebug>

//QList<struct serialSequenceElem*> serialSeqList;

serialSeq::serialSeq()
{

}

void serialSeq::addSeqToList(int id, const QString &seqName, const QString &seqData)
{
    struct serialSequenceElem seq;
    seq.seqId = id;
    seq.data = seqData;
    seq.seqName = seqName;

    this->serialSeqList.append(&seq);
    qDebug() << "Num of items11: " <<this-> serialSeqList.size();
}

void serialSeq::printSeqList()
{

}
