/*********************************************
    IPanForLinux
    author:ywchen2000
    e-mail:ywchen2000@gmail.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************/
#include"ipangui.h"
 IPanGui::IPanGui(QWidget *parent):QWidget(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("./images/red-monster-happy-icon.png"));
    trayIcon->setToolTip(SOFTWARENAME);
    QMenu* trayIconMenu = new QMenu(this);
    QAction* quit = new QAction(QString("退出"),this);
    QAction* openFolder = new QAction(QString("打开IPan目录"),this);
    QAction* set = new QAction(QString("设置..."),this);
    QAction* helpCenter = new QAction(QString("帮助中心"),this);
    QAction* synchronous = new QAction(QString("立即同步"),this);
    QAction* pay = new QAction(QString("软件主页"),this);
    QAction* onlinemusic = new QAction(QString("音乐"),this);
         QMenu* submusic = new QMenu(this);
//         submusic->addAction(QString("百度音乐下载"));
//         submusic->addAction(QString("搜狗音乐下载"));
         QAction* kuwo = submusic->addAction(QString("酷我音乐"));
         connect(kuwo,SIGNAL(triggered()),this,SLOT(KuWo()));
         QAction* kugou = submusic->addAction(QString("酷狗音乐"));
         connect(kugou,SIGNAL(triggered()),this,SLOT(KuGou()));
         QAction* yiting = submusic->addAction(QString("一听音乐"));
         connect(yiting,SIGNAL(triggered()),this,SLOT(YiTing()));
         QAction* yinbei = submusic->addAction(QString("音贝网"));
         connect(yinbei,SIGNAL(triggered()),this,SLOT(YinBei()));
         QAction* yinyue = submusic->addAction(QString("音悦TV"));
         connect(yinyue,SIGNAL(triggered()),this,SLOT(YinYue()));
         QAction* suixinting =submusic->addAction(QString("百度随心听"));
         connect(suixinting,SIGNAL(triggered()),this,SLOT(SuiXinTing()));
         QAction* xiami = submusic->addAction(QString("虾米音乐"));
         connect(xiami,SIGNAL(triggered()),this,SLOT(XiaMi()));
         QAction* beiwa =submusic->addAction(QString("贝瓦电台"));
         connect(beiwa,SIGNAL(triggered()),this,SLOT(BeiWa()));
         QAction* douban =submusic->addAction(QString("豆瓣FM"));
         connect(douban,SIGNAL(triggered()),this,SLOT(DouBanFM()));
         QAction* qq = submusic->addAction(QString("QQ音乐"));
         connect(qq,SIGNAL(triggered()),this,SLOT(QQMusic()));
         QAction* doumi = submusic->addAction(QString("多米音乐"));
         connect(doumi,SIGNAL(triggered()),this,SLOT(DouBanFM()));
         QAction* qiuku = submusic->addAction(QString("九酷听听"));
         connect(qiuku,SIGNAL(triggered()),this,SLOT(QiuKu()));
         QAction* xiamidiantai = submusic->addAction(QString("虾米电台"));
         connect(xiamidiantai,SIGNAL(triggered()),this,SLOT(XiaMiDT()));
         QAction* kuwodt = submusic->addAction(QString("酷我电台"));
         connect(kuwodt,SIGNAL(triggered()),this,SLOT(KuGouDT()));
         QAction* kugoudt = submusic->addAction(QString("酷狗电台"));
         connect(kugoudt,SIGNAL(triggered()),this,SLOT(KuGouDT()));
         onlinemusic->setMenu(submusic);
  //  QAction* onlinepic = new QAction(QString("壁纸"),this);
    QAction* share = new QAction(QString("共享"),this);
   // QAction* webkuaipan = new QAction(QString("Web金山快盘"),this);
    connect(openFolder,SIGNAL(triggered()),this,SLOT(OpenIPanDir()));
    connect(synchronous,SIGNAL(triggered()),this,SLOT(Synchronous()));
    connect(quit,SIGNAL(triggered()),this,SLOT(Close()));
    connect(helpCenter,SIGNAL(triggered()),this,SLOT(HelpCenter()));
    connect(set,SIGNAL(triggered()),this,SLOT(setShow()));
    connect(pay,SIGNAL(triggered()),this,SLOT(Pay()));
    connect(share,SIGNAL(triggered()),this,SLOT(Share()));
 //   connect(webkuaipan,SIGNAL(triggered()),this,SLOT(WebKuaiPan()));
    trayIconMenu->addAction(openFolder);
    trayIconMenu->addAction(synchronous);
    trayIconMenu->addAction(share);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(onlinemusic);
  //  trayIconMenu->addAction(onlinepic);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(pay);
    trayIconMenu->addAction(set);
    trayIconMenu->addAction(helpCenter);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quit);
    trayIcon->setContextMenu(trayIconMenu);
    oauth= new IOauth(this,CONSUMERKEY,CONSUMERSECRET);
    connect(oauth,SIGNAL(SendDownloadProgress(qint64,qint64)),this,SLOT(CounterRate(qint64,qint64)));
    connect(oauth,SIGNAL(SendUploadProgress(qint64,qint64)),this,SLOT(UploadProgress(qint64,qint64)));
    QSettings settings(qgetenv("HOME")+SYSTEM,QSettings::IniFormat);
    QString Home = qgetenv("HOME");
    if(settings.value("SavePath").toString().isEmpty())
    {
        settings.setValue("SavePath",Home + "/IPan");
        QDir dir(Home + "/IPan");
        if(!dir.exists())
        {
            dir.mkdir(Home + "/IPan");
        }
    }
    else
    {
        QDir dir(settings.value("SavePath").toString());
        if(!dir.exists())
        {
            dir.mkdir(settings.value("SavePath").toString());
        }
    }

    SavePath = settings.value("SavePath").toString();
    inotifyThread = new InotifyThread(this);
    uploadThread = new UploadThread(this);
   // connect(inotifyThread,SIGNAL(SendNotify(QString,QString,QString)),this,SLOT(DoUserHandle(QString,QString,QString)));
    time = new QTime();
    inotifyQueue = new QQueue<InotifyInfo>();
    inotifyThread->Init(inotifyQueue);
    uploadThread->Init(oauth,inotifyQueue,SavePath);
    localFiles = new QMap<QString,QString>();
    cloudFiles = new QMap<QString,QString>();
    localDirs = new QMap<QString,QString>();
    remoteDirs= new QMap<QString,QString>();
 }

