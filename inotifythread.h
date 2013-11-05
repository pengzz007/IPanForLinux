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
#ifndef INOTIFYTHREAD_H
#define INOTIRYTHREAD_H
#include<QThread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/inotify.h>
#include <signal.h>
#include<QDir>
#include<QString>
#include<QStringList>
#include<QList>
#include"public.h"
#include<error.h>
#include<errno.h>
#include<QQueue>
#include<QMutex>
#define ERROR(text) error(1, errno, "%s", text)

class InotifyThread:public QThread
{
    Q_OBJECT
    public:
        InotifyThread(QObject* parent);
        ~InotifyThread();
        void Init(QQueue<InotifyInfo>* queue);
        void Stop();
        void AddWatchDir(const QStringList& dirs);
        void AddWatchDir(const QString& dir);
        void DeleteWatchDir(const QString& dir);
        void SetStop(bool stop);
    protected:
            void run();
    signals:
            void SendNotify(const QString& filename,const QString& method,const QString& filetype);
    private:
            void EventHandle(struct inotify_event *event);
            QStringList dirs;
            QList<int> fds;
            QString type;
            int  inotify_fd;
            bool isStop;
            QQueue<InotifyInfo>* queue;
            QMutex mutex;
};

#endif
