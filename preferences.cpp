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
#ifndef PREFERENCES_CPP
#define PREFERENCES_CPP
#include<QtDebug>
#include"preferences.h"
Preference::Preference(QWidget *parent):QDialog(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    ui.setupUi(this);
    ui.preferenceTab->setTabText(ACCOUNT,QString("账号"));
    ui.preferenceTab->setTabText(ADVANCED,QString("高级"));
    ui.preferenceTab->setTabText(ABOUT,QString("关于"));
    connect(ui.preferenceTab,SIGNAL(currentChanged(int)),this,SLOT(ShowData(int)));
    connect(ui.unlinkButton,SIGNAL(clicked()),this,SLOT(Unlink()));
    QSettings settings(qgetenv("HOME")+SYSTEM,QSettings::IniFormat);
    if(settings.value("SavePath").toString().isEmpty())
    {
        QString Home = qgetenv("HOME");
        settings.setValue("SavePath",Home + "/IPan");
    }
    SavePath = settings.value("SavePath").toString();
    ui.dirLine->setText(SavePath);
    emit ChangeSavePath(SavePath);
    connect(ui.moveButton,SIGNAL(clicked()),this,SLOT(MoveDir()));
    connect(ui.closeButton,SIGNAL(clicked()),this,SLOT(CloseWindow()));
    connect(ui.runsysche,SIGNAL(stateChanged(int)),this,SLOT(setRunSys(int)));
    ui.preferenceTab->setCurrentIndex(ACCOUNT);
    if(settings.value("RunSys").toString().isEmpty())
    {
        ui.runsysche->setCheckState(Qt::Unchecked);
    }
    else
    {
        int val = settings.value("RunSys").toInt();
        if(val == 0)
            ui.runsysche->setCheckState(Qt::Unchecked);
        else if(val == 2)
            ui.runsysche->setCheckState(Qt::Checked);
    }
}

Preference::~Preference()
{}

void Preference::setRunSys(int val)
{
     QSettings settings(qgetenv("HOME")+SYSTEM,QSettings::IniFormat);
     settings.setValue("RunSys",val);
}

void Preference::Show()
{
    ShowData(ACCOUNT);
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->exec();
}

void Preference::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    //qDebug()<<"closeEvent";
}

void Preference::ShowData(int pos)
{
    if(pos == ACCOUNT)
    {
        ui.accountLabel->setText(userInfo["user_name"]);
        QString total = userInfo["quota_total"];
        int totalInt =total.toLongLong()/1024/1024/1024;
        ui.spaceLabel->setText(QString("用户空间总量:")+QString::number(totalInt)+"GB");
        QString used = userInfo["quota_used"];
        long usedInt = used.toLongLong()/1024;
        QString unit= "KB";
        if(usedInt>1000)
        {
            usedInt = usedInt/1024;
            unit = "MB";
        }
        if(usedInt>1000)
        {
            usedInt = usedInt/1024;
            unit = "GB";
        }
        QString usedSpace =QString("已经使用的空间:")+QString::number(usedInt)+unit;
        ui.usedSpaceLabel->setText(usedSpace);
        int maxFileSize = userInfo["max_file_size"].toLongLong()/1024/1024;
        QString maxFileSizeStr =QString("允许上传最大文件:")+QString::number(maxFileSize)+"MB";
        ui.maxFileSizeLabel->setText(maxFileSizeStr);
    }
    else if(pos == ADVANCED)
    {

    }
}

void Preference::GetUserInfo(const QMap<QString, QString> &userInfo)
{
    this->userInfo = userInfo;
}

void Preference::Unlink()
{
    QFile file(qgetenv("HOME")+IPANINI);
    file.remove();
    ui.accountLabel->setText("");
    ui.spaceLabel->setText(QString("用户空间总量:")+"0 GB");
    QString maxFileSizeStr =QString("允许上传最大文件:")+"0 MB";
    ui.maxFileSizeLabel->setText(maxFileSizeStr);
    QString usedSpace =QString("已经使用的空间:")+" 0";
    ui.usedSpaceLabel->setText(usedSpace);
    emit removeToken();
}

void Preference::MoveDir()
{
    QString dir = QFileDialog::getExistingDirectory(this,QString("选择目录"),qgetenv("HOME"));
    ui.dirLine->setText(dir);
    QSettings settings(qgetenv("HOME")+SYSTEM,QSettings::IniFormat);
    settings.setValue("SavePath",ui.dirLine->text());
}

void Preference::CloseWindow()
{
    QDir file;
    if(!file.exists(SavePath))
        file.mkdir(SavePath);
    close();
}

#endif // PREFERENCES_CPP