IPanGui::~IPanGui()
{
    //delete oauth;
    delete time;
    delete inotifyQueue;
    delete localFiles;
    delete cloudFiles;
    delete localDirs;
    delete remoteDirs;
}

void IPanGui::WebKuaiPan()
{
   // oauth->VistorWebKuaiPan();
}

bool IPanGui::isRunSys()
{
    QSettings settings(qgetenv("HOME")+SYSTEM,QSettings::IniFormat);
    if(settings.value("RunSys").isNull())
    {
        qDebug("isNull");
        return true;
    }
    if(settings.value("RunSys").toInt()==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void IPanGui::Synchronous()
{
    //需要提示
    if(!inotifyQueue->isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("正在上传，请稍后在同步"));
        msgBox.exec();
        return ;
    }
    inotifyThread->SetStop(true);
    uploadThread->SetStop(false);
    inotifyThread->wait(1000);
    uploadThread->wait(1000);
    inotifyThread->exit();
    uploadThread->exit();
    delete inotifyThread;
    delete uploadThread;
    getLocalAndRemoteFiles();
    InitLocalFolder();
    InitRemoteFolder();
    inotifyThread = new InotifyThread(this);
    RunInotifyThread();
    uploadThread = new UploadThread(this);
    uploadThread->Init(oauth,inotifyQueue,SavePath);
    uploadThread->start();
    trayIcon->setToolTip(SOFTWARENAME);
}

void IPanGui::Show()
{
    QFileInfo ipanInfo(qgetenv("HOME")+IPANINI);
     trayIcon->show();
     if(!ipanInfo.exists())
    {
        //显示窗口
        login = new Login(this);
        connect(login,SIGNAL(HaveAccount()),this,SLOT(LoginIPan()));
        login->show();
    }
     else
     {
         oauth->ReadStoreFile();
         oauth->ReadUserInfo();
         if(isRunSys())
         {
             getLocalAndRemoteFiles();
            InitLocalFolder();
            InitRemoteFolder();
         }
         RunInotifyThread();
         uploadThread->start();
     }
}

void IPanGui::LoginIPan()
{
    qDebug()<<"LoginIPan";
    QString Home = qgetenv("HOME");
    SavePath =Home+"/IPan";
    QDir dir;
    if(!dir.exists(SavePath))
    {
        dir.mkdir(SavePath);
    }
    oauth->RequestToken();
    oauth->RequestAuthorize();
    oauth->ReadStoreFile();
    if(isRunSys())
    {
        getLocalAndRemoteFiles();
        InitLocalFolder();
        InitRemoteFolder();
    }
    oauth->ReadUserInfo();
    RunInotifyThread();
    uploadThread->start();
}

void IPanGui::HelpCenter()
{
    openBrower = new QProcess(this);
    QStringList args;
    args<<HELPCENTER;
    openBrower->start(BROWER,args);
}

void IPanGui::setShow()
{
    Preference* pre;
    pre = new Preference(this);
    connect(pre,SIGNAL(removeToken()),this,SLOT(close()));
    connect(pre,SIGNAL(ChangeSavePath(QString)),this,SLOT(ChangeSavePath(QString)));
    oauth->ReadUserInfo();
    pre->GetUserInfo(oauth->UserInfo());
    pre->Show();
}

void IPanGui::ChangeSavePath(const QString &SavePath)
{
    this->SavePath = SavePath;
}

void IPanGui::InitLocalFolder()
{
    trayIcon->showMessage(QString("提示"),QString("系统开始同步服务器到PC端"),QSystemTrayIcon::NoIcon);
   // trayIcon->setIcon(QIcon(RINGPNG));
    mutex.lock();
    NewInitLocalFolder("/");
    mutex.unlock();
    trayIcon->showMessage(QString("提示"),QString("同步完成 "),QSystemTrayIcon::NoIcon);
    trayIcon->setToolTip(SOFTWARENAME);
   // trayIcon->setIcon(QIcon(REDMONSTER));


}

void IPanGui::RunInotifyThread()
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QStringList dirList;
    searchDir(SavePath,dirList);
    dirList.append(SavePath);
    inotifyThread->AddWatchDir(dirList);
    inotifyThread->Init(inotifyQueue);
    inotifyThread->start();
}

