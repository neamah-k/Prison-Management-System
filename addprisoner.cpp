#include "addprisoner.h"
#include "ui_addprisoner.h"
#include <QMessageBox>
#include "prisoner.h"
#include <QFile>
#include <QDataStream>

AddPrisoner::AddPrisoner(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddPrisoner)
{
    ui->setupUi(this);
}

AddPrisoner::~AddPrisoner()
{
    delete ui;
}

void AddPrisoner::on_submit_btn_clicked()
{
    QString name = ui->nameLineEdit->text();
    QString crime = ui->crimeLineEdit->text();
    QString crimeDetails=ui->crimeDetailsLineEdit->text();
    QString sentenceDuration = ui->sentenceLineEdit->text();
    bool ok;
    QDate entry= ui->entryDateEdit->date();
    int age = ui->ageLineEdit->text().toInt(&ok);   //convert string to int

    if(name.isEmpty() || crime.isEmpty() || sentenceDuration.isEmpty() || !ok)
    {
        QMessageBox::warning(this, "Incomplete Data", "Please fill in all fields.");
        return;
    }
    Prisoner newPrisoner(name, crime, sentenceDuration, age,entry,Prisoner::generateUniqueID(),crimeDetails);

    // save the prisoner to the file
    QFile file("prisoners.dat");
    if(!file.open(QIODevice::Append))
    {
        QMessageBox::warning(this, "Error", "Could not open file to save data.");
        return;
    }

    QDataStream out(&file);
    out << newPrisoner;  // serialize prisoner

    file.close();
    accept();  //to trigger execute so dialog ends

    QMessageBox::information(this, "Success", "Prisoner added successfully!");
}
