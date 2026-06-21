#include "viewcells.h"
#include "ui_viewcells.h"
#include <QFile>
#include <QDataStream>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "viewprisonersincell.h"

ViewCells::ViewCells(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewCells)
{
    ui->setupUi(this);
    loadCells();
      ui->cellsTableWidget->setColumnCount(4);

    ui->cellsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->cellsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->cellsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->cellsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    refreshTable();

}

ViewCells::~ViewCells()
{
    delete ui;
}

void ViewCells::loadCells()
{
    cells.clear();
    cells = Cell::loadAllCells();
}

void ViewCells::refreshTable()
{
    ui->cellsTableWidget->setRowCount(0);
    ui->cellsTableWidget->setColumnCount(5);
    QStringList headers = { "Block", "Cell No", "Capacity", "Assigned", "Guard" };
    ui->cellsTableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < cells.size(); ++i) {
        const Cell &c = cells[i];
        ui->cellsTableWidget->insertRow(i);
        ui->cellsTableWidget->setItem(i, 0, new QTableWidgetItem(c.getBlockName()));
        ui->cellsTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(c.getCellNumber())));
        ui->cellsTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(c.getCapacity())));
        ui->cellsTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(c.getAssignedPrisoners().size())));

        QString guard = c.getAssignedGuardID().isEmpty() ? "Unassigned" : c.getAssignedGuardID();
        QTableWidgetItem *item = new QTableWidgetItem(guard);
        if (guard == "Unassigned")
            item->setForeground(Qt::red);
        ui->cellsTableWidget->setItem(i, 4, item);
    }
}

void ViewCells::on_viewPrisoners_btn_clicked(){
    int row = ui->cellsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a cell first.");
        return;
    }

    Cell selectedCell = cells[row];
    ViewPrisonersInCell dialog(selectedCell, this);
    dialog.exec();
}

void ViewCells::on_exit_btn_clicked()
{
    close();
}
