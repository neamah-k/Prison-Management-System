#include "manageguards.h"
#include "ui_manageguards.h"
#include <QFile>
#include <QDataStream>
#include <QInputDialog>
#include <QMessageBox>
#include "guard.h"
#include "cell.h"

ManageGuards::ManageGuards(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageGuards)
{
    ui->setupUi(this);
    guards = Guard::loadAllGuards();
    refreshTable();
    ui->guardsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

ManageGuards::~ManageGuards() {
    delete ui;
}

void ManageGuards::refreshTable() {
    ui->guardsTableWidget->setRowCount(0);
    ui->guardsTableWidget->setColumnCount(4);
    QStringList headers = { "Name", "ID", "Assigned Block", "Assigned Cell" };
    ui->guardsTableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < guards.size(); ++i) {
        const Guard &g = guards[i];
        ui->guardsTableWidget->insertRow(i);
        ui->guardsTableWidget->setItem(i, 0, new QTableWidgetItem(g.getUsername()));
        ui->guardsTableWidget->setItem(i, 1, new QTableWidgetItem(g.getGuardID()));
        ui->guardsTableWidget->setItem(i, 2, new QTableWidgetItem(g.getAssignedBlock()));
        ui->guardsTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(g.getAssignedCell())));
    }
}

void ManageGuards::on_addGuard_btn_clicked() {
    QString Username = QInputDialog::getText(this, "Add Guard", "Enter Username:");
    QString password = QInputDialog::getText(this, "Add Guard", "Set password:");
    QString id = QInputDialog::getText(this, "Add Guard", "Enter ID:");

    if (Username.isEmpty() || password.isEmpty() || id.isEmpty()) return;

    for (const Guard &g : guards) {
        if (g.getGuardID() == id) {
            QMessageBox::warning(this, "Duplicate", "Guard ID already exists.");
            return;
        }
    }

    Guard newGuard(Username,password, id);
    guards.append(newGuard);
    Guard::saveGuardsToFile(guards);
    refreshTable();
}

void ManageGuards::on_assignCell_btn_clicked() {
    int row = ui->guardsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Select a guard to assign.");
        return;
    }

    QStringList blocks = { "A", "B" };
    bool ok;
    QString block = QInputDialog::getItem(this, "Assign Block", "Choose block:", blocks, 0, false, &ok);    //default is at 0 index(A), false = user cannot type,
    if (!ok || block.isEmpty()) return;

    int cellNum = QInputDialog::getInt(this, "Assign Cell", "Enter cell number:", 1, 1);    //1 is default value and also minimum input

    QList<Cell> cells = Cell::loadAllCells();

    bool found = false, alreadyAssigned = false;
    for (Cell &c : cells) {
        if (c.getCellNumber() == cellNum &&c.getBlockName()==block) {
            found = true;
            if (!c.getAssignedGuardID().isEmpty()) {
                alreadyAssigned = true;
                break;
            }
            c.setAssignedGuardID(guards[row].getGuardID());
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Not Found", "No such cell in block.");
        return;
    }

    if (alreadyAssigned) {
        QMessageBox::warning(this, "Already Assigned", "This cell already has a guard assigned.");
        return;
    }

    Cell::saveCells(cells);

    guards[row].setAssignedBlock(block);
    guards[row].setAssignedCell(cellNum);
    Guard::saveGuardsToFile(guards);
    refreshTable();

    QMessageBox::information(this, "Assigned", "Guard assigned to cell successfully.");
}



void ManageGuards::on_removeGuard_btn_clicked() {
    int row = ui->guardsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Select a guard to remove.");
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Confirm Removal",
        "Do you really want to remove the selected guard?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) return;

    QString block = guards[row].getAssignedBlock();
    int cellNum = guards[row].getAssignedCell();
    QString guardID = guards[row].getGuardID();

    if (!block.isEmpty() && cellNum != 0) {
        QList<Cell> cells = Cell::loadAllCells();

        bool modified = false;
        for (Cell &c : cells) {
            if (c.getCellNumber() == cellNum && c.getAssignedGuardID() == guardID) {
                c.setAssignedGuardID("");
                modified = true;
                break;
            }
        }

        if (modified)
            Cell::saveCells(cells);
    }

    guards.removeAt(row);
    Guard::saveGuardsToFile(guards);
    refreshTable();
}

void ManageGuards::on_exit_btn_clicked() {
    close();
}
