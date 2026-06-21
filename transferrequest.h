// transferrequest.h
#ifndef TRANSFERREQUEST_H
#define TRANSFERREQUEST_H

#include <QString>
#include <QDataStream>

class TransferRequest {
    QString prisonerID;
    QString fromBlock;
    int fromCell;
    QString requestedByGuardID;
    QString reason;
    QString status;      // "Pending", "Approved", "Rejected", "Completed"
    QString targetBlock; // used if approved
    int targetCell;      // used if approved

public:
    TransferRequest();
    TransferRequest(const QString &prisonerID,
                    const QString &fromBlock,
                    int fromCell,
                    const QString &guardID,
                    const QString &reason);

    // Getters
    QString getPrisonerID() const;
    QString getFromBlock() const;
    int getFromCell() const;
    QString getRequestedByGuardID() const;
    QString getReason() const;
    QString getStatus() const;
    QString getTargetBlock() const;
    int getTargetCell() const;

    // Setters
    void setStatus(const QString &status);
    void setTargetLocation(const QString &block, int cell);

    // Serialization
    friend QDataStream &operator<<(QDataStream &out, const TransferRequest &tr);
    friend QDataStream &operator>>(QDataStream &in, TransferRequest &tr);
};

#endif // TRANSFERREQUEST_H
