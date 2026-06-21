#ifndef WARDENAPPROVALDIALOG_H
#define WARDENAPPROVALDIALOG_H

#include <QDialog>
#include <QList>
#include "transferrequest.h"
#include "cell.h"

namespace Ui {
class WardenApprovalDialog;
}

class WardenApprovalDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WardenApprovalDialog(QWidget *parent = nullptr);
    ~WardenApprovalDialog();

    void loadPendingRequests();
    void loadAvailableCells();

private slots:
    void on_approve_btn_clicked();
    void on_reject_btn_clicked();

private:
    Ui::WardenApprovalDialog *ui;
    QList<TransferRequest> pendingRequests;
    QList<Cell> availableCells;
    void performTransfer(const TransferRequest &request);

    void updateRequestStatus(int requestIndex, const QString &status);
};

#endif // WARDENAPPROVALDIALOG_H

