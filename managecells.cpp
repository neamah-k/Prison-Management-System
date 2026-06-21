#include "managecells.h"
#include "ui_managecells.h"
#include <QFile>
#include <QDataStream>
#include <QList>
#include <QInputDialog>
#include <QMessageBox>
#include "prisoner.h"
#include "viewprisonersincell.h"
#include "guard.h"


ManageCells::ManageCells(QWidget *parent)
    : QDialog(parent)
    ,ui(new Ui::ManageCells)
{
    ui->setupUi(this);
    loadCells();
    ui->cellsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ManageCells::~ManageCells()
{
    delete ui;
}

void ManageCells::loadCells() {
    cells=Cell::loadAllCells();
    refreshTable();
}

void ManageCells::refreshTable() {
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
        auto *item = new QTableWidgetItem(guard);

        if (guard == "Unassigned")
            item->setForeground(Qt::red); // highlight unassigned guards in red

        ui->cellsTableWidget->setItem(i, 4, item);
    }
}


void ManageCells::on_addCell_btn_clicked()
{
    QStringList blockOptions = { "A", "B" };
    bool ok;
    QString block = QInputDialog::getItem(this, "Select Block", "Choose block:", blockOptions, 0, false, &ok);
    if (!ok || block.isEmpty()) return;


    int cellNo = QInputDialog::getInt(this, "Cell Number", "Enter cell number:", 1, 1); //default is 1 and start from 1
    int capacity = QInputDialog::getInt(this, "Capacity", "Enter cell capacity:", 1, 1);

    // check if cell number in block already exists
    for (int i = 0; i < cells.size(); ++i) {
        const Cell &c = cells[i];
        if (c.getBlockName() == block && c.getCellNumber() == cellNo) {
            QMessageBox::warning(this, "Duplicate Cell", "Cell already exists in this block.");
            return;
        }
    }

    Cell newCell(block, cellNo, capacity);
    cells.append(newCell);
    refreshTable();
    Cell::saveCells(cells);
}


QList<Prisoner> ManageCells::loadActivePrisoners() {
    QList<Prisoner> prisonerList;
    QList<Prisoner> AllprisonerList = Prisoner::LoadAllPrisoners();

    for(const Prisoner &p : AllprisonerList){
        if (p.getIsActive())
            prisonerList.append(p);
    }
    return prisonerList;
}

void ManageCells::on_assignPrisoner_btn_clicked()
{
    int row = ui->cellsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a cell to assign a prisoner.");
        return;
    }

    //which cell to add to
    Cell &cell = cells[row];
    if (cell.isFull()) {
        QMessageBox::warning(this, "Cell Full", "This cell has reached its capacity.");
        return;
    }

    //load prisoners that can be assigned (currently active and unassigned)
    QList<Prisoner> prisonerList = loadActivePrisoners();
    QStringList unassignedIds;

    for (const Prisoner &p : prisonerList) {
        if (p.getAssignedBlock().isEmpty())
            unassignedIds << p.getID();
    }

    if (unassignedIds.isEmpty()) {
        QMessageBox::information(this, "No Unassigned Prisoners", "All active prisoners are already assigned.");
        return;
    }

    bool ok;
    QString selectedId = QInputDialog::getItem(this, "Select Prisoner", "Choose Prisoner ID:", unassignedIds, 0, false, &ok);
                                                                        //start from index 0, user cannot type own value
    if (!ok || selectedId.isEmpty()) return;

    QList<Prisoner> AllPrisonerList = Prisoner::LoadAllPrisoners();
    for (Prisoner &p : AllPrisonerList) {
        if (p.getID().compare(selectedId, Qt::CaseInsensitive) == 0) {
            p.setAssignedBlock(cell.getBlockName());
            p.setAssignedCellNumber(cell.getCellNumber());
            break;
        }
    }

    cell.assignPrisoner(selectedId);
    Prisoner::saveToFile(AllPrisonerList);
    Cell::saveCells(cells);
    refreshTable();

    QMessageBox::information(this, "Assigned", "Prisoner assigned successfully.");
}


void ManageCells::on_removePrisoner_btn_clicked()
{
    int row = ui->cellsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a cell first.");
        return;
    }

    Cell &cell = cells[row];
    QStringList assigned = cell.getAssignedPrisoners();

    if (assigned.isEmpty()) {
        QMessageBox::information(this, "Empty Cell", "No prisoners are assigned to this cell.");
        return;
    }

    bool ok;
    QString selectedId = QInputDialog::getItem(this, "Remove Prisoner", "Select prisoner to remove:", assigned, 0, false, &ok);
    if (!ok || selectedId.isEmpty()) return;

    QList<Prisoner> prisonerList = Prisoner::LoadAllPrisoners();
    for (Prisoner &p : prisonerList) {
        if (p.getID() == selectedId &&
            p.getAssignedBlock() == cell.getBlockName() &&
            p.getAssignedCellNumber() == cell.getCellNumber()) {
            p.setAssignedBlock("");
            p.setAssignedCellNumber(-1);

            break;
        }
    }
    cell.removePrisoner(selectedId);
    Prisoner::saveToFile(prisonerList);
    Cell::saveCells(cells);
    refreshTable();

    QMessageBox::information(this, "Removed", selectedId + " was removed from the cell.");
}



void ManageCells:: closeEvent(QCloseEvent *event){
    Cell::saveCells(cells);
    QDialog::closeEvent(event);
}

void ManageCells::on_exit_btn_clicked()
{
    this->close();
}


void ManageCells::on_removeCell_btn_clicked()
{
    int row = ui->cellsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a cell to remove.");
        return;
    }

    Cell cell = cells[row];
    QStringList prisoners = cell.getAssignedPrisoners();

    if (!prisoners.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Cell Not Empty",
                                      "This cell has prisoners assigned. Do you still want to remove it?\n"
                                      "They will become unassigned.",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
            return;

        // unassign prisoners from this cell
        QList<Prisoner> prisonerList = Prisoner::LoadAllPrisoners();

        for (Prisoner &p : prisonerList) {
            if (p.getAssignedBlock() == cell.getBlockName() && p.getAssignedCellNumber() == cell.getCellNumber()) {
                p.setAssignedBlock("");
                p.setAssignedCellNumber(-1);
            }
        }

        Prisoner::saveToFile(prisonerList);
    }

    if(cell.getAssignedGuardID()!=""){
        QList <Guard> guards = Guard::loadAllGuards();
        for(Guard &g : guards){
            if(g.getAssignedCell()==cell.getCellNumber() && g.getAssignedBlock()==cell.getBlockName()){
                g.setAssignedBlock("");
                g.setAssignedCell(-1);
                break;
            }
        }
        Guard::saveGuardsToFile(guards);
    }

    cells.removeAt(row);
    Cell::saveCells(cells);
    refreshTable();

    QMessageBox::information(this, "Removed", "Cell removed successfully.");
}


void ManageCells::on_viewPrisoners_btn_clicked()
{
    int row = ui->cellsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a cell first.");
        return;
    }

    Cell selectedCell = cells[row];
    ViewPrisonersInCell dialog(selectedCell, this);
    dialog.exec();
}
