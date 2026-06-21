#ifndef MANAGEPRISONERS_H
#define MANAGEPRISONERS_H

#include <QDialog>
#include <QTableWidget>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include "prisoner.h"

namespace Ui {
class managePrisoners;
}

class managePrisoners : public QDialog
{
    Q_OBJECT

public:
    explicit managePrisoners(QWidget *parent = nullptr);
    ~managePrisoners();

private slots:
    void on_add_prisoner_btn_clicked();
    void on_search_btn_clicked();
    void on_edit_btn_clicked();

    void on_exit_btn_clicked();

    void on_remove_prisoner_btn_clicked();

    void on_view_cur_prisoners_btn_clicked();

    void on_view_all_prisoners_btn_clicked();

private:
    Ui::managePrisoners *ui;
    void loadPrisoners();  // Method to load prisoners from file
    void savePrisoners();
    QList<Prisoner> allPrisoners;
protected:
    void closeEvent(QCloseEvent *event) override;

};

#endif // MANAGEPRISONERS_H
