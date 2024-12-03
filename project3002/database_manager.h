#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
public:
    static DatabaseManager& instance();
    bool initDatabase();
    QSqlDatabase getDatabase();

private:
    DatabaseManager() = default;
    bool executeSQL(const QString& sql);
    bool createTables();
    
    QSqlDatabase m_db;
};

#endif