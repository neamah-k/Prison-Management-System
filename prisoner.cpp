#include "prisoner.h"
#include <QString>
#include <QDataStream>
#include <QDate>
#include <QFile>
#include <QMessageBox>

Prisoner::Prisoner() {}

Prisoner::Prisoner(const QString &name,
                   const QString &crime,
                   const QString &sentenceDuration,
                   const int age,
                   const QDate &entryDate,
                   const QString &id,
                   const QString &crimeDetails,
                   const QString &block,
                   const int cell,
                   const bool active)

    :name(name),
    crime(crime),
    sentenceDuration(sentenceDuration),
    age(age),
    entryDate(entryDate),
    ID(id),
    crimeDetails(crimeDetails),
    assignedBlock(block),
    assignedCellNumber(cell),
    isActive(active)
{}

QString Prisoner::getName() const { return name; }
QString Prisoner::getCrime() const { return crime; }
QString Prisoner::getSentenceDuration() const { return sentenceDuration; }
int Prisoner::getAge() const { return age; }
QString Prisoner::getAssignedBlock() const { return assignedBlock; }
int Prisoner::getAssignedCellNumber() const { return assignedCellNumber; }
bool Prisoner::getIsActive()const{return isActive;}
QDate Prisoner::getEntryDate()const{return entryDate;}
QString Prisoner::getID()const{return ID;}
QString Prisoner::getCrimeDetails() const{return crimeDetails;}

void Prisoner::setName(const QString &name) { this->name = name; }
void Prisoner::setCrime(const QString &crime) { this->crime = crime; }
void Prisoner::setSentenceDuration(const QString &sentenceDuration) { this->sentenceDuration = sentenceDuration; }
void Prisoner::setAge(const int age) { this->age = age; }
void Prisoner::setAssignedBlock(const QString &block) { assignedBlock = block; }
void Prisoner::setAssignedCellNumber(int num) { assignedCellNumber = num; }
void Prisoner::setIsActive(bool currentPrisoner) { isActive = currentPrisoner; }
void Prisoner::setCrimeDetails(const QString &crimeDetails){this->crimeDetails=crimeDetails;}

// Serialization (writing)
QDataStream& operator<<(QDataStream &out, const Prisoner &p)
{
    out << p.getName()
    << p.getCrime()
    << p.getSentenceDuration()
    << p.getAge()
    << p.getEntryDate()
    << p.getID()
    << p.getCrimeDetails()
    << p.getAssignedBlock()
    << p.getAssignedCellNumber()
    << p.getIsActive();
    return out;
}

// Deserialization (reading)
QDataStream& operator>>(QDataStream &in, Prisoner &p)
{
    QString name, crime, sentenceDuration, id, crimeDetails, block;
    int age, cell;
    QDate entryDate;
    bool active;

    in >> name >> crime >> sentenceDuration >> age >> entryDate >> id >> crimeDetails >> block >> cell >> active;
    p = Prisoner(name, crime, sentenceDuration, age, entryDate, id, crimeDetails, block, cell, active);
    return in;
}

QString Prisoner::generateUniqueID(){
    QFile file("latest.id");
    int last_id=0;
    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        in>>last_id;
        file.close();
    }

    last_id++;
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out<<last_id;
        file.close();
    }

    QString uniqueID= "PR0" + QString::number(last_id);
    return uniqueID;
}


QList <Prisoner> Prisoner:: LoadAllPrisoners(){
    QList<Prisoner> allPrisoners;
    QFile file("prisoners.dat");
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "Error", "Unable to open prisoners file");
        allPrisoners.clear();
        return allPrisoners;
    }

    QDataStream in(&file);
    while (!in.atEnd()) {
        Prisoner p;
        in >> p;
        allPrisoners.append(p);
    }
    file.close();

    return allPrisoners;
}

void Prisoner :: saveToFile(const QList<Prisoner> allPrisoners) {
    QFile file("prisoners.dat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(nullptr, "Error", "Unable to open prisoners file");
        return;
    }

    QDataStream out(&file);

    for (int i = 0; i < allPrisoners.size(); ++i) {
        const Prisoner &p = allPrisoners[i];
        out<<p;
    }

    file.close();
}

