#include "rfidreader.h"
#include "CFHidApi.h"

RFIDReader* g_pReader;

RFIDReader::RFIDReader(QObject *parent) : QObject(parent)
{
    g_pReader = this;

    CFHid_OpenDevice();
    CFHid_SetCallback(CallBackFunc);
    CFHid_StartRead(0xFF);
}

RFIDReader::~RFIDReader()
{
    CFHid_StopRead(0xFF);
    CFHid_SetCallback(nullptr);
    CFHid_CloseDevice();
}

void RFIDReader::parseTagId(unsigned char *pTag, unsigned char bTagLength)
{
    int iIndex = 0;
    QString strCnt, strTemp="";
    QString strTagLength;
    QString strID="";

    if(bTagLength < 2 || bTagLength > 32) {return;}

    //ID
    for(iIndex = 2; iIndex < bTagLength-1; iIndex ++)
    {
        strTemp.sprintf("%.2X",pTag[iIndex]);
        strID = strID + strTemp;
    }

    emit tagRead(strID);
}

bool RFIDReader::addTagBuffer(const unsigned char *pBuffer, const unsigned short iTagLength, const unsigned short iTagNum)
{
    if(iTagNum== 0) return false;
    int iIndex = 0;
    int iLength = 0;
    unsigned char *pID;
    unsigned char bPackLength = 0;
    if(iTagLength > 1400) return false;
    if(iTagNum >100 || iTagNum == 0) return false;

    for(iIndex = 0; iIndex < iTagNum; iIndex++)
    {
        bPackLength = pBuffer[iLength];
        pID = (unsigned char *)&pBuffer[1 + iLength];
        iLength = iLength + bPackLength + 1;
        if(iLength > 1400) break;
        parseTagId(pID, bPackLength);
    }
    return false;
}

void RFIDReader::CallBackFunc(int msg, int param1, unsigned char *param2, int param3,unsigned char *param4)
{
    Q_UNUSED(param4);

    if (msg == 2)  //Data
    {
        g_pReader->addTagBuffer((unsigned char *)param2, param3, param1);
    }
    else if(msg == 1) //Device Out
    {
    }
    else if(msg == 0) //Device Insert
    {
    }
}
