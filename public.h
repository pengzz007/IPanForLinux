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
#ifndef PUBLIC_H
#define PUBLIC_H
const QString IPANINI ="/.ipan/ipan.ini";
const QString SYSTEM ="/.ipan/system.ini";
const QString BROWSER ="firefox";
const QString CONSUMERKEY = "";
const QString CONSUMERSECRET ="";
const int PAGESIZE = 10;

class FolderStruct
{
   // Q_OBJECT
    public:
        FolderStruct(QString name,bool is_deleted,int rev,long file_id,QString create_time,QString modify_time,QString type,long size)
        {
            this->name =name;
            this->is_deleted = is_deleted;
            this->rev = rev;
            this->file_id = file_id;
            this->create_time = create_time;
            this->modify_time = modify_time;
            this->type =type;
            this->size = size;
        }
        FolderStruct(void){}
        ~FolderStruct(){}
        QString name;
        bool is_deleted;
        int rev;
        long file_id;
        QString create_time;
        QString modify_time;
        QString type;
        long size;
} ;

class InotifyInfo
{
    public:
        InotifyInfo(QString filename,QString method,QString type)
        {
            this->filename = filename;
            this->method = method;
            this->type = type;
        }
        QString filename;
        QString method;
        QString type;
};

const QString CREATE ="create";
const QString DELETE ="delete";
const QString MOVETO ="moveto";
const QString MOVEFROM ="movefrom";
const QString MODIFY ="modify";
const QString WRITE ="write";
const QString FOLDER= "folder";
const QString WFILE ="file";
const QString CLOSEWRITE = "closewrite";
const QString CLOSE = "close";

const QString SOFTWARENAME ="IPan For Linux";
#endif // PUBLIC_H
