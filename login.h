#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
private slots:

    void handleAdminLogin();
    void handleWardenLogin();
    void handleGuardLogin();


    void on_admin_login_btn_clicked();

    void on_guard_login_btn_clicked();

    void on_warden_login_btn_clicked();

    void on_exit_btn_clicked();

private:
    Ui::Login *ui;
};


#endif
