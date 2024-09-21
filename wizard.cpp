#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::done(int result)
{
    //通知向导界面关闭,组件被隐藏,而不是被析构
    //判断点击的是finish按钮还是cancle按钮
    if(result==QWizard::Rejected)
    {
        qDebug()<<"选择取消";
        return QWizard::done(result);
    }
    QString fileName;
    QString filePath;
    //获取向导界面执行过程中,创建的文件夹的名字和路径
    ui->wizardPage1->getProjectSetting(fileName,filePath);
    //获取到page界面上的文件夹名称和路径,发射信号通知TreeWidget创建并添加路径
    emit signalProjectSetting(fileName,filePath);
    QWizard::done(result); //运行父类的done函数,处理其他问题
}
