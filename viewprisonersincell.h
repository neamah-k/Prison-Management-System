#ifndef VIEWPRISONERSINCELL_H
#define VIEWPRISONERSINCELL_H

#include <QDialog>
#include "cell.h"
#include "prisoner.h"

namespace Ui {
class ViewPrisonersInCell;
}

class ViewPrisonersInCell : public QDialog
{
    Q_OBJECT

public:
    explicit ViewPrisonersInCell(Cell cell ,QWidget *parent = nullptr);
    ~ViewPrisonersInCell();

private slots:
    void on_exit_btn_clicked();

private:
    Ui::ViewPrisonersInCell *ui;
    void loadPrisonersOfCell();
    Cell cell;
    QList<Prisoner> cellPrisoners;

};

#endif // VIEWPRISONERSINCELL_H

