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
#include"login.h"
Login::Login( QWidget * parent):QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.okButton,SIGNAL(clicked()),this,SLOT(Ok()));
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

Login::~Login()
{
}

void Login::Ok()
{
    QProcess* process;
    QStringList args;
    args<<REG;
    if(ui.noHaveAccountRadio->isChecked())
    {
        process = new QProcess(this);
        process->start(BROWSER,args);
    }
    else if(ui.HaveAccountRadio->isChecked())
    {
        this->close();
        emit HaveAccount();
    }
}
