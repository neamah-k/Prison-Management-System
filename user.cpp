#include "user.h"
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QDebug>

const QString ADMIN_FILE = "admins.dat";
const QString WARDEN_FILE = "wardens.dat";

User::User(QString user, QString pass, QString role) {
    this->username = user;
    this->password = pass;
    this->role = role;
}

User::User() : username(""), password(""), role("") {}

QString User::getUserPassword() const { return password; }
QString User::getUsername() const { return username; }
QString User::getUserRole() const { return role; }

QDataStream &operator<<(QDataStream &out, const User &user) {
    out << user.username << user.password << user.role;
    return out;
}
QDataStream &operator>>(QDataStream &in, User &user) {
    in >> user.username >> user.password >> user.role;
    return in;
}

void saveUsers(const User users[], int userCount, const QString &fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&file);
        for (int i = 0; i < userCount; ++i) {
            out << users[i];
        }
        file.close();
    }
}

void initializeUsers() {
    if (QFile::exists(ADMIN_FILE) && QFile::exists(WARDEN_FILE)) {
        return;
    }
    User admin[] = { User("Admin1", "admin123", "Admin") };
    User warden[] = { User("Warden1", "warden123", "Warden") };

    saveUsers(admin, 1, ADMIN_FILE);
    saveUsers(warden, 1, WARDEN_FILE);
}

bool authenticateAdmin(const QString &username, const QString &password) {
    QFile file("admins.dat");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);
    while (!in.atEnd()) {
        User u;
        in >> u;
        if (u.getUsername() == username && u.getUserPassword() == password)
            return true;
    }
    return false;
}

bool authenticateWarden(const QString &username, const QString &password) {
    QFile file("wardens.dat");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);
    while (!in.atEnd()) {
        User u;
        in >> u;
        if (u.getUsername() == username && u.getUserPassword() == password)
            return true;
    }
    return false;
}


