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
#ifndef TreeView_H
#define TreeView_H
/*
 *编码规则:槽都要以slot开头
 *常量都要大写
 *变量开头小写后面单词都大写,一个单词命名的变量小写
 *函数名单词都要大写
 */
#include <QTreeView>
#include<QMouseEvent>
#include<QMenu>
#include<QAction>
#include<QTextCodec>
#include<QString>
#include<QMutex>
#include"IOauth.h"
#include<QMessageBox>

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);
    void Init( IOauth* oauth);

protected:
    void mouseReleaseEvent(QMouseEvent *e);

 private:
    QString fileName;
    IOauth* oauth;
signals:
    void SendFileName(const QString& filename);
    
private slots:
    void slotShareInfo();
};

#endif // TreeView_H
