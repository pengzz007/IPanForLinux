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
#ifndef PREFERENCES_H
#define PREFERENCES_H
#include"ui_preferences.h"
#include<QDialog>
#include<QWidget>
#include<QTextCodec>
#include<QMap>
#include<QString>
#include<QFile>
#include<QSettings>
#include<QtGlobal>
#include<QFileDialog>
#include<QDir>
#include"public.h"

const int ACCOUNT =0;
const int  ADVANCED =1;
const int ABOUT = 2;

class Preference:public QDialog
{
    Q_OBJECT
    public:
        Preference(QWidget* parent);
        ~Preference();
        void GetUserInfo(const QMap<QString,QString>& userInfo);
    signals:
        void removeToken();
        void ChangeSavePath(const QString& savePath);
    public slots:
        void Show();
    private:
       void closeEvent(QCloseEvent *event);
        Ui::PreferenceDialog ui;
        QMap<QString,QString> userInfo;
        QString SavePath;
    private slots:
        void ShowData(int pos);
        void Unlink();
        void MoveDir();
        void CloseWindow();
        void setRunSys(int val);
};

#endif // PREFERENCES_H
