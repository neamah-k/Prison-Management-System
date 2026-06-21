#ifndef ADDPRISONER_H
#define ADDPRISONER_H

#include <QDialog>

namespace Ui {
class AddPrisoner;
}

class AddPrisoner : public QDialog
{
    Q_OBJECT

public:
    explicit AddPrisoner(QWidget *parent = nullptr);
    ~AddPrisoner();

private slots:
    void on_submit_btn_clicked();

private:
    Ui::AddPrisoner *ui;
};

#endif // ADDPRISONER_H