int IPanGui::searchDir(const QString& sComFilePath,QStringList& dirlist){

    QDir dir(sComFilePath);
    if (!dir.exists()) {
        return -1;
    }
    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);

    //文件夹优先
    dir.setSorting(QDir::DirsFirst);

    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return -1;
    }
    int i=0;
    //递归算法的核心部分
    do{
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            dirlist.append(fileInfo.absoluteFilePath());
            searchDir(fileInfo.filePath(),dirlist);
        }
        i++;
    } while(i < list.size());
    return 0;
}

void IPanGui::CounterRate(qint64 bytesSent, qint64 bytesTotal)
{
     finishSize += bytesSent;
     int rate = bytesSent/1024/(float(time->elapsed())/1000);
     QString tip =SOFTWARENAME+"\n";
     tip = tip + QString("下载速度:")+QString::number(rate)+"kb/s";
     trayIcon->setToolTip(tip);
}

void IPanGui::OpenIPanDir()
{
    QProcess* brower = new QProcess(this);
    QStringList args;
    args<<SavePath;
    brower->start(OPENIPANBROWSER,args);
}
void IPanGui::UploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesTotal == -1)
    {
        qDebug()<<"upload file bytesTotal is -1";
    }
    else
    {
        int rate = bytesSent/1024/(float(time->elapsed())/1000);
        QString tip =SOFTWARENAME+"\n";
        tip += QString("上传速度:")+QString::number(rate)+"kb/s";
        trayIcon->setToolTip(tip);
    }
}

void IPanGui::InitRemoteFolder()
{
    trayIcon->showMessage(QString("提示"),QString("系统开始同步PC端到服务器"),QSystemTrayIcon::NoIcon);
   // trayIcon->setIcon(QIcon(RINGPNG));
    mutex.lock();

    NewInitRemoteFolder("/");
    mutex.unlock();
    trayIcon->showMessage(QString("提示"),QString("所有同步完成 "),QSystemTrayIcon::NoIcon);
    trayIcon->setToolTip(SOFTWARENAME);
   // trayIcon->setIcon(QIcon(REDMONSTER));
}



