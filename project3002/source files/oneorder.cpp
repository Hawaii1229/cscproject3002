#include "oneorder.h"
#include "ui_oneorder.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

// 店铺订单的单个模块
oneOrder::oneOrder(QString t_strStorename, QString t_strUsername, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::oneOrder) {
    ui->setupUi(this);

    // 记录店铺名和用户名
    setStorename(t_strStorename);
    setUsername(t_strUsername);

    // 打开数据库，并且拥有单独的连接，与其他模块的数据库连接区分
    m_dboneorder = QSqlDatabase::addDatabase("QSQLITE", "oneorderconnection");
    m_dboneorder.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");

    if (!m_dboneorder.open()) {
        qDebug() << "Database open error:" << m_dboneorder.lastError();
        QMessageBox::critical(this, "Database Error", "Failed to open the database.");
        return;
    }

    // 查询顾客购买的菜品数量
    QSqlQuery query(m_dboneorder);
    QString queryStr = QString("SELECT name, \"%1\" FROM \"%2\" WHERE \"%1\" != 0").arg(m_strUsername, m_strStorename);

    if (!query.exec(queryStr)) {
        qDebug() << "Query execution error:" << query.lastError();
        QMessageBox::critical(this, "Query Error", "Failed to execute query to fetch order details.");
        return;
    }

    // 构建订单字符串并显示
    QString t_strShow = m_strUsername + "\n";
    while (query.next()) {
        t_strShow += QString("%1  %2份\n")
                         .arg(query.value("name").toString())
                         .arg(query.value(m_strUsername).toInt());
    }

    showOrder(t_strShow);
}

oneOrder::~oneOrder() {
    delete ui;

    if (m_dboneorder.isOpen()) {
        m_dboneorder.close();
    }

    QSqlDatabase::removeDatabase("oneorderconnection");
}

// 显示订单字符串到文本框
void oneOrder::showOrder(QString t_str) {
    ui->showorder->setPlainText(t_str);
}

// 记录店铺名
void oneOrder::setStorename(QString t_str) {
    m_strStorename = t_str;
}

// 记录用户名
void oneOrder::setUsername(QString t_str) {
    m_strUsername = t_str;
}

// 完成订单按钮槽函数
void oneOrder::on_complete_clicked() {
    if (!m_dboneorder.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database connection is not open.");
        return;
    }

    QSqlQuery query(m_dboneorder);

    // 查询所有菜品
    QString selectStr = QString("SELECT * FROM \"%1\"").arg(m_strStorename);
    if (!query.exec(selectStr)) {
        qDebug() << "Select query error:" << query.lastError();
        QMessageBox::critical(this, "Query Error", "Failed to fetch order details.");
        return;
    }

    // 更新每个菜品的总销售量
    while (query.next()) {
        QString itemName = query.value("name").toString();
        int currentTotal = query.value("total").toInt();
        int customerOrder = query.value(m_strUsername).toInt();

        if (customerOrder > 0) {
            QSqlQuery updateQuery(m_dboneorder);
            QString updateStr = QString("UPDATE \"%1\" SET total = %2 WHERE name = '%3'")
                                    .arg(m_strStorename)
                                    .arg(currentTotal + customerOrder)
                                    .arg(itemName);

            if (!updateQuery.exec(updateStr)) {
                qDebug() << "Update query error for" << itemName << ":" << updateQuery.lastError();
            }
        }
    }

    // 清零顾客的订单数据
    QString clearStr = QString("UPDATE \"%1\" SET \"%2\" = 0").arg(m_strStorename, m_strUsername);
    if (!query.exec(clearStr)) {
        qDebug() << "Clear order query error:" << query.lastError();
        QMessageBox::critical(this, "Query Error", "Failed to clear customer order.");
    }

    QMessageBox::information(this, "Order Completed", "Order has been successfully processed.");
    this->close();
}