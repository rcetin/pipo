#include "serialseq.h"
#include <QDebug>
#include <QIcon>

serialSeq::serialSeq()
{

}

void serialSeq::addSeqToList(int id, int period, int sendCount, const QString &seqName, const char *seqData, int dataLen, const QString &textDat, QPushButton *button, int type, QLabel *lName, QLabel *lData, QLabel *lPeriod)
{
    struct serialSequenceElem seq;
    seq.seqId = id;
    memcpy(seq.data, seqData, dataLen);
    seq.dataLen = dataLen;
    seq.seqName = seqName;
    seq.period = period;
    seq.sendCount = sendCount;
    seq.status = 0;
    seq.sender = NULL;
    seq.button = button;
    seq.textData = textDat;
    seq.type = type;
    seq.labelData = lData;
    seq.labelName = lName;
    seq.labelPeriod = lPeriod;
    this->serialSeqList.append(seq);
}

struct serialSequenceElem* serialSeq::findSerialSeq(int seqId)
{
    int it = 0;
    while(it != this->serialSeqList.size())
    {
        if(this->serialSeqList[it].seqId == seqId)
        {
            return &this->serialSeqList[it];
        }
        it++;
    }

    return NULL;
}

void serialSeq::editSeq(struct serialSequenceElem *elem, int period, int sendCount, const QString &seqName, const char *seqData, int dataLen, const QString &textDat)
{
    elem->period = period;
    elem->seqName = seqName;
    memcpy(elem->data, seqData, dataLen);
    elem->dataLen = dataLen;
    elem->textData = textDat;
    elem->sendCount = sendCount;
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
