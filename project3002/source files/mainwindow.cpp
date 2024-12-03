#include "mainwindow_tob.h"
#include "ui_mainwindow_tob.h"
#include "menu_tob.h"
#include "order_tob.h"
#include "income_tob.h"
#include "home_tob.h"
#include "login.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

// 商家主页面
MainWindow_tob::MainWindow_tob(QString t_strUsername, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow_tob) {
    ui->setupUi(this);

    // 设置记录用户名并将对应的店铺名记录到数据成员中
    setStoreName(t_strUsername);

    // 设置上方标签为店铺名称
    ui->label->setText(m_strStoreName);

    // 将菜单栏退出登录按钮与关闭页面和显示登录界面关联
    connect(ui->menu, &QMenu::aboutToShow, [=]() {
        Login *login = new Login;
        login->show();
        this->close();
    });

    // 初始化数据库连接
    m_dbtob = QSqlDatabase::addDatabase("QSQLITE");
    m_dbtob.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbtob.open()) {
        qDebug() << "Database open error:" << m_dbtob.lastError();
    }
}

MainWindow_tob::~MainWindow_tob() {
    delete ui;
    if (m_dbtob.isOpen()) {
        m_dbtob.close();
    }
}

// 设置用户名
void MainWindow_tob::setUsername(QString t_str) {
    m_strUsername = t_str;
}

// 设置店铺名
void MainWindow_tob::setStoreName(QString t_str) {
    setUsername(t_str);

    QSqlQuery query(m_dbtob);
    QString queryStr = QString("SELECT storename FROM store WHERE username = '%1'").arg(t_str);

    if (!query.exec(queryStr)) {
        qDebug() << "Query error:" << query.lastError();
        return;
    }

    if (query.next()) {
        m_strStoreName = query.value("storename").toString();
    } else {
        qDebug() << "Store name not found for username:" << t_str;
    }
}

// 菜单管理按钮槽函数
void MainWindow_tob::on_pushButton_clicked() {
    Menu *menu = new Menu(m_strStoreName, this);
    menu->show();
}

// 订单管理按钮槽函数
void MainWindow_tob::on_pushButton_2_clicked() {
    order_tob *order = new order_tob(m_strStoreName, this);
    order->show();
}

// 营收查看按钮槽函数
void MainWindow_tob::on_pushButton_3_clicked() {
    Income *income = new Income(m_strStoreName, this);
    income->show();
}

// 个人中心按钮槽函数
void MainWindow_tob::on_pushButton_4_clicked() {
    home *h = new home(m_strUsername, this);

    // 个人主页修改后返回登录界面
    connect(h, &home::closewindow, [=]() {
        Login *login = new Login;
        login->show();
        this->close();
    });

    h->show();
}