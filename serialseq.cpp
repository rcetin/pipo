#include "serialseq.h"
#include <QDebug>
#include <QIcon>

serialSeq::serialSeq()
{

}

void serialSeq::addSeqToList(int id, int period, const QString &seqName, char *seqData, int dataLen, QPushButton *button)
{
    struct serialSequenceElem seq;
    seq.seqId = id;
    memcpy(seq.data, seqData, dataLen);
    seq.dataLen = dataLen;
    seq.seqName = seqName;
    seq.period = period;
    seq.status = 0;
    seq.sender = NULL;
    seq.button = button;
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

/**
 * @brief serialSeq::stopAllSequences
 *
 * This function stops all the started sequences.
 */
void serialSeq::stopAllSequences()
{
    qDebug() << "stopping all sequences! size: " << serialSeqList.size();
    int it = 0;
    while(it != this->serialSeqList.size())
    {
        if(serialSeqList[it].sender != NULL)
        {
            serialSeqList[it].sender->finishWork();
            serialSeqList[it].sender = NULL;
        }

        serialSeqList[it].button->setIcon(QIcon("/home/rcetin/workspace/qt_projects/pipo/img/st_seq.png"));
        serialSeqList[it].status = 0;
        it++;
    }

}
