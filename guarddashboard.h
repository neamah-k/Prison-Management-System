#ifndef GUARDDASHBOARD_H
#define GUARDDASHBOARD_H

#include <QMainWindow>
#include "guard.h"

namespace Ui {
class GuardDashboard;
}

class GuardDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuardDashboard(const Guard &guard, QWidget *parent = nullptr);
    ~GuardDashboard();

private slots:


    void on_logout_btn_clicked();

    void on_requestTransfer_btn_clicked();

private:
    Ui::GuardDashboard *ui;
    Guard guard;
    //void viewAssignedCellDetails();
    void loadAssignedCellAndPrisoners();
};

#endif // GUARDDASHBOARD_H
