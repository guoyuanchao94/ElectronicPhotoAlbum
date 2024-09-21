#ifndef PROJECTSETPAGE_H
#define PROJECTSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProjectSetPage;
}

class ProjectSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProjectSetPage(QWidget *parent = nullptr);
    ~ProjectSetPage();
public:
    bool isComplete() const override; //向导页面是否完成
    bool validatePage() override; //批准
public:
    void getProjectSetting(QString &name,QString &path); //获取page页面文本框的文件夹名字和路径
private slots:
    void on_btnBrowser_clicked(); //浏览按钮点击槽函数
private:
    Ui::ProjectSetPage *ui;
};

#endif // PROJECTSETPAGE_H
