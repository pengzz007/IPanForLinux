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
#include"inotifythread.h"
#include<QtDebug>
InotifyThread::InotifyThread(QObject *parent):QThread(parent)
{
    isStop = false;
}

InotifyThread::~InotifyThread()
{
    qDebug()<<"~InotifyThread";
    for(int i=0;i<fds.size();i++)
    {
        inotify_rm_watch(inotify_fd,fds.at(i));
    }
    close(inotify_fd);
}

void InotifyThread::Init(QQueue<InotifyInfo> *queue)
{
    mutex.lock();
    this->queue = queue;
    mutex.unlock();
}

void InotifyThread::Stop()
{
    QThread::quit();
}

void InotifyThread::SetStop(bool stop)
{
    isStop = stop;
}

void InotifyThread::run()
{

    int cnt,index;
    fd_set 	rfds;
    char buf[10240];
    struct inotify_event * event;
    inotify_fd = inotify_init();
    qDebug()<<inotify_fd;
    if (inotify_fd < 0)
     {
        this->exit();
     }
    int watchfd;
    qDebug()<<"dirs"<<dirs.size();
    for(int i =0;i<dirs.size();i++)
    {
        if ((watchfd=inotify_add_watch (inotify_fd,dirs.at(i).toStdString().c_str(),IN_ALL_EVENTS)) < 0)
        {
            qDebug()<<"watchfd:"<<watchfd;
            ERROR("inotify_add_watch");
            printf("Watch %s Error\n", dirs.at(i).toStdString().c_str());
            continue;
        }
        fds.append(watchfd);
    }
    qDebug()<<"fds:"<<fds.size();
    while (!isStop)
     {
//        if(isStop)
//        {
//            qDebug()<<"stop thread";
//           // QThread::quit();
//           break;
//        }
        FD_ZERO(&rfds);
        FD_SET(inotify_fd, &rfds);
        if (select(FD_SETSIZE, &rfds, NULL, NULL, NULL) > 0)
        {
            if (FD_ISSET(inotify_fd, &rfds))
            {
                cnt = read(inotify_fd, buf, 10240);
                index = 0;
                while (cnt > index)
                {
                    event = (struct inotify_event *)&buf[index];
                    index = index + event->len + 16;//offsetof(struct inotify_event, name);
                    EventHandle(event);
                }
                fflush(NULL);
            }
            if(isStop)
            {
                qDebug()<<"stop thread";
               // QThread::quit();
               break;
            }
        }
        this->usleep(1000);
    }
    qDebug()<<"thread end";
    isStop = false;
}

void InotifyThread::AddWatchDir(const QStringList &dirs)
{
    this->dirs = dirs;
}

void InotifyThread::EventHandle(inotify_event *event)
{

    QString fileName;
    if(event->mask & IN_ISDIR)
    {
        type ="folder";
    }
    else
    {
        type = "file";
    }
    for(int i=0;i<fds.size();i++)
    {
        if(fds.at(i) == event->wd)
        {
            fileName = dirs.at(i)+"/"+QString(event->name);
        }
        else
            continue;
    }

    printf(" mask: %08X",event->mask);
    qDebug()<<"inotifyThread:"<<fileName;
    if(event->mask&IN_CREATE)
    {
            //emit SendNotify(fileName,CREATE,type);
        InotifyInfo node(fileName,CREATE,type);
       // InotifyInfo* node =new InotifyInfo (fileName,CREATE,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        this->AddWatchDir(fileName);
     }
     else if(event->mask&IN_DELETE)
    {
            //emit SendNotify(fileName,DELETE,type);
      //  InotifyInfo* node =new InotifyInfo(fileName,DELETE,type);
        InotifyInfo node(fileName,DELETE,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        this->DeleteWatchDir(fileName);
     }
    else if(event->mask &IN_MOVED_TO)
    {
          //  emit SendNotify(fileName,MOVETO,type);
       // InotifyInfo* node =new InotifyInfo(fileName,MOVETO,type);
        InotifyInfo node(fileName,MOVETO,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        this->AddWatchDir(fileName);
     }
    else if(event->mask &IN_MOVED_FROM)
    {
         //   emit SendNotify(fileName,MOVEFROM,type);
        InotifyInfo node(fileName,MOVEFROM,type);
        //InotifyInfo* node =new InotifyInfo(fileName,MOVEFROM,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        this->AddWatchDir(fileName);
     }
    else if(event->mask &  IN_CLOSE_WRITE)
    {
           // emit SendNotify(fileName,CLOSEWRITE,type);
        InotifyInfo node(fileName,CLOSEWRITE,type);
       // InotifyInfo* node =new InotifyInfo(fileName,CLOSEWRITE,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        //目前不用调用函数AddWatchDir
      }
    else if(event->mask & IN_MODIFY)
    {
          //  emit SendNotify(fileName,MODIFY,type);
        InotifyInfo node(fileName,MODIFY,type);
        //InotifyInfo* node =new InotifyInfo(fileName,MODIFY,type);
        mutex.lock();
        queue->enqueue(node);
        mutex.unlock();
        //目前不用调用函数AddWatchDir
     }
}

void InotifyThread::AddWatchDir(const QString &dir)
{
    dirs.append(dir);
    int watchfd=inotify_add_watch (inotify_fd,dir.toStdString().c_str(),IN_ALL_EVENTS);
    fds.append(watchfd);
}

void InotifyThread::DeleteWatchDir(const QString &dir)
{
    for(int i=0;i<dirs.size();i++)
    {
        if (dirs[i] == dir)
        {
            dirs.removeAt(i);
            inotify_rm_watch(inotify_fd,fds.at(i));
            fds.removeAt(i);
            break;
        }
    }
}
