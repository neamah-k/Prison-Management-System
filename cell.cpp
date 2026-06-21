#include "cell.h"
#include "prisoner.h"
#include <QFile>

Cell::Cell() : cellNumber(0), capacity(0) {}

Cell::Cell(const QString &blockName, int cellNumber, int capacity)
    : blockName(blockName), cellNumber(cellNumber), capacity(capacity) {
    assignedGuardID = "";
}

QString Cell::getBlockName() const { return blockName; }
int Cell::getCellNumber() const { return cellNumber; }
int Cell::getCapacity() const { return capacity; }
QStringList Cell::getAssignedPrisoners() const { return assignedPrisoners; }
QString Cell::getAssignedGuardID() const { return assignedGuardID; }
QList<Prisoner> Cell:: getAllAssignedPrisoner() { return cellPrisoners; }


void Cell::setBlockName(const QString &b) { blockName = b; }
void Cell::setCellNumber(int n) { cellNumber = n; }
void Cell::setCapacity(int c) { capacity = c; }
void Cell::setAssignedPrisoners(const QStringList &list) { assignedPrisoners = list;}
void Cell::setAssignedGuardID(const QString &id) { assignedGuardID = id; }

bool Cell::isFull() const {
    return assignedPrisoners.size() >= capacity;
}

void Cell::assignPrisoner(const QString &id) {
    assignedPrisoners.append(id);
    LoadCellPrisoners();    //to update cellPrisoners
}

void Cell::removePrisoner(const QString &id) {
    assignedPrisoners.removeAll(id);
    LoadCellPrisoners();
}


QDataStream &operator<<(QDataStream &out, const Cell &cell) {
    out << cell.blockName << cell.cellNumber << cell.capacity << cell.assignedPrisoners << cell.assignedGuardID;
    return out;
}

QDataStream &operator>>(QDataStream &in, Cell &cell) {
    in >> cell.blockName >> cell.cellNumber >> cell.capacity >> cell.assignedPrisoners >> cell.assignedGuardID;
    cell.LoadCellPrisoners();
    return in;
}


void Cell:: LoadCellPrisoners(){
    cellPrisoners.clear();
    QList <Prisoner> allPrisoner = Prisoner::LoadAllPrisoners();

    for(const Prisoner &p: allPrisoner) //remove inactive prisoners
    {
        if(assignedPrisoners.contains(p.getID()) ){
            if(!p.getIsActive()) removePrisoner(p.getID());
            else cellPrisoners.append(p);
        }
    }

}


//static function
QList<Cell> Cell:: loadAllCells(){

    QList<Cell> cells;

    auto loadFromFile = [&](const QString &filename) {  //lambda function that takes file name as argument, [&] means it can access and modify variables
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) return;

        QDataStream in(&file);
        QList <Prisoner> allPrisoner = Prisoner::LoadAllPrisoners();

        while (!in.atEnd()) {
            Cell c;
            in >> c;

            for(const Prisoner &p: allPrisoner) //remove inactive prisoners
                if(c.getAssignedPrisoners().contains(p.getID())&& (!p.getIsActive()) )
                    c.removePrisoner(p.getID());

            cells.append(c);
        }
        file.close();
    };

    loadFromFile(blockAFile);
    loadFromFile(blockBFile);

    return cells;
}

//static
void Cell:: saveCells(QList<Cell> cells) {
    QList<Cell> blockACells, blockBCells;
    for (int i = 0; i < cells.size(); ++i) {
        const Cell &c = cells[i];
        if (c.getBlockName().compare("A", Qt::CaseInsensitive) == 0)
            blockACells.append(c);
        else if (c.getBlockName().compare("B", Qt::CaseInsensitive) == 0)
            blockBCells.append(c);
    }

    QFile fileA(blockAFile);
    if (fileA.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&fileA);
        for (const Cell &c : blockACells)
            out << c;
        fileA.close();
    }

    QFile fileB(blockBFile);
    if (fileB.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&fileB);
        for (const Cell &c : blockBCells)
            out << c;
        fileB.close();
    }
}

//static
QString Cell:: blockAFile ="blockA_cells.dat";
QString Cell:: blockBFile ="blockB_cells.dat";
