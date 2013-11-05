/*********************************************
    金山快盘 for QT SDK
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
#ifndef IOAUTH_H
#define IOAUTH_H
/*
 *编码规则:槽都要以slot开头
 *常量都要大写
 *变量开头小写后面单词都大写
 *一个单词命名的变量小写
 */
#include<QtDebug>
#include<QtNetwork/QNetworkAccessManager>
#include<QtNetwork/QNetworkReply>
#include<QtNetwork/QNetworkCookie>
#include<QObject>
#include<QDateTime>
#include<QCryptographicHash>
#include<QByteArray>
#include<QMap>
#include<QMapIterator>
#include<QUrl>
#include<QRegExp>
#include<QFile>
#include<QtScript/QScriptEngine>
#include<QtScript/QScriptValueIterator>
#include<QtScript/QScriptValue>
#include<QEventLoop>
#include<QProcess>
#include<QStringList>
#include<QSettings>
#include<QFileInfo>
#include<QtGlobal>
#include<QTextCodec>
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QMessageBox>
#include<QApplication>
#include"public.h"
#include<QVariant>
#include<QtWebKit/QWebView>
#include<QDataStream>

const QString RequestTokenUrl = "https://openapi.kuaipan.cn/open/requestToken";
const QString AUTHORISE ="https://www.kuaipan.cn/api.php?ac=open&op=authorise&oauth_token=";
const QString ACCESSTOKEN = "https://openapi.kuaipan.cn/open/accessToken";
const QString BROWSE ="firefox";
const QString V =  "1";
const QString ACCOUNTINFO = "http://openapi.kuaipan.cn/"+V+"/account_info";
const QString METADATA = "http://openapi.kuaipan.cn/"+V+"/metadata/kuaipan";
const QString CREATEFOLDER ="http://openapi.kuaipan.cn/"+V+"/fileops/create_folder";
const QString DELETEFOLDER ="http://openapi.kuaipan.cn/"+V+"/fileops/delete";
const QString MOVEFILE ="http://openapi.kuaipan.cn/"+V+"/fileops/move";
const QString COPYFILE = "http://openapi.kuaipan.cn/"+V+"/fileops/copy";
const QString UPLOADFILE = "http://api-content.dfs.kuaipan.cn/"+V+"/fileops/upload_locate";
const QString UPLOADLOCATE ="http://api-content.dfs.kuaipan.cn/"+V+"/fileops/upload_locate";
const QString DOWNLOADFILE="http://api-content.dfs.kuaipan.cn/"+V+"/fileops/download_file";
const QString THUMBNAIL = "http://conv.kuaipan.cn/"+V+"/fileops/thumbnail";
const QString WEBKUAIPAN = "http://www.kuaipan.cn/home.htm?m=kp";
const QString SHAREURL ="http://openapi.kuaipan.cn/"+V+"/shares/";
const QString HISTORY = "http://openapi.kuaipan.cn/"+V+"/history/";
const QString width =QString::number(50);
const QString height = QString::number(50);
const qint64 SIZE =  1048576;
const qint64 BUF_LEN = 102400;
const QString ROOT = "kuaipan";
class IOauth :public QObject
{
    Q_OBJECT
    public:
        IOauth(QObject* parent);
        IOauth(QObject* parent,const QString& consumer_key,const QString& consumer_secret );
        virtual ~IOauth(void);
        void RequestToken(void);
        void RequestAuthorize(void);
        void RequestAccessToken(void);
        void ReadStoreFile(void);
        void ReadUserInfo(void);
       // void ReadFolderInfo(const QString& path);
        void ReadFolderInfo(const QString &path,int page=1,int pageSize=PAGESIZE);
        void CreateFolder(const QString& folderName);
        void DeleteFileFolder(const QString& folderName);
        void MoveFile(const QString& from,const QString& to);
        void CopyFile(const QString& from,const QString& to);
        void ReadUploadUrl(const QString& filename,const QString& to);
        void UploadFile(const QString& filename,const QString& to);
        void DownloadFile(const QString& filename,const QString& localName);
        void History(const QString& filename,QMap<QString,QString>& backinfo);
        void Thumbnail(const QString& filename);
        QMap<QString,QString> UserInfo();
        void Folders(QMap<QString,QString>&cfolderInfo,QMap<QString,FolderStruct>& cfoldercontent );
        void VistorWebKuaiPan();
        void ShareFile(const QString& filename,QString& url);
    signals:
        void SendDownloadSize(qint64 size);
        void FinishLogin();
        void SendUploadProgress ( qint64 bytesSent, qint64 bytesTotal );
        void SendDownloadProgress ( qint64 bytesReceived, qint64 bytesTotal );
     private:
        void setUploadFile(const QString& file,const QString& to);
        QString HmacSha1(QByteArray key, QByteArray baseString);
        QString GetOauthSignature(const QString& method,const QString& base_uri, const QMap<QString, QString> & paramesters);
        void GetConstData(QMap<QString,QString>& map);
        QString CreatePara(const QMap<QString,QString>& map);
        QUrl CreateUrL(const QString& baseUrl,QMap<QString,QString>& constData);
        QString EncodeUrl(QString url);
        QString getRandNonce(void);
        QByteArray CreatePostData(const QByteArray& fileData,QString& contentType,const QString& fileName);
        void ParseJson(const QString& jsonString);

        QNetworkAccessManager* networkManager;
        QNetworkReply* reply;
        QNetworkReply* getFolderInfoReply;
        QNetworkReply* fileDownloadReply;
        QNetworkReply* readData;
        QNetworkReply* getUserInfoReply;
        QNetworkReply* getUploadUrlReply;
        QNetworkReply* postData;
        QNetworkReply* deleteFileReply;
        QNetworkReply* createFolderReply;
        QNetworkReply* moveFileReply;
        QNetworkReply* copyFileReply;
        QNetworkReply* thumbnailReply;
        QNetworkReply* webKuaiPanReply;
        QNetworkReply* shareFileReply;
        QNetworkReply* historyReply;
        QString consumer_key;
        QString consumer_secret;
        QNetworkRequest request;
        QString verifier;
        QString downloadFile;
        QString folderString;
        int filelen;
        int loadWebCount;
        QString uploadLocalFile;
        QString uploadRemoteFile;
        QEventLoop* folderLoop;
        QMap<QString,QString> tempTokenMap;
        QMap<QString,QString> accessTokenMap;
        QMap<QString,QString> userInfo;
        QMap<QString,QString> folderInfo;
        QMap<QString,FolderStruct> foldercontent;
        QMap<QString,QString> uploadUrl;
        QMap<QString,QString> uploadFile;
        QMap<QString,QString> copyFile;
        QMap<QString,QString> moveFile;
        QMap<QString,QString> createFile;
        QMap<QString,QString> shareFile;
        QMap<QString,QString> historyFile;

    private slots:
      //  void slotSslErrors(QList<QSslError> error);
        void slotReadOauthToken();
        void slotReadAccessToken();
        void slotFinishVerifier(int code);
        void slotReadUserInfo();
        void slotReadFolderInfo();
        void slotCreateFolder();
        void slotDeleteFolder();
        void slotMoveFile();
        void slotCopyFile();
        void slotReadUploadUrl(void);
        void slotUploadFile(void);
        void slotDownloadFile(QNetworkReply* downloadReply);
        bool slotDownloadFile();
        void slotWebKuaiPan();
        void slotShareFile();
        void slotHistory();
        void slotError(QNetworkReply::NetworkError error);
 };
#endif // IOAUTH_H
