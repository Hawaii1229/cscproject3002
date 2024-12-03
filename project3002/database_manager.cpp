#include "database_manager.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::initDatabase() {
    // 设置数据库文件路径
    QString dbPath = QDir::currentPath() + "/database/waimai.db";
    QDir().mkpath(QFileInfo(dbPath).path());  // 确保目录存在
    
    // 如果数据库文件不存在，需要创建表
    bool needInit = !QFile::exists(dbPath);
    
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        qDebug() << "无法打开数据库:" << m_db.lastError().text();
        return false;
    }
    
    if (needInit) {
        return createTables();
    }
    
    return true;
}

bool DatabaseManager::createTables() {
    QString sql = R"(
        -- 用户表
        CREATE TABLE IF NOT EXISTS users (
            username TEXT PRIMARY KEY,
            password TEXT NOT NULL,
            type INTEGER NOT NULL  -- 0: 顾客, 1: 商家
        );

        -- 店铺表
        CREATE TABLE IF NOT EXISTS store (
            username TEXT PRIMARY KEY,
            storename TEXT NOT NULL,
            address TEXT,
            photo TEXT,
            FOREIGN KEY (username) REFERENCES users(username)
        );

        -- 订单表
        CREATE TABLE IF NOT EXISTS orders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            storename TEXT NOT NULL,
            ordertime DATETIME DEFAULT CURRENT_TIMESTAMP,
            status INTEGER DEFAULT 0,  -- 0: 待处理, 1: 已完成, 2: 已取消
            total REAL NOT NULL,
            FOREIGN KEY (username) REFERENCES users(username),
            FOREIGN KEY (storename) REFERENCES store(storename)
        );

        -- 订单详情表
        CREATE TABLE IF NOT EXISTS order_details (
            order_id INTEGER,
            dish_name TEXT NOT NULL,
            price REAL NOT NULL,
            quantity INTEGER NOT NULL,
            FOREIGN KEY (order_id) REFERENCES orders(id)
        );

        -- 收入记录表
        CREATE TABLE IF NOT EXISTS income (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            storename TEXT NOT NULL,
            amount REAL NOT NULL,
            date DATE DEFAULT CURRENT_DATE,
            FOREIGN KEY (storename) REFERENCES store(storename)
        );
    )";
    
    return executeSQL(sql);
}

bool DatabaseManager::executeSQL(const QString& sql) {
    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    
    for (const QString& statement : statements) {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty()) {
            continue;
        }
        
        QSqlQuery query(m_db);
        if (!query.exec(trimmed)) {
            qDebug() << "SQL执行失败:" << query.lastError().text();
            qDebug() << "SQL语句:" << trimmed;
            return false;
        }
    }
    
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return m_db;
}