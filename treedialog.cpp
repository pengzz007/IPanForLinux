#include "treedialog.h"

treedialog::treedialog(QWidget *parent) :QDialog(parent)
{
    ui.setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose, false);
}

treedialog::~treedialog()
{}

void treedialog::Init(IOauth *oauth, const QString &rootpath)
{
    QMutex mutex;
    mutex.lock();
    this->oauth = oauth;
    this->rootpath = rootpath;
    mutex.unlock();
}

void treedialog::Show()
{
    QFileSystemModel model;
    model.setRootPath(rootpath);
    //qDebug()<<"treedialog"<<rootpath;
    TreeView tree(this);
    tree.Init(oauth);
    tree.setModel(&model);
    tree.setRootIndex(model.index(rootpath));
    tree.setColumnHidden(1,true);
    tree.setColumnHidden(2,true);
    tree.setColumnHidden(3,true);
    tree.setAnimated(false);
    tree.setIndentation(20);
    tree.setSortingEnabled(true);
    tree.setWindowTitle(QString("共享"));
    tree.resize(640, 480);
    tree.show();
    this->exec();
}
