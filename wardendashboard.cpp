#include "wardendashboard.h"
#include "ui_wardendashboard.h"
#include "wardenapprovaldialog.h"
#include <QMessageBox>
#include "login.h"
#include "viewguards.h"
#include "viewprisoners.h"
#include "viewcells.h"

WardenDashboard::WardenDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WardenDashboard)
{
    ui->setupUi(this);
}

WardenDashboard::~WardenDashboard()
{
    delete ui;
}

void WardenDashboard::on_view_transfer_requests_btn_clicked()
{
    WardenApprovalDialog *wardenapprovaldDialog = new WardenApprovalDialog(this);
    wardenapprovaldDialog->exec();  // blocks code
}


void WardenDashboard::on_view_prisoners_btn_clicked()
{
    ViewPrisoners *viewprisonerssWindow = new ViewPrisoners();
    viewprisonerssWindow->show();
}


void WardenDashboard::on_view_cells_btn_clicked()
{
    ViewCells *viewcellsWindow = new ViewCells();
    viewcellsWindow->show();
}


void WardenDashboard::on_view_guards_btn_clicked()
{
    ViewGuards *viewguardsWindow = new ViewGuards();
    viewguardsWindow->show();
}


void WardenDashboard::on_logout_btn_clicked()
{
    this->close();
    QMessageBox::information(this, "Message", "Logged out");
    Login *login = new Login();
    login->show();
}
