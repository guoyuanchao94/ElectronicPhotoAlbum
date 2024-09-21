#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>

namespace Ui {
class Wizard;
}

class Wizard : public QWizard
{
    Q_OBJECT

public:
    explicit Wizard(QWidget *parent = nullptr);
    ~Wizard();
protected:
    void done(int result) override;
private:
    Ui::Wizard *ui;
signals:
    void signalProjectSetting(const QString name,const QString path);
};

#endif // WIZARD_H
