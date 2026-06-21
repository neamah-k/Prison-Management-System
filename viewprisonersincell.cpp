#include "viewprisonersincell.h"
#include "ui_viewprisonersincell.h"
#include "cell.h"
#include "prisoner.h"
#include <QFile>
#include <QMessageBox>

ViewPrisonersInCell::ViewPrisonersInCell(Cell cell,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewPrisonersInCell)
    ,cell(cell)
{
    ui->setupUi(this);

    QString title = QString("Viewing Block %1 - Cell %2")
                        .arg(cell.getBlockName())
                        .arg(cell.getCellNumber());
    ui->titleLabel->setText(title);

    loadPrisonersOfCell();

}

ViewPrisonersInCell::~ViewPrisonersInCell()
{
    delete ui;
}

void ViewPrisonersInCell::loadPrisonersOfCell() {

    QList<Prisoner> cellPrisoners = cell.getAllAssignedPrisoner();

    ui->prisonersTableWidget->setRowCount(0);
    ui->prisonersTableWidget->setColumnCount(7);
    QStringList headers = { "ID","Name", "Crime","Crime Details", "Sentence Duration", "Age","Entry Date" };
    ui->prisonersTableWidget->setHorizontalHeaderLabels(headers);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // crime Details
    ui->prisonersTableWidget->setColumnWidth(3, 250);

    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->prisonersTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);

    for (int i = 0; i < cellPrisoners.size(); ++i) {
        const Prisoner &p = cellPrisoners[i];
        ui->prisonersTableWidget->insertRow(i);
        ui->prisonersTableWidget->setItem(i,0,new QTableWidgetItem(p.getID()));
        ui->prisonersTableWidget->setItem(i, 1, new QTableWidgetItem(p.getName()));
        ui->prisonersTableWidget->setItem(i, 2, new QTableWidgetItem(p.getCrime()));
        ui->prisonersTableWidget->setItem(i,3, new QTableWidgetItem(p.getCrimeDetails()));
        ui->prisonersTableWidget->setItem(i, 4, new QTableWidgetItem(p.getSentenceDuration()));
        ui->prisonersTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(p.getAge())));
        ui->prisonersTableWidget->setItem(i, 6, new QTableWidgetItem(p.getEntryDate().toString("yyyy-MM-dd")));

    }
}

void ViewPrisonersInCell::on_exit_btn_clicked()
{
    this->close();
}
