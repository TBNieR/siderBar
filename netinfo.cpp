#include "netinfo.h"
#include <winsock.h>
#include <iphlpapi.h>
#include <QThread>
#include <QDebug>
NetInfo::NetInfo(QObject *parent)
    : QObject{parent}
{
    isExit = false;
}

void NetInfo::doWork()
{
    PMIB_IFTABLE pTable = nullptr;
        DWORD dword = 0;
        ULONG retCode = GetIfTable(pTable, &dword, true);
        if(retCode == ERROR_NOT_SUPPORTED)
            return;
        if(retCode == ERROR_INSUFFICIENT_BUFFER)
            pTable = (PMIB_IFTABLE)new BYTE[65535];
        //上传速度
        DWORD dwIn = 0;
        //下载速度
        DWORD dwOut = 0;
        //上传最后字节
        DWORD dwLastIn = 0;
        //下载最后字节
        DWORD dwLastOut = 0;

        while(!isExit)
        {
            GetIfTable(pTable, &dword, true);
            DWORD dwInOc = 0;
            DWORD dwOutOc = 0;
            for(UINT i = 0; i < pTable->dwNumEntries; i++)
            {
                MIB_IFROW row = pTable->table[i];
                dwInOc += row.dwInOctets;
                dwOutOc += row.dwOutOctets;
            }
            dwIn = dwInOc - dwLastIn;
            dwOut = dwOutOc - dwLastOut;

            if(dwLastIn <= 0)
                dwIn = 0;
            else
                dwIn = dwIn / 1024;

            if(dwLastOut <= 0)
                dwOut = 0;
            else
                dwOut = dwOut / 1024;

            dwLastIn = dwInOc;
            dwLastOut = dwOutOc;

            emit sendNewNetSpeed(QString("%1kb/s").arg(dwIn));
            QThread::msleep(1000);
        }

        delete [] pTable;
        qDebug()<<"exit thread";
}

void NetInfo::exitThread()
{
    isExit=true;
}
