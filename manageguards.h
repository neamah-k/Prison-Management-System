// manageguards.h
#ifndef MANAGEGUARDS_H
#define MANAGEGUARDS_H

#include <QDialog>
#include "guard.h"
#include "cell.h"

namespace Ui {
class ManageGuards;
}

class ManageGuards : public QDialog {
    Q_OBJECT

public:
    explicit ManageGuards(QWidget *parent = nullptr);
    ~ManageGuards();

private slots:
    void on_addGuard_btn_clicked();
    void on_assignCell_btn_clicked();
    void on_removeGuard_btn_clicked();
    void on_exit_btn_clicked();

private:
    Ui::ManageGuards *ui;
    QList<Guard> guards;

    void loadGuards();
    void saveGuards();
    void refreshTable();
    void LoadCellList();
    bool saveCellsToBlock(const QString &block, const QList<Cell> &cells);
    QList<Cell> loadCellsFromBlock(const QString &block);
};

#endif // MANAGEGUARDS_H
