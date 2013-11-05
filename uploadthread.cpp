#include "uploadthread.h"

UploadThread::UploadThread(QObject *parent) :
    QThread(parent)
{
    isStop = true;
    fileUploadList = new FileDownUpList();
    fileUploadList->setAttribute(Qt::WA_QuitOnClose, false);
    connect(this,SIGNAL(HaveDataOnQueue()),this,SLOT(UpdateFileWindow()));
}

UploadThread::~UploadThread()
{
    qDebug()<<"~UploadThread";
    delete this->oauth;
    delete fileUploadList;
    delete time;
}

void UploadThread::Init(IOauth *oauth, QQueue<InotifyInfo>* queue ,const QString& SavePath)
{
    //this->oauth = oauth;
    Q_UNUSED(oauth);
    this->oauth =NULL;
    this->queue = queue;
    this->SavePath = SavePath;
    time = new QTime();
}

void UploadThread::run()
{
//    qDebug()<<"DoUserHandle:"<<filename;
//    qDebug()<<"method:"<<method;
//    QString tmpfile = filename;
//    tmpfile.replace(SavePath,QString(""));
    QMutex mutex;
    row =0;
    this->oauth = new IOauth(NULL,CONSUMERKEY,CONSUMERSECRET);
    this->oauth->ReadStoreFile();
    //fileUploadList = new FileDownUpList(NULL);
    connect(this->oauth,SIGNAL(SendUploadProgress(qint64,qint64)),this,SLOT(UploadProgress(qint64,qint64)));
    while(isStop)
    {
        mutex.lock();
        bool r = queue->isEmpty();
        mutex.unlock();
        if(r)
        {
         //   row =0;
        }
        if(!r)
        {
            emit HaveDataOnQueue();
            mutex.lock();
            InotifyInfo inotifynode = queue->dequeue();
            mutex.unlock();
            QString method = inotifynode.method;
            QString type = inotifynode.type;
            QString filename = inotifynode.filename;
            QString tmpfile = filename;
            tmpfile.replace(SavePath,QString(""));
            if((QString::compare(method ,CREATE) == 0) | (QString::compare(method,MOVETO)==0))
            {
               // row++;
                if(QString::compare(type,FOLDER) ==0)
                {
                    //创建目录
                    mutex.lock();
                    oauth->CreateFolder(tmpfile);
                    mutex.unlock();
                }
                else if(QString::compare(type ,WFILE ) == 0)
                {
                    qDebug()<<"uploads file";
                    time->restart();
                    mutex.lock();
                    time->restart();
                    oauth->ReadUploadUrl(filename,tmpfile);
                    mutex.unlock();
                  //  trayIcon->showMessage(QString::null,QFileInfo(filename).fileName()+tr(" 上传完毕"),QSystemTrayIcon::NoIcon,2000);
                   // trayIcon->setToolTip(SOFTWARENAME);
                }
            }
            else if((QString::compare(method,DELETE)==0) | (QString::compare(method ,MOVEFROM) ==0))
            {
              //  row++;
                if((QString::compare(type ,FOLDER) == 0) | (QString::compare(type,WFILE) ==0))
                {
                    qDebug()<<"delete file";
                    mutex.lock();
                    oauth->DeleteFileFolder(tmpfile);
                    mutex.unlock();
                }
            }
            else if(QString::compare(method,WRITE) ==0)
            {       //目前不支持写操作
        //        if(QString::compare(type,WFILE) == 0)
        //        {

                   // qDebug()<<"close wirte uploads file"<<fileName<<type;
        //            time->restart();
        //            oauth->ReadUploadUrl(filename,tmpfile);
        //            trayIcon->showMessage(QString::null,QFileInfo(filename).fileName()+tr(" 上传完毕"),QSystemTrayIcon::NoIcon,2000);
        //            trayIcon->setToolTip(SOFTWARENAME);
        //        }
            }
        }
        msleep(1000);
    }
}

void UploadThread::SetStop(bool isStop)
{
    this->isStop = isStop;
}

void UploadThread::UploadProgress(qint64 bytes, qint64 totalbytes)
{
    if(fileUploadList->isHidden())
    {
        fileUploadList->show();
        fileUploadList->exec();
    }
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    int rate = bytes/1024/(float(time->elapsed())/1000);
    QString ratestr= QString("速度:")+QString::number(rate)+"kb/s";
    int percent = int((bytes/float(totalbytes))*100);
    fileUploadList->Insert(0,1,ratestr);
    fileUploadList->Insert(0,2,QString::number(percent));
}

void UploadThread::UpdateFileWindow()
{
    QMutex mutex;
    fileUploadList->Clear();
    QString type;
    QString method;
    mutex.lock();
    int size = queue->size();
    mutex.unlock();
    for(int i=0;i<size;i++)
    {
        mutex.lock();
        type = queue->at(i).type;
        method = queue->at(i).method;
        mutex.unlock();
        if((QString::compare(method ,CREATE)==0&&QString::compare(type,WFILE)==0) ||
           (QString::compare(method,CREATE)==0&&QString::compare(type,FOLDER)==0) ||
           (QString::compare( method,DELETE)==0&&QString::compare(type ,WFILE )==0) ||
           (QString::compare(method ,DELETE)==0&&QString::compare(type,FOLDER)==0)||
           (QString::compare(method ,MOVEFROM)==0&&QString::compare(type,FOLDER)==0)||
           (QString::compare(method ,MOVEFROM)==0&&QString::compare(type,WFILE)==0)||
           (QString::compare(method ,MOVETO)==0&&QString::compare(type,FOLDER)==0)||
           (QString::compare(method ,MOVETO)==0&&QString::compare(type,WFILE)==0)
              )
        {
            qDebug()<<"inset data;";
            mutex.lock();
            QFileInfo fileinfo(queue->at(i).filename);
            mutex.unlock();
            fileUploadList->Insert(fileinfo.fileName());
        }
    }
    if(fileUploadList->isHidden())
    {
        fileUploadList->show();
        fileUploadList->exec();
    }
}
