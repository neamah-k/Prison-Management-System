#ifndef VIEWCELLS_H
#define VIEWCELLS_H

#include <QDialog>
#include "cell.h"

namespace Ui {
class ViewCells;
}

class ViewCells : public QDialog
{
    Q_OBJECT

public:
    explicit ViewCells(QWidget *parent = nullptr);
    ~ViewCells();

private slots:

    void on_viewPrisoners_btn_clicked();

    void on_exit_btn_clicked();

private:
    void loadCells();
    void refreshTable();

    Ui::ViewCells *ui;
    QList<Cell> cells;
    QString blockAFile = "blockA_cells.dat";
    QString blockBFile = "blockB_cells.dat";
};

#endif // VIEWCELLS_H
