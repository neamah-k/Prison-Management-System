#ifndef WARDENDASHBOARD_H
#define WARDENDASHBOARD_H

#include <QMainWindow>

namespace Ui {
class WardenDashboard;
}

class WardenDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit WardenDashboard(QWidget *parent = nullptr);
    ~WardenDashboard();

private slots:
    void on_view_transfer_requests_btn_clicked();

    void on_logout_btn_clicked();

    void on_view_prisoners_btn_clicked();

    void on_view_cells_btn_clicked();

    void on_view_guards_btn_clicked();

private:
    Ui::WardenDashboard *ui;
};

#endif // WARDENDASHBOARD_H