void IPanGui::Close()
{
   // delete oauth;
    qDebug()<<"Close";
    inotifyThread->SetStop(true);
    uploadThread->SetStop(false);
    inotifyThread->exit();
    uploadThread->exit();
    inotifyThread->wait(1000);
   // delete inotifyThread;
    uploadThread->wait();
    qApp->processEvents(QEventLoop::DeferredDeletion);
    this->close();
}

void IPanGui::Pay()
{
    QProcess* firefox = new QProcess(this);
    QStringList args;
    args<<"http://www.ipanx.net";
    firefox->start("firefox",args);
}

void IPanGui::Share()
{
    treedialog* dialog= new treedialog();
    dialog->Init(oauth,SavePath);
    dialog->Show();
    delete dialog;
}

inline void IPanGui::OpenUrl(const QString &url)
{
    QProcess* firefox = new QProcess(this);
    QStringList args;
    args<<url;
    firefox->start(BROWSER,args);
}

void IPanGui::YinBei()
{
    QString url="http://www.yinbeiwang.com/";
    OpenUrl(url);
//    QWebView* player = new QWebView(this);
//    player->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
//    player->load(QUrl(url));
}

void IPanGui::KuWo()
{
    qDebug()<<"KuWo";
    QString url="http://player.kuwo.cn/webmusic/play?src=top";
    OpenUrl(url);
}

void IPanGui::KuGou()
{
    QString url="http://web.kugou.com/default.html";
    OpenUrl(url);
}

void IPanGui::KuGouDT()
{
    QString url="http://topic.kugou.com/radio/";
    OpenUrl(url);
}

void IPanGui::KuWoDT()
{
    QString url = "http://player.kuwo.cn/webmusic/kuwodt/diantai123.html";
    OpenUrl(url);
}

void IPanGui::YiTing()
{
    QString url="http://www.1ting.com/api/hao123/";
    OpenUrl(url);
}
void IPanGui::YinYue()
{
    QString url="http://www.yinyuetai.com/baidu/hao123";
    OpenUrl(url);
}

void IPanGui::SuiXinTing()
{
    QString url="http://fm.baidu.com/#/channel/public_xinqing_huankuai";
    OpenUrl(url);
}

void IPanGui::XiaMi()
{
   QString url="http://www.xiami.com/player/hao123/";
   OpenUrl(url);
}

void IPanGui::BeiWa()
{
    QString url = "http://app.beva.com/hao123/fm/index.html";
    OpenUrl(url);
}

void IPanGui::DouBanFM()
{
    QString url ="http://douban.fm/partner/playerhao123";
    OpenUrl(url);
}

void IPanGui::QQMusic()
{
    QString url ="http://y.qq.com/#type=singer&p=all_hot_1";
    OpenUrl(url);
}

void IPanGui::DuoMi()
{}

void IPanGui::QiuKu()
{
    QString url="http://app.jiuku.com/hao123/index.html";
    OpenUrl(url);

}

void IPanGui::XiaMiDT()
{
    QString url="http://www.xiami.com/radioi";
    OpenUrl(url);
}

int IPanGui::searchFile(const QString &sConFilePath, QMap<QString,QString> *filemap)
{
    QDir dir(sConFilePath);
    if (!dir.exists()) {
        return -1;
    }
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //文件夹优先
    dir.setSorting(QDir::DirsFirst);

    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return -1;
    }
    int i=0;
    //递归算法的核心部分
    do{
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            QString dir =fileInfo.filePath();
            dir.replace(SavePath,"");
            localDirs->insert( dir,dir);
            searchFile(fileInfo.filePath(),filemap);
        }
        else if(fileInfo.isFile())
        {
          //   filemap[fileInfo.absoluteFilePath()] = fileInfo.absoluteFilePath();
            if(!fileInfo.isHidden())
                    filemap->insert(fileInfo.absoluteFilePath(),fileInfo.absoluteFilePath());
        }
        i++;
    } while(i < list.size());
    return 0;
}

