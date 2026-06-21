#include "wardenapprovaldialog.h"
#include "ui_wardenapprovaldialog.h"
#include "transferrequest.h"
#include "cell.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QInputDialog>
#include "prisoner.h"

WardenApprovalDialog::WardenApprovalDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WardenApprovalDialog)
{
    ui->setupUi(this);
    loadPendingRequests();
    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->requestsTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    loadAvailableCells();
}

WardenApprovalDialog::~WardenApprovalDialog()
{
    delete ui;
}

void WardenApprovalDialog::loadPendingRequests()
{
    pendingRequests.clear();
    QFile file("transfer_requests.dat");

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Unable to open transfer requests file.");
        return;
    }

    QDataStream in(&file);
    while (!in.atEnd()) {
        TransferRequest tr;
        in >> tr;
        if (tr.getStatus() == "Pending")
            pendingRequests.append(tr);

    }
    file.close();

    ui->requestsTableWidget->setRowCount(0);
    ui->requestsTableWidget->setColumnCount(6);
    QStringList headers = { "Prisoner ID", "From Block", "From Cell", "Requested By", "Reason", "Action" };
    ui->requestsTableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < pendingRequests.size(); i++) {
        const TransferRequest &tr = pendingRequests[i];
        ui->requestsTableWidget->insertRow(i);
        ui->requestsTableWidget->setItem(i, 0, new QTableWidgetItem(tr.getPrisonerID()));
        ui->requestsTableWidget->setItem(i, 1, new QTableWidgetItem(tr.getFromBlock()));
        ui->requestsTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(tr.getFromCell())));
        ui->requestsTableWidget->setItem(i, 3, new QTableWidgetItem(tr.getRequestedByGuardID()));
        ui->requestsTableWidget->setItem(i, 4, new QTableWidgetItem(tr.getReason()));

        QTableWidgetItem *actionItem = new QTableWidgetItem("Pending");
        actionItem->setBackground(Qt::green);
        ui->requestsTableWidget->setItem(i, 5, actionItem);
    }
}

void WardenApprovalDialog::loadAvailableCells()
{
    QList<Cell> AllCells = Cell::loadAllCells();
    availableCells.clear();

    for(const Cell &c : AllCells){
        if (!c.isFull())
            availableCells.append(c);
    }

}

void WardenApprovalDialog::on_approve_btn_clicked()
{
    int row = ui->requestsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a request to approve.");
        return;
    }

    TransferRequest &selectedRequest = pendingRequests[row];

    bool ok;
    QStringList cellOptions;
    for(const Cell &c : availableCells)
        if(!(c.getCellNumber()==selectedRequest.getFromCell() && c.getBlockName() == selectedRequest.getFromBlock()))
            cellOptions.append(QString("%1-%2").arg(c.getBlockName()).arg(c.getCellNumber()));


    QString selectedCell = QInputDialog::getItem(this, "Select Target Cell", "Choose a target cell:", cellOptions, 0, false, &ok);
    if (!ok || selectedCell.isEmpty()) return;

    QStringList parts = selectedCell.split("-");

    QString targetBlock = parts[0];
    int targetCellNumber = parts[1].toInt();

    selectedRequest.setTargetLocation(targetBlock, targetCellNumber);
    selectedRequest.setStatus("Approved");

    performTransfer(selectedRequest);

    updateRequestStatus(row, "Approved");

    QFile file("transfer_requests.dat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error", "Unable to save transfer request.");
        return;
    }

    QDataStream out(&file);
    for (const TransferRequest &tr : pendingRequests)
        out << tr;
    file.close();

    QMessageBox::information(this, "Approved", "Transfer request approved and target cell selected.");
}



void WardenApprovalDialog::on_reject_btn_clicked()
{
    int row = ui->requestsTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a request to reject.");
        return;
    }

    TransferRequest &selectedRequest = pendingRequests[row];
    selectedRequest.setStatus("Rejected");
    updateRequestStatus(row, "Rejected");

    QFile file("transfer_requests.dat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Error", "Unable to save transfer request.");
        return;
    }
    QDataStream out(&file);
    for (const TransferRequest &tr : pendingRequests)
        out << tr;
    file.close();

    QMessageBox::information(this, "Rejected", "Transfer request rejected.");
}

void WardenApprovalDialog::updateRequestStatus(int requestIndex, const QString &status)
{
    ui->requestsTableWidget->setItem(requestIndex, 5, new QTableWidgetItem(status));
}


void WardenApprovalDialog::performTransfer(const TransferRequest &request)
{
    QString prisonerID = request.getPrisonerID();
    QString targetBlock = request.getTargetBlock();
    int targetCellNumber = request.getTargetCell();

    QList<Cell> allCells = Cell::loadAllCells();

    // update cells
    for (Cell &c : allCells) {  //dont put const here
        if (c.getBlockName() == request.getFromBlock() && c.getCellNumber() == request.getFromCell())
            c.removePrisoner(prisonerID);

        if (c.getBlockName() == targetBlock && c.getCellNumber() == targetCellNumber)
            c.assignPrisoner(prisonerID);
    }


    QList<Prisoner> prisonerList = Prisoner::LoadAllPrisoners();

    for (Prisoner &p : prisonerList) {
        if (p.getID() == prisonerID) {
            p.setAssignedBlock(targetBlock);
            p.setAssignedCellNumber(targetCellNumber);
            break;
        }
    }

    // Save updated data
    Prisoner::saveToFile(prisonerList);
    Cell::saveCells(allCells);

}
