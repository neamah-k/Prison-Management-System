#include "login.h"
#include "ui_login.h"
#include "QMessageBox"
#include "admindashboard.h"
#include "guarddashboard.h"
#include "wardendashboard.h"
#include "user.h"
#include "guard.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login:: handleAdminLogin(){
    QString username= ui->name_line->text();
    QString password=ui->pass_line->text();

    if(authenticateAdmin(username, password)){
        AdminDashboard *adminWin= new AdminDashboard;
        adminWin->show();
        this->hide();
    }
    else {
        QMessageBox::warning(this, "Error", "Not an admin!");
    }
}

void Login:: handleWardenLogin(){
    QString username= ui->name_line->text();
    QString password=ui->pass_line->text();

    if(authenticateWarden(username, password)){
        WardenDashboard *wardenWin= new WardenDashboard;
        wardenWin->show();
        this->hide();
    }
    else {
        QMessageBox::warning(this, "Error", "Not a Warden!");
    }
}

void Login:: handleGuardLogin(){
    QString username= ui->name_line->text();
    QString password=ui->pass_line->text();
    Guard g;

    if(authenticateGuard(username, password, g)){
        GuardDashboard *guardWindow = new GuardDashboard(g);
        guardWindow->show();
        this->hide();
    }
    else {
        QMessageBox::warning(this, "Error", "Not a guard");
    }
}


void Login::on_admin_login_btn_clicked()
{
    handleAdminLogin();
}


void Login::on_guard_login_btn_clicked()
{
    handleGuardLogin();
}


void Login::on_warden_login_btn_clicked()
{
    handleWardenLogin();
}

void Login::on_exit_btn_clicked(){
    this->close();
}

