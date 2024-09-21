#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include "wizard.h"
#include "projecttree.h"
#include "projecttreewidget.h"
#include "pictureshow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMinimumSize(1000,600);
    //获取MainWindow的菜单栏,创建文件菜单,并添加到菜单栏
    QMenu *fileMenu= menuBar()->addMenu(tr("文件(&F)"));
    //创建创建项目行为和打开项目行为
    QAction *actCreateProject=new QAction(QIcon(":/resource/icon/createpro.png"),tr("创建项目"),this);
    actCreateProject->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up));
    fileMenu->addAction(actCreateProject);
    QAction *actOpenProject=new QAction(QIcon(":/resource/icon/openpro.png"),tr("打开项目"),this);
    actOpenProject->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down));
    fileMenu->addAction(actOpenProject);

    //创建设置菜单
    QMenu *musicMenu=menuBar()->addMenu(tr("设置(&S)"));
    //创建设置背景音乐的行为
    QAction *actMusic=new QAction(QIcon(":/resource/icon/music.png"),tr("背景音乐"),this);
    actMusic->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    musicMenu->addAction(actMusic);

    connect(actCreateProject,&QAction::triggered,this,&MainWindow::slotCreateProject);
    connect(actOpenProject,&QAction::triggered,this,&MainWindow::slotOpenProject);

    projectTree=new ProjectTree(); //projectTree就是一个QDialog,存储了treeWidget
    ui->projectVLayout->addWidget(projectTree); //将projectTree添加到mainwindow左侧显示

    //获取projectTree界面上的treeWidget
    //也可以直接链接projecttree,再通过projecttree传递给treewidget
    QTreeWidget *treeWidget=dynamic_cast<ProjectTree *>(projectTree)->getTreeWidget();
    ProjectTreeWidget *projectTreeWidget=dynamic_cast<ProjectTreeWidget *>(treeWidget);
    connect(this,&MainWindow::signalOpenPath,projectTreeWidget,&ProjectTreeWidget::slotOpenProject);

    pictureShow=new PictureShow();
    ui->pictureVLayout->addWidget(pictureShow);

    PictureShow *projectPictureShow=dynamic_cast<PictureShow *>(pictureShow);

    //双击treewidget项,获取项路径,发射信号通知pictureshow加载图片
    connect(projectTreeWidget,&ProjectTreeWidget::signalUpdateSelected,projectPictureShow,&PictureShow::slotSelectedItem);

    //连接点击按钮信号和更新treewidget项的槽函数
    connect(projectPictureShow,&PictureShow::signalPreviousClicked,projectTreeWidget,&ProjectTreeWidget::slotPreviousShow);
    connect(projectPictureShow,&PictureShow::signalNextClicked,projectTreeWidget,&ProjectTreeWidget::slotNextShow);

    //更新图片槽函数,发出信号通知pictureshow更换图片
    connect(projectTreeWidget,&ProjectTreeWidget::signalUpdatePicture,projectPictureShow,&PictureShow::slotUpdatePicture);
    connect(projectTreeWidget,&ProjectTreeWidget::signalClearPicture,projectPictureShow,&PictureShow::slotClearPicture);

    connect(actMusic,&QAction::triggered,projectTreeWidget,&ProjectTreeWidget::slotSetMusic);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    PictureShow *projectPictureShow=dynamic_cast<PictureShow *>(pictureShow);
    projectPictureShow->reloadPicture();
    QMainWindow::resizeEvent(event);
}

//创建项目的action槽函数,打开一个向导页面
void MainWindow::slotCreateProject(bool checked)
{
    Q_UNUSED(checked)
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    QWizardPage *page=wizard.page(0);
    page->setTitle(tr("设置项目配置"));
    //wizard界面发射信号无法直接传递给QTreeWidget对象,通过其父对象传递
    //也可以获取projectTree上的treeWidget对象
    connect(&wizard,&Wizard::signalProjectSetting,dynamic_cast<ProjectTree *>(projectTree),&ProjectTree::addProjectToTree);
    wizard.show();
    wizard.exec();
    disconnect(&wizard);
}

void MainWindow::slotOpenProject(bool checked)
{
    Q_UNUSED(checked)
    QFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入的文件夹"));
    fileDialog.setDirectory(QDir::currentPath());
    fileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(!fileDialog.exec())
    {
        return;
    }
    fileNames=fileDialog.selectedFiles();
    if(fileNames.size()<=0)
    {
        return;
    }
    QString importPath=fileNames.at(0);
    emit signalOpenPath(importPath); //将打开的文件夹的路径传递出去
}



