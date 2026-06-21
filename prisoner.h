#ifndef PRISONER_H
#define PRISONER_H

#include <QString>
#include <QDataStream>//to convert object to binary
#include <QDate>

class Prisoner
{
public:
    Prisoner();

    Prisoner(    const QString &name,
             const QString &crime,
             const QString &sentenceDuration,
             const int age,
             const QDate &entryDate,
             const QString &id,
             const QString &crimeDetails,
             const QString &block="",
             const int cell=-1,
             const bool active=true);
    QString getName() const;
    QString getCrime() const;
    QString getSentenceDuration() const;
    QDate getEntryDate() const;
    int getAge() const;
    QString getAssignedBlock() const;
    int getAssignedCellNumber() const;
    bool getIsActive() const;
    QString getID() const;
    QString getCrimeDetails() const;

    void setCrimeDetails(const QString &crimeDetails);
    void setName(const QString &name);
    void setCrime(const QString &crime);
    void setSentenceDuration(const QString &sentenceDuration);
    void setAge(const int age);
    void setAssignedCellNumber(const int cell);
    void setAssignedBlock(const QString &block);
    void setIsActive(const bool value);
    static QString generateUniqueID();

public:
    static QList <Prisoner> LoadAllPrisoners();
    static void saveToFile(const QList<Prisoner> allPrisoners);

    // Stream operators
    friend QDataStream& operator<<(QDataStream &out, const Prisoner &prisoner);
    friend QDataStream& operator>>(QDataStream &in, Prisoner &prisoner);

private:
    QString name;
    QString crime;
    QString sentenceDuration;
    int age;
    QString assignedBlock;
    int assignedCellNumber;
    bool isActive;
    QDate entryDate;
    QString ID;
    QString crimeDetails;

};

#endif // PRISONER_H
