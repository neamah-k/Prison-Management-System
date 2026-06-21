#ifndef CELL_H
#define CELL_H

#include <QString>
#include <QStringList>
#include <QDataStream>
#include "prisoner.h"

class Cell {
private:
    QString blockName;
    int cellNumber;
    int capacity;
    QStringList assignedPrisoners; // Store prisoner names or IDs
    QString assignedGuardID;
    QList <Prisoner> cellPrisoners; //not serialised

    void LoadCellPrisoners();

public:
    Cell();
    Cell(const QString &blockName, int cellNumber, int capacity);

    QString getBlockName() const;
    int getCellNumber() const;
    int getCapacity() const;
    QStringList getAssignedPrisoners() const;
    QList<Prisoner> getAllAssignedPrisoner() ;
    QString getAssignedGuardID() const;

    void setBlockName(const QString &blockName);
    void setCellNumber(int cellNumber);
    void setCapacity(int capacity);
    void setAssignedPrisoners(const QStringList &list);
    void setAssignedGuardID(const QString &id);

    bool isFull() const;
    void assignPrisoner(const QString &prisonerID);
    void removePrisoner(const QString &prisonerID);
    static QList<Cell> loadAllCells();
    static void saveCells(QList<Cell> cells);

    friend QDataStream &operator<<(QDataStream &out, const Cell &cell);
    friend QDataStream &operator>>(QDataStream &in, Cell &cell);

    static QString blockAFile;
    static QString blockBFile;
};

#endif // CELL_H

