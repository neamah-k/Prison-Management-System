#include "guard.h"
#include "user.h"
#include "QFile"

const QString GUARD_FILE = "guards.dat";

Guard::Guard() : User(), assignedCell(-1) {}


Guard::Guard(QString username, QString password, QString guardID,
            QString assignedBlock, int assignedCell, QString role)
    : User(username, password, role),
    guardID(guardID),
    assignedBlock(assignedBlock),
    assignedCell(assignedCell) {}

QString Guard::getGuardID() const { return guardID; }
QString Guard::getAssignedBlock() const { return assignedBlock; }
int Guard::getAssignedCell() const { return assignedCell; }

void Guard::setGuardID(const QString &id) { guardID = id; }
void Guard::setAssignedBlock(const QString &block) { assignedBlock = block; }
void Guard::setAssignedCell(int cell) { assignedCell = cell; }

QDataStream &operator<<(QDataStream &out, const Guard &g) {
    out << g.getUsername() << g.getUserPassword()
    << g.guardID << g.assignedBlock << g.assignedCell;
    return out;
}

QDataStream &operator>>(QDataStream &in, Guard &g) {
    QString user, pass;
    in >> user >> pass >> g.guardID >> g.assignedBlock >> g.assignedCell;
    g = Guard(user, pass, g.guardID, g.assignedBlock, g.assignedCell);
    return in;
}

bool authenticateGuard(const QString &username, const QString &password, Guard &matchedGuard) {
    QFile file("guards.dat");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);
    while (!in.atEnd()) {
        Guard g;
        in >> g;
        if (g.getUsername() == username && g.getUserPassword() == password) {
            matchedGuard = g;  //address of matched guard sent
            return true;
        }
    }

    file.close();
    return false;
}

QList<Guard> Guard:: loadAllGuards() {
    QList<Guard> guards;
    QFile file("guards.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        while (!in.atEnd()) {
            Guard g;
            in >> g;
            guards.append(g);
        }
        file.close();
    }
    return guards;
}

void Guard::saveGuardsToFile(QList<Guard> guards) {
    QFile file("guards.dat");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&file);
        for (const Guard &g : guards)
            out << g;
        file.close();
    }
}
