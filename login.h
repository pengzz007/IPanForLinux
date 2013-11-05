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
#ifndef LOGIN_H
#define LOGIN_H
#include"ui_login.h"
#include<QDialog>
#include<QProcess>
#include<QStringList>
#include"public.h"
const QString REG ="http://www.kuaipan.cn/welcome.htm?channel=_ipanforlinux";
class Login:public QDialog
{
    Q_OBJECT
    public:
        Login( QWidget * parent = NULL);
        ~Login();
     signals:
        void HaveAccount();
    private slots:
        void Ok(void);
    private:
        Ui::Dialog ui;
};
#endif // LOGIN_H
