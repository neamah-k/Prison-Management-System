// transferrequest.cpp
#include "transferrequest.h"

TransferRequest::TransferRequest()
    : fromCell(0), targetCell(0), status("Pending") {}

TransferRequest::TransferRequest(const QString &prisonerID,
                                 const QString &fromBlock,
                                 int fromCell,
                                 const QString &guardID,
                                 const QString &reason)
    : prisonerID(prisonerID),
    fromBlock(fromBlock),
    fromCell(fromCell),
    requestedByGuardID(guardID),
    reason(reason),
    status("Pending"),
    targetCell(0) {}

QString TransferRequest::getPrisonerID() const { return prisonerID; }
QString TransferRequest::getFromBlock() const { return fromBlock; }
int TransferRequest::getFromCell() const { return fromCell; }
QString TransferRequest::getRequestedByGuardID() const { return requestedByGuardID; }
QString TransferRequest::getReason() const { return reason; }
QString TransferRequest::getStatus() const { return status; }
QString TransferRequest::getTargetBlock() const { return targetBlock; }
int TransferRequest::getTargetCell() const { return targetCell; }

void TransferRequest::setStatus(const QString &s) { status = s; }
void TransferRequest::setTargetLocation(const QString &block, int cell) {
    targetBlock = block;
    targetCell = cell;
}

QDataStream &operator<<(QDataStream &out, const TransferRequest &tr) {
    out << tr.prisonerID << tr.fromBlock << tr.fromCell
        << tr.requestedByGuardID << tr.reason << tr.status
        << tr.targetBlock << tr.targetCell;
    return out;
}

QDataStream &operator>>(QDataStream &in, TransferRequest &tr) {
    in >> tr.prisonerID >> tr.fromBlock >> tr.fromCell
        >> tr.requestedByGuardID >> tr.reason >> tr.status
        >> tr.targetBlock >> tr.targetCell;
    return in;
}

