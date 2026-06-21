#include "guarddashboard.h"
#include "ui_guarddashboard.h"
#include "guard.h"
#include <QMessageBox>
#include <QFile>
#include "cell.h"
#include "prisoner.h"
#include "transferrequest.h"
#include "login.h"
#include <QInputDialog>


GuardDashboard::GuardDashboard(const Guard &g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GuardDashboard)
    , guard(g)
{
    ui->setupUi(this);
    loadAssignedCellAndPrisoners();
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);  //crime details

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    loadAssignedCellAndPrisoners();
}


GuardDashboard::~GuardDashboard()
{
    delete ui;
}

void GuardDashboard::loadAssignedCellAndPrisoners() {
    QList<Cell> allCells = Cell::loadAllCells();

    Cell matchedCell;
    bool found = false;
    for (const Cell &c : allCells) {
        if (c.getCellNumber() == guard.getAssignedCell() && c.getBlockName()==guard.getAssignedBlock()) {
            matchedCell = c;
            found = true;
            break;
        }
    }

    if (!found) {
        ui->titleLabel->setText("Assigned cell not found.");
        ui->prisonersTableWidget->setRowCount(0);
        return;
    }

    QString labelText = QString("Block %1 - Cell %2")
                            .arg(matchedCell.getBlockName())
                            .arg(matchedCell.getCellNumber());
    ui->titleLabel->setText(labelText);

    QList<Prisoner> prisonersInCell = matchedCell.getAllAssignedPrisoner();

    ui->prisonersTableWidget->setRowCount(0);
    ui->prisonersTableWidget->setColumnCount(7);
    QStringList headers = { "ID", "Name", "Crime", "Crime Details", "Sentence Duration", "Age", "Entry Date" };
    ui->prisonersTableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < prisonersInCell.size(); ++i) {
        const Prisoner &p = prisonersInCell[i];
        ui->prisonersTableWidget->insertRow(i);
        ui->prisonersTableWidget->setItem(i, 0, new QTableWidgetItem(p.getID()));
        ui->prisonersTableWidget->setItem(i, 1, new QTableWidgetItem(p.getName()));
        ui->prisonersTableWidget->setItem(i, 2, new QTableWidgetItem(p.getCrime()));
        ui->prisonersTableWidget->setItem(i, 3, new QTableWidgetItem(p.getCrimeDetails()));
        ui->prisonersTableWidget->setItem(i, 4, new QTableWidgetItem(p.getSentenceDuration()));
        ui->prisonersTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(p.getAge())));
        ui->prisonersTableWidget->setItem(i, 6, new QTableWidgetItem(p.getEntryDate().toString("yyyy-MM-dd")));

    }
}


void GuardDashboard::on_requestTransfer_btn_clicked() {
    int selectedRow = ui->prisonersTableWidget->currentRow();
    if (selectedRow == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a prisoner first.");
        return;
    }

    QString prisonerID = ui->prisonersTableWidget->item(selectedRow, 0)->text(); // Assuming ID is column 0
    QString reason = QInputDialog::getText(this, "Transfer Reason", "Enter reason for transfer:");

    if (reason.isEmpty()) {
        QMessageBox::warning(this, "Reason Required", "Transfer reason cannot be empty.");
        return;
    }

    TransferRequest req(prisonerID, guard.getAssignedBlock(), guard.getAssignedCell(), guard.getGuardID(), reason);

    QFile file("transfer_requests.dat");
    if (!file.open(QIODevice::Append)) {
        QMessageBox::warning(this, "Error", "Unable to store transfer request.");
        return;
    }

    QDataStream out(&file);
    out << req;
    file.close();

    QMessageBox::information(this, "Success", "Transfer request submitted for approval.");
}

void GuardDashboard::on_logout_btn_clicked()
{
    this->close();
    QMessageBox::information(this, "Message", "Logged out");
    Login *login = new Login();
    login->show();
}
