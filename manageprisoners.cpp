#include "manageprisoners.h"
#include "ui_manageprisoners.h"
#include "addprisoner.h"
#include "prisoner.h"
#include "editprisoner.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTableWidgetItem>


managePrisoners::managePrisoners(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::managePrisoners)
{
    ui->setupUi(this);
    ui->prisonersTableWidget->setColumnCount(7); // no. of columns


    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->prisonersTableWidget->setColumnWidth(3, 250);  // max width

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

    loadPrisoners();
}

managePrisoners::~managePrisoners()
{
    delete ui;
}

void managePrisoners::on_add_prisoner_btn_clicked()
{
    AddPrisoner addPrisonerDialog(this);
    if (addPrisonerDialog.exec() == QDialog::Accepted)
        loadPrisoners();

}

void managePrisoners::loadPrisoners()
{
    allPrisoners.clear();

    allPrisoners = Prisoner::LoadAllPrisoners();

    ui->prisonersTableWidget->clearContents();
    ui->prisonersTableWidget->setRowCount(0); // clear old rows
    ui->prisonersTableWidget->setColumnCount(7);  //sSet number of columns

    QStringList headers;
    headers <<"ID"<< "Name" << "Crime" <<"Crime Details"<< "Sentence" << "Age"<<"Entry date";
    ui->prisonersTableWidget->setHorizontalHeaderLabels(headers);


    for (int i = 0; i < allPrisoners.size(); ++i) {
        const Prisoner &p = allPrisoners[i];
        if (!p.getIsActive())
            continue;

        int row = ui->prisonersTableWidget->rowCount();
        ui->prisonersTableWidget->insertRow(row);

        ui->prisonersTableWidget->setItem(row,0,new QTableWidgetItem(p.getID()));
        ui->prisonersTableWidget->setItem(row, 1, new QTableWidgetItem(p.getName()));
        ui->prisonersTableWidget->setItem(row, 2, new QTableWidgetItem(p.getCrime()));
        ui->prisonersTableWidget->setItem(row,3,new QTableWidgetItem(p.getCrimeDetails()));
        ui->prisonersTableWidget->setItem(row, 4, new QTableWidgetItem(p.getSentenceDuration()));
        ui->prisonersTableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(p.getAge())));
        ui->prisonersTableWidget->setItem(row, 6, new QTableWidgetItem(p.getEntryDate().toString("yyyy-MM-dd")));

    }
}

void managePrisoners::on_search_btn_clicked()
{
    for (int row = 0; row < ui->prisonersTableWidget->rowCount(); ++row)
        ui->prisonersTableWidget->setRowHidden(row, false);

    QString searchName = ui->searchLineEdit->text().trimmed();


    if (searchName.isEmpty()) {
        QMessageBox::information(this, "Search", "Please enter a name to search.");
        return;
    }

    bool found = false;
    for (int row = 0; row < ui->prisonersTableWidget->rowCount(); ++row) {
        QString ID = ui->prisonersTableWidget->item(row, 0)->text();
        if (ID.compare(searchName) == 0) {
            ui->prisonersTableWidget->selectRow(row);
            found = true;
        }
        else
            ui->prisonersTableWidget->setRowHidden(row, true);

    }

    if (!found)
        QMessageBox::information(this, "Not Found", "No prisoner with that name found.");
}


void managePrisoners::on_edit_btn_clicked()
{
    int row = ui->prisonersTableWidget->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a prisoner to edit.");
        return;
    }

    QString id = ui->prisonersTableWidget->item(row, 0)->text();

    Prisoner* prisonerToEdit = nullptr;
    allPrisoners = Prisoner::LoadAllPrisoners();
    for (Prisoner &p : allPrisoners) {
        if (p.getID() == id) {
            prisonerToEdit = &p;
            break;
        }
    }

    if (!prisonerToEdit) {
        QMessageBox::warning(this, "Error", "Selected prisoner not found in records.");
        return;
    }

    EditPrisoner editDialog(*prisonerToEdit, this);
    if (editDialog.exec() == QDialog::Accepted) {
        Prisoner editedPrisoner = editDialog.getEditedPrisoner();
        *prisonerToEdit = editedPrisoner;

        Prisoner::saveToFile(allPrisoners);
        loadPrisoners();

        QMessageBox::information(this, "Success", "Prisoner information updated successfully.");
    }
}

//overriding this function
void managePrisoners::closeEvent(QCloseEvent *event) {
    Prisoner::saveToFile(allPrisoners);
    QDialog::closeEvent(event);
}


void managePrisoners::on_exit_btn_clicked()
{
    Prisoner::saveToFile(allPrisoners);
    this->close();
}


void managePrisoners::on_remove_prisoner_btn_clicked()
{
    int row = ui->prisonersTableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a prisoner to remove.");
        return;
    }

    QString id = ui->prisonersTableWidget->item(row, 0)->text();

    bool found = false;
    allPrisoners = Prisoner::LoadAllPrisoners();

    for (Prisoner &p : allPrisoners) {
        if (p.getID() == id) {
            p.setIsActive(false);
            p.setAssignedBlock("");
            p.setAssignedCellNumber(-1);
            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Error", "Prisoner not found in records.");
        return;
    }

    Prisoner::saveToFile(allPrisoners);
    loadPrisoners();

    QMessageBox::information(this, "Success", "Prisoner has been released.");
}


void managePrisoners::on_view_cur_prisoners_btn_clicked()
{
    loadPrisoners();
}


void managePrisoners::on_view_all_prisoners_btn_clicked()
{
    ui->prisonersTableWidget->clearContents();
    ui->prisonersTableWidget->setRowCount(0);
    ui->prisonersTableWidget->setColumnCount(8);

    QStringList headers;
    headers <<"ID"<< "Name" << "Crime" <<"Crime Details"<< "Sentence" << "Age"<< "Enter"<<"Status"; //status tells whether prisoner is released or not
    ui->prisonersTableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < allPrisoners.size(); ++i) {
        const Prisoner &p = allPrisoners[i];

        int row = ui->prisonersTableWidget->rowCount();
        ui->prisonersTableWidget->insertRow(row);
        ui->prisonersTableWidget->setItem(row, 0,new QTableWidgetItem(p.getID()));
        ui->prisonersTableWidget->setItem(row, 1, new QTableWidgetItem(p.getName()));
        ui->prisonersTableWidget->setItem(row, 2, new QTableWidgetItem(p.getCrime()));
        ui->prisonersTableWidget->setItem(row, 3, new QTableWidgetItem(p.getCrimeDetails()));
        ui->prisonersTableWidget->setItem(row, 4, new QTableWidgetItem(p.getSentenceDuration()));
        ui->prisonersTableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(p.getAge())));
        ui->prisonersTableWidget->setItem(row, 6, new QTableWidgetItem(p.getEntryDate().toString("yyyy-MM-dd")));
        QString status = p.getIsActive() ? "Current" : "Released";
        ui->prisonersTableWidget->setItem(row, 7, new QTableWidgetItem(status));

    }
}
