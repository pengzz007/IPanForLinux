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
#ifndef IPANGUI_H
#define IPANGUI_H
/*
 *编码规则:槽都要以slot开头
 *常量都要大写
 *变量开头小写后面单词都大写
 *一个单词命名的变量小写
 *函数单词第一个字母都要大写
 */
#include<QWidget>
#include<QSystemTrayIcon>
#include<QMenu>
#include<QTextCodec>
#include<QProcess>
#include<QStringList>
#include<QFile>
#include<QFileInfo>
#include"public.h"
#include"login.h"
#include"preferences.h"
#include"IOauth.h"
#include<QDir>
#include<QFileInfoList>
#include"inotifythread.h"
#include"uploadthread.h"
#include<QTime>
#include <QQueue>
#include <QMutex>
#include<QMap>
#include<QFileSystemModel>
#include"treedialog.h"

#include <QtWebKit/QWebView>

const QString HELPCENTER = "http://help.kuaipan.cn/";
const QString BROWER = "firefox";
const QString OPENIPANBROWSER="nautilus";
const QString RINGPNG ="./images/yellow-monster-happy.png";
const QString REDMONSTER = "./images/red-monster-happy-icon.png";
class IPanGui: public QWidget
{
    Q_OBJECT
    public:
         IPanGui(QWidget* parent = NULL);
         ~IPanGui();
         void Show();
     private:
         /*
         void InitLocalFolder(const QString& path);
         void InitRemoteFolder(const QString& path);
         */
         void NewInitLocalFolder(const QString& path);
         void NewInitRemoteFolder(const QString& path);
         void RunInotifyThread(void);
         int searchDir(const QString& sComFilePath,QStringList& dirlist);
         int searchFile(const QString& sConFilePath,QMap<QString,QString>* filemap);
         void searchRemoteFile(const  QString& sConFilePath,QMap<QString,QString>* filemap);
         void OpenUrl(const QString& url);
         bool isRunSys(void);
         void printMap(const QMap<QString,QString>* file);
         void getLocalAndRemoteFiles();
        QSystemTrayIcon *trayIcon;
        QProcess *openBrower;
        IOauth* oauth;
        QString SavePath;
        InotifyThread* inotifyThread;
        uint32_t fileSize;
        qint64 finishSize;
        QTime* time;
        Login* login;
        QNetworkReply* reply;
        QNetworkAccessManager* networkManager;
        QQueue<InotifyInfo>* inotifyQueue;
        UploadThread* uploadThread;
        QMutex mutex;
        QMap<QString,QString>* localFiles;
        //服务器那端是没有SavePath;
        QMap<QString,QString>* cloudFiles;
        QMap<QString,QString>* localDirs;
        QMap<QString,QString>* remoteDirs;

    private slots:
        void InitLocalFolder(void);
        void InitRemoteFolder(void);
        void HelpCenter();
        void LoginIPan();
        void setShow();
      //  void  DoUserHandle(const QString& filename,const QString& method,const QString& type);
        void  CounterRate(qint64 bytesSent,qint64 bytesTotal);
        void OpenIPanDir();
        void ChangeSavePath(const QString& SavePath);
        void Close();
        void Synchronous();
        void WebKuaiPan();
        void UploadProgress(qint64 bytesSent, qint64 bytesTotal);
        void Pay();
        void Share();
        void YinBei();
        void KuWo();
        void KuGou();
        void YiTing();
        void YinYue();
        void SuiXinTing();
        void XiaMi();
        void BeiWa();
        void DouBanFM();
        void QQMusic();
        void DuoMi();
        void QiuKu();
        void XiaMiDT();
        void KuWoDT();
        void KuGouDT();
};
#endif // IPANGUI_H
