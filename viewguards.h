#ifndef VIEWGUARDS_H
#define VIEWGUARDS_H

#include <QDialog>
#include "guard.h"

namespace Ui {
class ViewGuards;
}

class ViewGuards : public QDialog {
    Q_OBJECT

public:
    explicit ViewGuards(QWidget *parent = nullptr);
    ~ViewGuards();

private slots:


    void on_exit_btn_clicked();

private:
    Ui::ViewGuards *ui;
    QList<Guard> guards;

    void loadGuards();
    void populateTable();
};

#endif // VIEWGUARDS_H
