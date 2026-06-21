#include "viewprisoners.h"
#include "ui_viewprisoners.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTableWidgetItem>

ViewPrisoners::ViewPrisoners(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewPrisoners)
{
    ui->setupUi(this);
    ui->prisonersTableWidget->setColumnCount(7);

    //column sizes
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // crime Details
    ui->prisonersTableWidget->setColumnWidth(3, 250);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    loadPrisoners();
}

ViewPrisoners::~ViewPrisoners()
{
    delete ui;
}

void ViewPrisoners::loadPrisoners()
{
    allPrisoners.clear();
    allPrisoners = Prisoner::LoadAllPrisoners();

    ui->prisonersTableWidget->clearContents();
    ui->prisonersTableWidget->setRowCount(0);
    ui->prisonersTableWidget->setColumnCount(7);
    ui->prisonersTableWidget->setHorizontalHeaderLabels({"ID", "Name", "Crime", "Crime Details", "Sentence", "Age", "Entry Date"});

    for (const Prisoner &p : allPrisoners) {
        if (!p.getIsActive())
            continue;

        int row = ui->prisonersTableWidget->rowCount();
        ui->prisonersTableWidget->insertRow(row);
        ui->prisonersTableWidget->setItem(row, 0, new QTableWidgetItem(p.getID()));
        ui->prisonersTableWidget->setItem(row, 1, new QTableWidgetItem(p.getName()));
        ui->prisonersTableWidget->setItem(row, 2 , new QTableWidgetItem(p.getCrime()));
        ui->prisonersTableWidget->setItem(row, 3, new QTableWidgetItem(p.getCrimeDetails()));
        ui->prisonersTableWidget->setItem(row,4, new QTableWidgetItem(p.getSentenceDuration()));
        ui->prisonersTableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(p.getAge())));
        ui->prisonersTableWidget->setItem(row,6 , new QTableWidgetItem(p.getEntryDate().toString("yyyy-MM-dd")));
    }
}

void ViewPrisoners::on_search_btn_clicked()
{
    QString searchName = ui->searchLineEdit->text().trimmed();
    if (searchName.isEmpty()) {
        QMessageBox::information(this, "Search", "Please enter a name to search.");
        return;
    }

    bool found = false;
    for (int row = 0; row < ui->prisonersTableWidget->rowCount(); ++row) {
        QString name = ui->prisonersTableWidget->item(row, 1)->text();
        if (name.compare(searchName, Qt::CaseInsensitive) == 0) {
            ui->prisonersTableWidget->selectRow(row);
            found = true;
        } else {
            ui->prisonersTableWidget->setRowHidden(row, true);
        }
    }

    if (!found)
        QMessageBox::information(this, "Not Found", "No prisoner with that name found.");

}


void ViewPrisoners::on_view_cur_prisoners_btn_clicked()
{
    loadPrisoners();
}


void ViewPrisoners::on_view_all_prisoners_btn_clicked()
{
    ui->prisonersTableWidget->clearContents();
    ui->prisonersTableWidget->setRowCount(0);
    ui->prisonersTableWidget->setColumnCount(8);

    QStringList headers;
    headers <<"ID"<< "Name" << "Crime" <<"Crime Details"<< "Sentence" << "Age"<< "Enter"<<"Status";
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

void ViewPrisoners::on_exit_btn_clicked()
{
    this->close();
}

