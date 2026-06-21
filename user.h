#ifndef USER_H
#define USER_H

#include <QString>
#include <QDataStream>

class User{
protected:
    QString username;
    QString password;
    QString role;

public:
    User();
    User(QString user, QString pass, QString role);

    QString getUsername() const;
    QString getUserPassword ()const ;
    QString getUserRole() const;

    friend QDataStream &operator<<(QDataStream &out, const User &user);
    friend QDataStream &operator>>(QDataStream &in, User &user);
};

void initializeUsers();
void saveUsers(const User users[], int userCount, const QString &usersStoreFile);
void loadUsers();
bool authenticateAdmin(const QString &username, const QString &password);
bool authenticateWarden(const QString &username, const QString &password);

#endif // USER_H
