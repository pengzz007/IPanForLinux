#ifndef FILEDOWNUPLIST_H
#define FILEDOWNUPLIST_H

#include <QDialog>
#include<QtDebug>
#include<QTextCodec>
#include<QStringList>
#include<QTableWidgetItem>
#include "ui_filelist.h"
const int COLUMNWIDTH =250;
class FileDownUpList : public QDialog
{
    Q_OBJECT
public:
    FileDownUpList(QWidget * parent = 0);
    ~FileDownUpList();
    void Clear();
    void Insert(const QStringList& fileline);
    void Insert(int row,int column,const QString& text);
    void Insert(const QString& filename);
private:
    Ui::FileListDialog* ui;
};

#endif // FILEDOWNUPLIST_H
