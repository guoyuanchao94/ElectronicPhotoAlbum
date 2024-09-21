#include "removeprojectdialog.h"
#include "ui_removeprojectdialog.h"

RemoveProjectDialog::RemoveProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemoveProjectDialog)
{
    ui->setupUi(this);
}

RemoveProjectDialog::~RemoveProjectDialog()
{
    delete ui;
}

bool RemoveProjectDialog::isRemoveFromLocal()
{
    bool isChecked=ui->checkBox->isChecked();
    return isChecked;
}
