#ifndef EDITPRISONER_H
#define EDITPRISONER_H

#include <QDialog>
#include "prisoner.h"

namespace Ui {
class EditPrisoner;
}

class EditPrisoner : public QDialog
{
    Q_OBJECT

public:
    explicit EditPrisoner(Prisoner prisoner, QWidget *parent = nullptr);
    ~EditPrisoner();
    Prisoner getEditedPrisoner() const;
private slots:
    void on_edit_btn_clicked();

private:
    Ui::EditPrisoner *ui;
    Prisoner prisoner;
};

#endif // EDITPRISONER_H

