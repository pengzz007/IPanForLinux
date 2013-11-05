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
#include "filetree.h"
TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
     QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
}

void TreeView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        fileName = this->indexAt(e->globalPos()).data().toString();
       // emit SendFileName(filename);
        QMenu* menu = new QMenu();
        QAction* share = new QAction(QString("分享"),this);
        connect(share,SIGNAL(triggered()),this,SLOT(slotShareInfo()));
        menu->addAction(share);
        menu->exec(e->globalPos());
        delete menu;
    }
}

void TreeView::Init(IOauth *oauth)
{
    QMutex mutex;
    mutex.lock();
    this->oauth=oauth;
    mutex.unlock();
}

void TreeView::slotShareInfo()
{
   // emit SendFileName(fileName);
    QMutex mutex;
    QString url;
    mutex.lock();
    oauth->ShareFile(fileName,url);
    mutex.unlock();
    QMessageBox prompt;
    prompt.setWindowTitle(QString("分享"));
    prompt.setAttribute(Qt::WA_QuitOnClose, false);
    prompt.setText(QString("地址:")+url);
    prompt.exec();
}

