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
#include"IOauth.h"
QString getTimeStamp()
{
    time_t curTime=time(0);
    return QString().number(curTime);
}
IOauth::IOauth(QObject *parent):QObject(parent)
{}

IOauth::~IOauth()
{
   // delete networkManager;
}

IOauth::IOauth(QObject *parent, const QString &consumer_key, const QString &consumer_secret):QObject(parent)
{
    this->consumer_key = consumer_key;
    this->consumer_secret = consumer_secret;
    folderString="";
    networkManager = new QNetworkAccessManager(this);
    reply = NULL;
}

QString IOauth::getRandNonce(void)
{
    QTime time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return QString().number(qrand());
}


void IOauth::GetConstData(QMap<QString,QString>& map)
{
    map["oauth_consumer_key"]= consumer_key;
    map["oauth_nonce"] = getRandNonce();
    map["oauth_signature_method"]= "HMAC-SHA1";
    map["oauth_timestamp"]=getTimeStamp();
    map["oauth_version"] ="1.0";
}

QString IOauth::CreatePara(const QMap<QString, QString>& map)
{
    QString Uri;
    Uri.clear();
    QMapIterator<QString, QString> i(map);
    while (i.hasNext()) {
        i.next();
        if(!i.hasNext())
            Uri = Uri+i.key()+"="+i.value();
        else
            Uri = Uri+i.key()+"="+i.value()+"&";
    }
    return Uri;
}

void IOauth::RequestToken(void)
{
    QMap<QString,QString> map;
    GetConstData(map);
    QString BaseString=GetOauthSignature("GET&",RequestTokenUrl,map);
    QString ConsumerSecret = consumer_secret+"&";
    QString oauth_signature = HmacSha1(ConsumerSecret.toAscii(),BaseString.toAscii());
   // qDebug()<<"oauth_signature:"<<oauth_signature<<endl;
    QString Uri =RequestTokenUrl+"?";
    QString para = CreatePara(map);
    para.append("&oauth_signature="+QUrl::toPercentEncoding(oauth_signature));
    Uri.append(para);
    QUrl outputUrl ;
    outputUrl.setEncodedUrl(Uri.toAscii());
   // qDebug()<<Uri.toAscii();
  //  return;
    request.setUrl(outputUrl);
    reply = networkManager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadOauthToken()));
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(slotReadOauthToken()));
    disconnect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
//    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
//            this, SLOT(slotError(QNetworkReply::NetworkError)));

}

void IOauth::slotError(QNetworkReply::NetworkError error)
{
    qDebug()<<"slotError"<<error;
}
void ReadDataToMap(const QString& jsonString,QMap<QString,QString>& map)
{
    QScriptEngine engine;
    QScriptValue json = engine.evaluate("("+ jsonString+")");
    QScriptValueIterator it(json);
    map.clear();
    while (it.hasNext()) {
        it.next();
        map[it.name()]=it.value().toString();
      //  qDebug()<<it.name()<<":"<<it.value().toString();
    }
}

void IOauth::slotReadOauthToken()
{
    qDebug()<<"slotReadOauthToken";
    QString jsonString;
    jsonString  = reply->readAll();
    int result = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(result == 200)
    {
        ReadDataToMap(jsonString,tempTokenMap);
    }
    else
    {
      //  qDebug()<<"slotReadOauthToken:"<<result;
    }

}

void IOauth::RequestAuthorize(void)
{
  //  qDebug()<<"isTrue"<<tempTokenMap.empty();
    qDebug()<<"RequestAuthorize";
    if(tempTokenMap.empty())
        return;
    QString uri = AUTHORISE+tempTokenMap["oauth_token"];
    loadWebCount =0;
  //  QWebView *view;
    QEventLoop loop;
//    view = new QWebView();
//    view->load(QUrl(uri));
//    connect(view,SIGNAL(loadFinished(bool)),this,SLOT(slotFinishVerifier(bool)));
//    connect(this,SIGNAL(FinishLogin()),&loop,SLOT(quit()));
//    view->show();
    QProcess* process = new QProcess();
    connect(process,SIGNAL(finished(int)),this,SLOT(slotFinishVerifier(int)));
    connect(this,SIGNAL(FinishLogin()),&loop,SLOT(quit()));
    QStringList args;
    args<<uri;
    process->start(BROWSE,args);
    loop.exec();
    delete process;
//    delete view;
}

