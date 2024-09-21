#ifndef REMOVEPROJECTDIALOG_H
#define REMOVEPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProjectDialog;
}

class RemoveProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveProjectDialog(QWidget *parent = nullptr);
    ~RemoveProjectDialog();
public:
    bool isRemoveFromLocal(); //复选框是否选中
private:
    Ui::RemoveProjectDialog *ui;
};

#endif // REMOVEPROJECTDIALOG_H
