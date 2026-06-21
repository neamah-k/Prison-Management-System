#ifndef VIEWPRISONERS_H
#define VIEWPRISONERS_H

#include <QDialog>
#include "prisoner.h"

namespace Ui {
class ViewPrisoners;
}

class ViewPrisoners : public QDialog
{
    Q_OBJECT

public:
    explicit ViewPrisoners(QWidget *parent = nullptr);
    ~ViewPrisoners();

private slots:
    void on_search_btn_clicked();

void on_view_cur_prisoners_btn_clicked();

void on_view_all_prisoners_btn_clicked();

    void on_exit_btn_clicked();

private:
    void loadPrisoners();

    Ui::ViewPrisoners *ui;
    QVector<Prisoner> allPrisoners;
};

#endif // VIEWPRISONERS_H