void IPanGui::NewInitLocalFolder(const QString &path)
{
    qDebug()<<"NewInitLocalFolder:"<<path;
    /*
    QMap<QString,QString> localFiles;
    //服务器那端是没有SavePath;
    QMap<QString,QString> cloudFiles;
    searchFile(SavePath+path,localFiles);
    searchRemoteFile("/",cloudFiles);
    printMap(cloudFiles);
    */
    QMapIterator<QString, QString> cloud(*cloudFiles);
    while (cloud.hasNext()) {
        cloud.next();
        QString cloudFilename = SavePath+cloud.key();
        if(localFiles->value(cloudFilename).isEmpty())
        {
             time->restart();
             oauth->DownloadFile(cloud.value(),SavePath+cloud.value());
        }
    }
}
void IPanGui::printMap(const QMap<QString, QString> *file)
{
    int k=0;
   qDebug()<<"filecount:"<<file->size();
   QMapIterator<QString, QString> i(*file);
   while (i.hasNext()) {
       i.next();
       qDebug()<<i.value();
       k++;
   }
   qDebug()<<k;
}

void IPanGui::searchRemoteFile(const QString &sConFilePath, QMap<QString, QString> *filemap)
{
    QMap<QString,QString> remoteFiles;
    QMap<QString,QString> folderinfo;
    QMap<QString,FolderStruct> foldercontext;
    QString tmpFileName;
    oauth->ReadFolderInfo(sConFilePath,1,1);
    oauth->Folders(folderinfo,foldercontext);
    int filesTotal = folderinfo["files_total"].toInt();
    qDebug()<<"filesTotal"<<filesTotal<<folderinfo;
    int page = int(filesTotal/PAGESIZE);
    if(filesTotal%PAGESIZE>0)
        page+=1;
    for(register int i = 1; i<=page; i++)
    {
        oauth->ReadFolderInfo(sConFilePath,i);
        oauth->Folders(folderinfo,foldercontext);
        qDebug()<<foldercontext.size();
        QMapIterator<QString, FolderStruct> itor(foldercontext);
        while (itor.hasNext())
        {
            itor.next();
            qDebug()<<itor.key();
            if(folderinfo["path"] == "/")
                tmpFileName = folderinfo["path"]+itor.value().name;
            else
                tmpFileName = folderinfo["path"]+"/"+itor.value().name;
            if(itor.value().type =="folder")
            {
                QDir dir(SavePath+tmpFileName);
                if(!dir.exists())
                {
                    dir.mkdir(SavePath+tmpFileName);
                }
                remoteDirs->insert(tmpFileName,tmpFileName);
                searchRemoteFile(tmpFileName,filemap);
            }
            else if(itor.value().type == "file")
            {
                  filemap->insert(tmpFileName,tmpFileName);
            }
        }
   }
}

void IPanGui::NewInitRemoteFolder(const QString &path)
{
    qDebug()<<"NewInitRemoteFolder";
    QMapIterator<QString, QString> ldirs(*localDirs);
    while(ldirs.hasNext())
    {
        ldirs.next();
        QString localdirname = ldirs.key();
        if(remoteDirs->value(localdirname).isEmpty())
        {
            oauth->CreateFolder(localdirname);
        }
    }
    QMapIterator<QString, QString> local(*localFiles);
    while (local.hasNext()) {
        local.next();
        QString  localFilename = local.key();
        QString  cloudFilename = localFilename.replace(SavePath,"");
        if(cloudFiles->value(cloudFilename).isEmpty())
        {
            QMap<QString,QString> backinfo;
            oauth->History(cloudFilename,backinfo);
            if(backinfo.value("msg") == QString("file not exist"))
            {
                    time->restart();
                    oauth->ReadUploadUrl(local.value(),cloudFilename);
           }
        }
    }
}

void IPanGui::getLocalAndRemoteFiles()
{
    localFiles->clear();
    cloudFiles->clear();
    localDirs->clear();
    remoteDirs->clear();
    searchFile(SavePath,localFiles);
    searchRemoteFile("/",cloudFiles);
    qDebug()<<"printMap";
    printMap(localFiles);
    printMap(cloudFiles);
}
