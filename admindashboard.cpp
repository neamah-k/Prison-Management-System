#include "admindashboard.h"
#include "ui_admindashboard.h"
#include "login.h"
#include "QMessageBox"
#include "managecells.h"
#include "manageprisoners.h"
#include "manageguards.h"

AdminDashboard::AdminDashboard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminDashboard)
{
    ui->setupUi(this);
}

AdminDashboard::~AdminDashboard()
{
    delete ui;
}

void AdminDashboard::on_manage_cells_btn_clicked()
{
    ManageCells ManageCellsDialog(this);
    ManageCellsDialog.exec();
}


void AdminDashboard::on_manage_prisoners_btn_clicked()
{
    managePrisoners ManagePrisonersDialog(this);
    ManagePrisonersDialog.exec();
}


void AdminDashboard::on_manage_guards_btn_clicked()
{
    ManageGuards ManageGuardsDialog(this);
    ManageGuardsDialog.exec();
}


void AdminDashboard::on_logout_btn_clicked()
{
    this->hide();
    QMessageBox::information(this, "Message", "Logged out");
    Login *login = new Login();
    login->show();
}
