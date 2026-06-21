#ifndef MANAGECELLS_H
#define MANAGECELLS_H

#include <QDialog>
#include <QVector>
#include "cell.h"
#include "prisoner.h"

namespace Ui {
    class ManageCells;
}

class ManageCells : public QDialog
{
    Q_OBJECT

public:
    explicit ManageCells(QWidget *parent = nullptr);
    ~ManageCells();

private slots:
    void loadCells();
    void on_addCell_btn_clicked();
    void on_assignPrisoner_btn_clicked();
    void on_removePrisoner_btn_clicked();

    void on_exit_btn_clicked();

    void on_removeCell_btn_clicked();

    void on_viewPrisoners_btn_clicked();

private:
    Ui::ManageCells *ui;
    QVector<Cell> cells;
    void refreshTable();
    QList<Prisoner> loadActivePrisoners();

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // MANAGECELLS_H

