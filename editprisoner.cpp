#include "editprisoner.h"
#include "ui_editprisoner.h"
#include "QMessageBox"

EditPrisoner::EditPrisoner(Prisoner prisoner,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditPrisoner)
    , prisoner(prisoner)
{
    ui->setupUi(this);

    ui->nameLineEdit->setText(prisoner.getName());
    ui->crimeLineEdit->setText(prisoner.getCrime());
    ui->sentenceLineEdit->setText(prisoner.getSentenceDuration());
    ui->ageLineEdit->setText(QString::number(prisoner.getAge()));
    ui->crimeDetailsTextEdit->setPlainText(prisoner.getCrimeDetails());
}

EditPrisoner::~EditPrisoner()
{
    delete ui;
}

void EditPrisoner::on_edit_btn_clicked()
{
    // validate
    if (ui->nameLineEdit->text().isEmpty() || ui->crimeLineEdit->text().isEmpty() || ui->sentenceLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Incomplete Data", "Please fill in all required fields.");
        return;
    }

    bool ageOk; //see if age is int and positive
    int age = ui->ageLineEdit->text().toInt(&ageOk);
    if (!ageOk || age <= 0) {
        QMessageBox::warning(this, "Invalid Age", "Please enter a valid age.");
        return;
    }

    prisoner.setName(ui->nameLineEdit->text());
    prisoner.setCrime(ui->crimeLineEdit->text());
    prisoner.setSentenceDuration(ui->sentenceLineEdit->text());
    prisoner.setAge(age);
    prisoner.setCrimeDetails(ui->crimeDetailsTextEdit->toPlainText());

    accept();
}

Prisoner EditPrisoner::getEditedPrisoner() const
{
    return prisoner;
}
