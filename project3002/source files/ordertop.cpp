#include "order_tob.h"
#include "ui_order_tob.h"
#include "oneorder.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QMessageBox>

// 店铺订单管理界面
order_tob::order_tob(QString t_strStorename, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::order_tob) {
    ui->setupUi(this);

    // 记录店铺名称
    setStorename(t_strStorename);

    // 创建动态添加订单模块的布局
    QWidget *widget = new QWidget(this);
    QVBoxLayout *m_Layout = new QVBoxLayout(widget);
    widget->setLayout(m_Layout);

    ui->m_scrollArea->setWidget(widget);
    ui->m_scrollArea->setWidgetResizable(true);

    // 连接数据库
    m_dborder = QSqlDatabase::addDatabase("QSQLITE", "orderconnection");
    m_dborder.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");

    if (!m_dborder.open()) {
        qDebug() << "Database open error:" << m_dborder.lastError();
        QMessageBox::critical(this, "Database Error", "Failed to open the database.");
        return;
    }

    // 获取 `users` 表中所有顾客的用户名
    QSqlQuery query(m_dborder);
    QString customerQuery = "SELECT username FROM users WHERE type='customer'";

    if (!query.exec(customerQuery)) {
        qDebug() << "Customer query error:" << query.lastError();
        QMessageBox::critical(this, "Query Error", "Failed to fetch customer usernames.");
        return;
    }

    // 遍历顾客用户名并判断是否有订餐
    while (query.next()) {
        QString username = query.value("username").toString();

        // 检查顾客是否在本店铺下单
        QSqlQuery query1(m_dborder);
        QString orderQuery = QString("SELECT \"%1\" FROM \"%2\" WHERE \"%1\" != 0").arg(username, m_strStorename);

        if (query1.exec(orderQuery) && query1.next()) {
            // 若订餐，则创建一个 `oneOrder` 模块
            oneOrder *oneorder = new oneOrder(m_strStorename, username, this);

            // 将该 `oneOrder` 模块添加到布局中
            m_Layout->addWidget(oneorder);
        } else if (query1.lastError().isValid()) {
            qDebug() << "Order query error for customer:" << username << ":" << query1.lastError();
        }
    }
}

order_tob::~order_tob() {
    delete ui;

    if (m_dborder.isOpen()) {
        m_dborder.close();
    }

    QSqlDatabase::removeDatabase("orderconnection");
}

// 设置店铺名
void order_tob::setStorename(QString t_str) {
    m_strStorename = t_str;
}