#include "filedownuplist.h"
FileDownUpList::FileDownUpList(QWidget * parent):QDialog(parent)
{
    ui = new Ui::FileListDialog();
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QStringList headerlist;
    headerlist<<tr("文件")<<tr("速度")<<tr("完成百分比");
    ui->filelist->setColumnCount(3);
    ui->filelist->setHorizontalHeaderLabels(headerlist);
  //  ui->filelist->show();
}

FileDownUpList::~FileDownUpList()
{
    Clear();
    delete ui;
}

void FileDownUpList::Insert(const QStringList &fileline)
{
    int rowcount = ui->filelist->rowCount();
    ui->filelist->setRowCount(rowcount+1);
    for(int i =0;i<3;i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(fileline[i]);
        item->setTextAlignment(Qt::AlignCenter);
        if(i ==0)
            ui->filelist->setColumnWidth(0,COLUMNWIDTH);
        ui->filelist->setItem(rowcount,i,item);
    }
}

void FileDownUpList::Insert(int row, int column, const QString &text)
{
    QTableWidgetItem* item = new QTableWidgetItem(text);
    if(column ==0)
        ui->filelist->setColumnWidth(0,COLUMNWIDTH);
    ui->filelist->setItem(row,column,item);
}

void FileDownUpList::Insert(const QString& filename)
{
    int rowcount = ui->filelist->rowCount();
    ui->filelist->setRowCount(rowcount+1);
    QTableWidgetItem* item = new QTableWidgetItem(filename);
    ui->filelist->setColumnWidth(0,COLUMNWIDTH);
    item->setTextAlignment(Qt::AlignCenter);
    ui->filelist->setItem(rowcount,0,item);

}

void FileDownUpList::Clear()
{
    //ui->filelist->clear();
    int count = ui->filelist->rowCount();
    for(int i =0;i<count;i++)
    {
        ui->filelist->removeRow(i);
    }
}
