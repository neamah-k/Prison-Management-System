#include "viewguards.h"
#include "ui_viewguards.h"
#include <QFile>
#include <QDataStream>

ViewGuards::ViewGuards(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewGuards)
{
    ui->setupUi(this);
    loadGuards();
    ui->guardsTableWidget->setColumnCount(4);

    ui->guardsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->guardsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->guardsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->guardsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    populateTable();
}

ViewGuards::~ViewGuards() {
    delete ui;
}

void ViewGuards::loadGuards() {
    guards.clear();
    guards = Guard::loadAllGuards();
}

void ViewGuards::populateTable() {
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

void ViewGuards::on_exit_btn_clicked()
{
    close();
}

