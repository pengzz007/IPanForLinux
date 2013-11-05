#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include <QThread>
#include<QQueue>
#include<QMutex>
#include<QTime>
#include"public.h"
#include"IOauth.h"
#include "filedownuplist.h"
#include<QSystemTrayIcon>
#include<QFileInfo>
class UploadThread : public QThread
{
    Q_OBJECT
    public:
        explicit UploadThread(QObject *parent = 0);
        ~UploadThread();
        void Init(IOauth* oauth,QQueue<InotifyInfo>* queue ,const QString& SavePath);
        void run();
        void SetStop(bool isStop);
    private:
        IOauth* oauth;
        QQueue<InotifyInfo>* queue;
        QTime* time;
        QString SavePath;
        bool isStop;
        FileDownUpList* fileUploadList;
        int row;
        qint64 m_DataSize;
    signals:
        void HaveDataOnQueue(void);
    
    private slots:
        void UploadProgress(qint64 bytes,qint64 totalbytes);
        void UpdateFileWindow(void);
    
};

#endif // UPLOADTHREAD_H
