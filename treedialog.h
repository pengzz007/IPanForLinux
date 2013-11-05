#ifndef TREEDIALOG_H
#define TREEDIALOG_H
#include"ui_treedialog.h"
#include <QDialog>
#include"IOauth.h"
#include<QString>
#include<QFileSystemModel>
#include"filetree.h"

class treedialog : public QDialog
{
    Q_OBJECT
public:
    explicit treedialog(QWidget *parent = 0);
    ~treedialog();
    void Init(IOauth* oauth,const QString& rootpath);
    void Show();
 private:
    Ui::treeDialog ui;
    IOauth* oauth;
    QString rootpath;
signals:
    
public slots:
    
};

#endif // TREEDIALOG_H