void IOauth::slotFinishVerifier(int code)
{
    Q_UNUSED(code)
    RequestAccessToken();
//    loadWebCount =loadWebCount + 1;
//    if(loadWebCount == 2)
//    {
//        RequestAccessToken();
//        loadWebCount =0;
//    }
}

void IOauth::RequestAccessToken()
{
    qDebug()<<"RequestAccessToken";
    QMap<QString,QString> constData;
    QString baseUrl = ACCESSTOKEN;
    GetConstData(constData);
    constData["oauth_token"] = tempTokenMap["oauth_token"];
    QString BaseString=GetOauthSignature("GET&",ACCESSTOKEN,constData);
    QString Secret = consumer_secret+"&"+tempTokenMap["oauth_token_secret"];
    QString oauth_signature = HmacSha1(Secret.toAscii(),BaseString.toAscii());
   // qDebug()<<"oauth_signature:"<<oauth_signature<<endl;
    QString Uri =ACCESSTOKEN+"?";
    QString para = CreatePara(constData);
    para.append("&oauth_signature="+QUrl::toPercentEncoding(oauth_signature));
    Uri.append(para);
    QUrl outputUrl ;
    outputUrl.setEncodedUrl(Uri.toAscii());
    request.setUrl(outputUrl);
    reply = networkManager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadAccessToken()));
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    disconnect(reply, SIGNAL(readyRead()), this, SLOT(slotReadAccessToken()));
    disconnect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
}

void IOauth::slotReadAccessToken()
{
    qDebug()<<"slotReadAccessToken";
    QString data =reply->readAll();
    int result = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(result == 200)
    {
        ReadDataToMap(data,accessTokenMap);
        QSettings settings(qgetenv("HOME")+IPANINI,QSettings::IniFormat);
        QMapIterator<QString, QString> i(accessTokenMap);
        while (i.hasNext())
        {
            i.next();
            settings.setValue(i.key(),i.value());
        }
        //需要添加授权时间
       // qDebug()<<"setting"<<settings.allKeys();
        emit FinishLogin();
    }
}

