// guard.h
#ifndef GUARD_H
#define GUARD_H

#include "user.h"
#include <QString>
#include <QDataStream>

class Guard : public User {
private:
    QString guardID;
    QString assignedBlock;
    int assignedCell;

public:
    Guard();
    Guard(QString username, QString password, QString guardID,
          QString assignedBlock="", int assignedCell=-1, QString role="Guard");

    QString getGuardID() const;
    QString getAssignedBlock() const;
    int getAssignedCell() const;

    void setGuardID(const QString &id);
    void setAssignedBlock(const QString &block);
    void setAssignedCell(int cell);

    static QList<Guard> loadAllGuards();
    static void saveGuardsToFile(QList<Guard> guards);

    friend QDataStream &operator<<(QDataStream &out, const Guard &g);
    friend QDataStream &operator>>(QDataStream &in, Guard &g);
};

void saveGuard(const Guard users[], int userCount, const QString &fileName);
void initializeGuards();
bool authenticateGuard(const QString &username, const QString &password, Guard &matchedGuard);
#endif // GUARD_H
