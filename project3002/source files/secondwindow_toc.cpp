#include "secondwindow_toc.h"
#include "ui_secondwindow_toc.h"
#include "dishes.h"
#include "thirdwindow_toc.h"
#include "DatabaseManager.h"

#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

// 顾客的第二页面（选餐页面）
secondwindow_toc::secondwindow_toc(QString t_strUsername, QString t_strStoreName, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::secondwindow_toc), m_iPayment(0) {
    ui->setupUi(this);

    // 初始化用户名与店铺名
    setUsername(t_strUsername);
    setStoreName(t_strStoreName);
    setWindowTitle(m_strStoreName);

    // 设置返回按钮功能
    connect(ui->menu, &QMenu::aboutToShow, this, &secondwindow_toc::close);

    // 连接数据库
    m_dbdishes = DatabaseManager::getConnection("dishes_connection");

    // 设置可滚动的菜品展示区域
    QWidget* widget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(widget);
    ui->m_scrollArea->setWidget(widget);
    ui->m_scrollArea->setWidgetResizable(true);

    // 从数据库加载店铺菜品信息
    QSqlQuery query(m_dbdishes);
    QString queryStr = QString("SELECT * FROM `%1`").arg(m_strStoreName);
    if (!query.exec(queryStr)) {
        qDebug() << "Database query error:" << query.lastError();
    } else {
        while (query.next()) {
            // 动态生成每个菜品控件
            dishes* dish = new dishes(this);
            dish->setName(query.value("name").toString());
            dish->setIntroduction(query.value("introduction").toString());
            dish->setPrice(query.value("price").toInt());
            dish->setPhoto(query.value("photo").toString());
            layout->addWidget(dish);

            // 连接选餐信号与槽函数
            connect(dish, &dishes::signaltocalPayment, this, &secondwindow_toc::calPayment);
        }
    }
}

secondwindow_toc::~secondwindow_toc() {
    delete ui;
}

// 设置店铺名称
void secondwindow_toc::setStoreName(QString t_str) {
    m_strStoreName = t_str;
}

// 设置用户名
void secondwindow_toc::setUsername(QString t_str) {
    m_strUsername = t_str;
    qDebug() << "Username:" << m_strUsername;
}

// 实时计算选中餐品总价并更新显示
void secondwindow_toc::calPayment() {
    QSqlQuery query(m_dbdishes);
    QString queryStr = QString("SELECT price, quantity FROM `%1` WHERE quantity > 0").arg(m_strStoreName);
    if (!query.exec(queryStr)) {
        qDebug() << "Database query error:" << query.lastError();
    } else {
        m_iPayment = 0; // 清空总价
        while (query.next()) {
            m_iPayment += query.value("price").toInt() * query.value("quantity").toInt();
        }
        ui->payment->display(m_iPayment); // 更新显示总价
    }
}

// 确定订单按钮逻辑
void secondwindow_toc::on_pushButton_clicked() {
    if (m_iPayment == 0) {
        QMessageBox::warning(this, "提示", "未选择任何菜品！");
        return;
    }
    // 进入第三页面（订单确认与支付页面）
    thirdwindow_toc* tw = new thirdwindow_toc(m_strUsername, m_strStoreName, this);
    tw->setPayment(m_iPayment);
    connect(tw, &thirdwindow_toc::closeSecondwindow, this, &secondwindow_toc::close);
    tw->show();
}