void IOauth::ReadStoreFile()
{
    QSettings settings(qgetenv("HOME")+IPANINI,QSettings::IniFormat);
    accessTokenMap["oauth_token"] = settings.value("oauth_token").toString();
    accessTokenMap["oauth_token_secret"] = settings.value("oauth_token_secret").toString();
    accessTokenMap["user_id"] =settings.value("user_id").toString();
    accessTokenMap["charged_dir"] =settings.value("charged_dir").toString();
}

 void IOauth::ReadUserInfo()
 {
    QString baseUrl =ACCOUNTINFO;
  // qDebug()<<"baseUrl"<<baseUrl;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    getUserInfoReply = networkManager->get(request);
    QEventLoop loop;
    connect(getUserInfoReply, SIGNAL(readyRead()), this, SLOT(slotReadUserInfo()));
    connect(getUserInfoReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
 }

 void IOauth::slotReadUserInfo()
 {
    QString data = getUserInfoReply->readAll();
    int result = getUserInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(result == 200)
    {
        ReadDataToMap(data,userInfo);
    }

 }

 QUrl IOauth::CreateUrL(const QString& baseUrl,QMap<QString,QString>& constData)
 {
     GetConstData(constData);
     QString baseString = GetOauthSignature("GET&",baseUrl,constData);
     QString Secret = consumer_secret+"&"+accessTokenMap["oauth_token_secret"];
     QString oauthSignature = HmacSha1(Secret.toAscii(),baseString.toAscii());
   //  qDebug()<<"oauth_signature:"<<oauthSignature<<endl;
     QString Uri =baseUrl+"?";
     QString para = CreatePara(constData);
     para.append("&oauth_signature="+QUrl::toPercentEncoding(oauthSignature));
     Uri.append(para);
    // qDebug()<<Uri;
     QUrl outputUrl ;
     outputUrl.setEncodedUrl(Uri.toAscii());
     return outputUrl;
 }

 void IOauth::ReadFolderInfo(const QString &path,int page,int pageSize)
 {
     QString baseUrl =METADATA+QUrl::toPercentEncoding(path);
    // qDebug()<<"baseUrl"<<baseUrl;
     QMap<QString,QString> constData;
     constData["oauth_token"] = accessTokenMap["oauth_token"];
     constData["page"] = QString::number(page);
     constData["page_size"] = QString::number(pageSize);
     QUrl outputUrl = CreateUrL(baseUrl,constData);
     request.setUrl(outputUrl);
     getFolderInfoReply = networkManager->get(request);
     folderString = "" ;
     qDebug()<<"ReadFolderInfo folder";
     QEventLoop loop;
     connect(getFolderInfoReply, SIGNAL(readyRead()), this, SLOT(slotReadFolderInfo()));
     connect(getFolderInfoReply,SIGNAL(finished()),&loop,SLOT(quit()));
     loop.exec();
     disconnect(getFolderInfoReply,SIGNAL(finished()),&loop,SLOT(quit()));
     disconnect(getFolderInfoReply, SIGNAL(readyRead()), this, SLOT(slotReadFolderInfo()));
    ParseJson(folderString);
 }

 void IOauth::ParseJson(const QString &jsonString)
 {
     //qDebug()<<jsonString;
     //sha1 加密格式
     QScriptEngine engine;
     QScriptValue json;
      json = engine.evaluate("("+ jsonString+")");

     QScriptValueIterator other(json);
     foldercontent.clear();
     folderInfo.clear();
     while(other.hasNext())
     {
         other.next();
         if(other.name() =="files")
             continue;
         folderInfo[other.name()] = other.value().toString();
     }
     if(json.property("files").isArray())
     {
         QScriptValueIterator it(json.property("files"));
         while (it.hasNext())
         {
             it.next();
             //map[it.name()]=it.value().toString();
          // qDebug()<<it.name()<<":"<< it.value().property("name").toString()<<"type"<<it.value().property("type").toString();
            FolderStruct fs(it.value().property("name").toString(),it.value().property("is_deleted").toBool(),it.value().property("rev").toInteger(),
                                                 it.value().property("file_id").toUInt32(),it.value().property("create_time").toString(),it.value().property("modify_time").toString(),
                                                 it.value().property("type").toString(),it.value().property("size").toUInt32()
                                                 );
             foldercontent[it.value().property("name").toString()]=fs;
         }
         FolderStruct fs(it.value().property("name").toString(),it.value().property("is_deleted").toBool(),it.value().property("rev").toInteger(),
                                              it.value().property("file_id").toUInt32(),it.value().property("create_time").toString(),it.value().property("modify_time").toString(),
                                              it.value().property("type").toString(),it.value().property("size").toUInt32()
                                              );
          foldercontent[it.value().property("name").toString()]=fs;
     }
 }

 void IOauth::slotReadFolderInfo(void)
 {

     int result = getFolderInfoReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
     if(result == 200)
     {
            folderString    += getFolderInfoReply->readAll();
     }
 }


 void IOauth::Folders(QMap<QString, QString> &cfolderInfo, QMap<QString, FolderStruct> &cfoldercontent)
 {
    cfolderInfo = this->folderInfo;
    cfoldercontent = this->foldercontent;
 }

 void  IOauth::CreateFolder(const QString& folderName)
 {
     QString baseUrl =CREATEFOLDER;
     QMap<QString,QString> constData;
     constData["root"]=ROOT;
     constData["path"]= QUrl::toPercentEncoding(folderName);
     constData["oauth_token"] = accessTokenMap["oauth_token"];
     QUrl outputUrl = CreateUrL(baseUrl,constData);
     request.setUrl(outputUrl);
     createFolderReply = networkManager->get(request);
     QEventLoop loop;
     connect(createFolderReply, SIGNAL(readyRead()), this, SLOT(slotCreateFolder()));
     connect(createFolderReply,SIGNAL(finished()),&loop,SLOT(quit()));
     loop.exec();

 }


 void IOauth::slotCreateFolder()
 {
     QString data = createFolderReply->readAll();
   //  qDebug()<<data;
     int result = createFolderReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
     if(result == 200)
     {
         ReadDataToMap(data,createFile);
     }
 }

 void IOauth::DeleteFileFolder(const QString &folderName)
 {
    QString baseUrl = DELETEFOLDER;
    QMap<QString,QString> constData;
    constData["root"]=ROOT;
    constData["path"]= QUrl::toPercentEncoding(folderName);
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["to_recycle"] = QString("True");
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    deleteFileReply = networkManager->get(request);
    QEventLoop loop;
    connect(deleteFileReply, SIGNAL(readyRead()), this, SLOT(slotDeleteFolder()));
    connect(deleteFileReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
 }

 void IOauth::slotDeleteFolder()
 {
    QString data = deleteFileReply->readAll();
   // qDebug()<<data;
    QMap<QString,QString> Msg;
    ReadDataToMap(data,Msg);
    if(Msg["msg"]!=QString::number(200))
    {
       // QMessageBox::Information(this,"",tr("删除文件出错"));
    }
    //http code 200表示成功
 }

 void IOauth::MoveFile(const QString &from, const QString &to)
 {
    QString baseUrl = MOVEFILE;
    QMap<QString,QString> constData;
    constData["root"]=ROOT;
    constData["from_path"]= QUrl::toPercentEncoding(from);
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["to_path"] = QUrl::toPercentEncoding(to);
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    moveFileReply = networkManager->get(request);
    QEventLoop loop;
    connect(moveFileReply, SIGNAL(readyRead()), this, SLOT(slotMoveFile()));
    connect(moveFileReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
 }

void IOauth::slotMoveFile()
 {
    QString data = moveFileReply->readAll();
   // qDebug()<<data;
    ReadDataToMap(data,moveFile);
 }

void IOauth::CopyFile(const QString &from, const QString &to)
{
    QString baseUrl = COPYFILE;
    QMap<QString,QString> constData;
    constData["root"]=ROOT;
    constData["from_path"]= QUrl::toPercentEncoding(from);
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["to_path"] = QUrl::toPercentEncoding(to);
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    copyFileReply = networkManager->get(request);
    QEventLoop loop;
    connect(copyFileReply, SIGNAL(readyRead()), this, SLOT(slotCopyFile()));
    connect(copyFileReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
}
void IOauth::slotCopyFile()
 {
    QString data = copyFileReply->readAll();
  //  qDebug()<<data;
    ReadDataToMap(data,copyFile);
 }

void IOauth::setUploadFile(const QString &file, const QString &to)
{
    uploadLocalFile = file;
    uploadRemoteFile = to;
}

void IOauth::ReadUploadUrl(const QString& filename,const QString& to)
{
    setUploadFile(filename,to);
    QString baseUrl = UPLOADLOCATE;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    //QNetworkAccessManager* uploadNetwork = new QNetworkAccessManager();
    qDebug()<<"ReadUploadUrl:"<<filename;
    getUploadUrlReply = networkManager->get(request);
    QEventLoop loop;
    connect(getUploadUrlReply, SIGNAL(readyRead()), this, SLOT(slotReadUploadUrl()));
    connect(getUploadUrlReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
   // delete uploadNetwork;
}

void IOauth::slotReadUploadUrl()
{
    QString data = getUploadUrlReply->readAll();
    ReadDataToMap(data,uploadUrl);
    UploadFile(uploadLocalFile,uploadRemoteFile);
}

void IOauth::UploadFile(const QString& filename,const QString& to)
{
    QString baseUrl =uploadUrl["url"]+V+"/fileops/upload_file";
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["overwrite"] = QString("True");
    constData["root"] =ROOT;
   // constData["path"]=QUrl::toPercentEncoding("/tupian/"+QFileInfo(filename).fileName());
    constData["path"]=QUrl::toPercentEncoding(to);
    GetConstData(constData);
    QString baseString = GetOauthSignature("POST&",baseUrl,constData);
    QString Secret = consumer_secret+"&"+accessTokenMap["oauth_token_secret"];
    QString oauthSignature = HmacSha1(Secret.toAscii(),baseString.toAscii());
    QString Uri =baseUrl+"?";
    QString para = CreatePara(constData);
    para.append("&oauth_signature="+QUrl::toPercentEncoding(oauthSignature));
    Uri.append(para);
   // qDebug()<<Uri;
    QUrl outputUrl ;
    outputUrl.setEncodedUrl(Uri.toAscii());
    QByteArray data;
    QNetworkRequest request(outputUrl);
    QNetworkAccessManager manager;
    QByteArray needToUploadFile;
    QFile openfile;
    QFileInfo fileInfo(filename);
    openfile.setFileName(filename);
    openfile.open(QIODevice::ReadOnly);
    needToUploadFile = openfile.readAll();
    QString contentType;
    data = CreatePostData(needToUploadFile,contentType,filename);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType.toAscii());
    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(data.size()).toString());
    postData = manager.post(request, data);
    QEventLoop loop;
    connect(postData, SIGNAL(readyRead()), this, SLOT(slotUploadFile()));
    connect(postData,SIGNAL(uploadProgress(qint64,qint64)),this,SIGNAL(SendUploadProgress(qint64,qint64)));
    connect(postData,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
}

QByteArray IOauth::CreatePostData(const QByteArray &fileData,QString& contentType,const QString& fileName)
{
    QByteArray data;
    QString crlf="\r\n";
    QString boundary="--------Michael"+getRandNonce();
    contentType="multipart/form-data; boundary="+boundary; //开始的头部信息
    QString endBoundary=crlf+"--"+boundary+"--"+crlf+crlf; //结束处
    boundary="--"+boundary+crlf; //每行数据开头
    QByteArray bond=boundary.toAscii();
    data.append(bond);
    data.append(QString("Content-Disposition: form-data; name=\"file\"; filename=\""
                        +QFileInfo(fileName).fileName()+"\""+crlf).toAscii());
    data.append(QString("Content-Type: application/octet-stream"+crlf).toAscii());
    data.append(crlf.toAscii());
    data.append(fileData);
    data.append(endBoundary.toAscii());
    return data;
}

void IOauth::slotUploadFile()
{
    QString data = postData->readAll();
    qDebug()<<data;
    ReadDataToMap(data,uploadFile);
}

void IOauth::DownloadFile(const QString &filename,const QString& localName)
{
    QString baseUrl = DOWNLOADFILE;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["root"] =ROOT;
    constData["path"] = QUrl::toPercentEncoding(filename);
    downloadFile = localName;
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    fileDownloadReply = networkManager->get(request);
    QEventLoop loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slotDownloadFile(QNetworkReply*)));
    connect(fileDownloadReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    disconnect(networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slotDownloadFile(QNetworkReply*)));
    disconnect(fileDownloadReply,SIGNAL(finished()),&loop,SLOT(quit()));
}
void IOauth::slotDownloadFile(QNetworkReply *downloadReply)
{

    int result = downloadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(result ==302 )
    {
        qDebug()<<"302";
        QUrl location = downloadReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        readData =  networkManager->get(QNetworkRequest(location));
        QEventLoop loop;
        connect(readData,SIGNAL(readyRead()),this,SLOT(slotDownloadFile()));
        connect(readData,SIGNAL(downloadProgress(qint64,qint64)),this,SIGNAL(SendDownloadProgress(qint64,qint64)));
        connect(readData,SIGNAL(finished()),&loop,SLOT(quit()));
        qApp->processEvents();
        loop.exec();
        disconnect(readData,SIGNAL(readyRead()),this,SLOT(slotDownloadFile()));
        disconnect(readData,SIGNAL(finished()),&loop,SLOT(quit()));
    }
    else if(result ==403 || result ==404 )
    {
        qDebug()<<"file not exist or file forbidden";
    }
}

bool IOauth::slotDownloadFile()
{
    qDebug()<<"slotDownloadFile";
    QFile file;
    file.setFileName(downloadFile);
   // QFileInfo fileInfo(downloadFile);
    if(!file.open(QIODevice::ReadWrite |QIODevice::Append))
    {
        qDebug()<<"not save download file"<<downloadFile<<file.errorString();
        disconnect(readData,SIGNAL(readyRead()),this,SLOT(slotDownloadFile()));
        return false;
    }
    QByteArray buffer = readData->readAll();
    file.write( buffer);
    file.close();
   // emit SendDownloadSize(buffer.size());
    return true;
}

//支持格式
//TYPE_IMG = ('gif', 'png', 'jpg', 'bmp', 'jpeg', 'jpe')
void IOauth::Thumbnail(const QString &filename)
{
    QString baseUrl = THUMBNAIL;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["root"] =ROOT;
    constData["path"] = QUrl::toPercentEncoding(filename);
    constData["height"] = height;
    constData["width"] = width;
    downloadFile = filename;
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    thumbnailReply = networkManager->get(request);
    QEventLoop loop;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(slotDownloadFile(QNetworkReply*)));
    connect(thumbnailReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
}

void IOauth::VistorWebKuaiPan()
{}
void IOauth::slotWebKuaiPan()
{}

QString IOauth::GetOauthSignature(const QString &method, const QString &base_uri, const QMap<QString, QString> &paramesters)
{
    QString para;
    para = CreatePara(paramesters);
    QString Signature = method + QUrl::toPercentEncoding(base_uri)  +"&"+ QUrl::toPercentEncoding(para);
   // qDebug()<<"basestring:"<<Signature<<endl;
    return Signature;
}

void IOauth::ShareFile(const QString &filename,QString& url)
{
    QString baseUrl = SHAREURL;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["root"] =ROOT;
    constData["path"] = QUrl::toPercentEncoding(filename);
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    shareFileReply = networkManager->get(request);
    QEventLoop loop;
    connect(shareFileReply, SIGNAL(readyRead()), this, SLOT(slotShareFile()));
    connect(shareFileReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    url = shareFile["url"];
}

void IOauth::slotShareFile()
{
    QString data = shareFileReply->readAll();
    int result = shareFileReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(result == 200)
    {
        ReadDataToMap(data,shareFile);
    }
}

void IOauth::History(const QString &filename,QMap<QString,QString>& backinfo)
{
    QString baseUrl = HISTORY;
    QMap<QString,QString> constData;
    constData["oauth_token"] = accessTokenMap["oauth_token"];
    constData["root"] =ROOT;
    constData["path"] = QUrl::toPercentEncoding(filename);
    QUrl outputUrl = CreateUrL(baseUrl,constData);
    request.setUrl(outputUrl);
    historyReply = networkManager->get(request);
    QEventLoop loop;
    connect(historyReply, SIGNAL(readyRead()), this, SLOT(slotHistory()));
    connect(historyReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    backinfo = historyFile;
}

void IOauth::slotHistory()
{
    QString data = historyReply->readAll();
    /*
    ReadDataToMap(data,historyFile);

    */
    QScriptEngine engine;
    QScriptValue json;
     json = engine.evaluate("("+ data+")");

    QScriptValueIterator other(json);
    while(other.hasNext())
    {
        other.next();
        if(other.name() =="files")
            continue;
        historyFile[other.name()] = other.value().toString();
    }
    if(json.property("files").isArray())
    {
        QScriptValueIterator it(json.property("files"));
        while (it.hasNext())
        {
            it.next();
            historyFile["file_id"] = it.value().property("file_id").toString();
            historyFile["rev"] = it.value().property("rev").toString();
            historyFile["create_time"] = it.value().property("create_time").toString();
        }
    }
  qDebug()<<historyFile;
}

//文档转换没有实现
QMap<QString,QString> IOauth::UserInfo()
{
    return this->userInfo;
}

QString IOauth::HmacSha1(QByteArray key, QByteArray baseString)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
     //qDebug()<<"hmasha1"<<hashed.toBase64()<<endl;
     return hashed.toBase64();
}

// QTextCodec *codec = QTextCodec::codecForName("utf-8");
//            QFile file("out.txt");
//                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//                    return;
//                QTextStream out(&file);
// out<< it.value().property("name").toString();
