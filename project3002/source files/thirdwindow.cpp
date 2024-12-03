#include "thirdwindow_toc.h"
#include "ui_thirdwindow_toc.h"

#include <QTime>
#include <QGraphicsScene>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

// 顾客的第三个页面（订单界面）
thirdwindow_toc::thirdwindow_toc(QString t_strUsername, QString t_strStoreName, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::thirdwindow_toc),
      m_iPayment(0) {
    ui->setupUi(this);

    // 将菜单栏返回按钮与本页面关闭关联
    connect(ui->menu, &QMenu::aboutToShow, this, &thirdwindow_toc::close);

    // 记录店铺名称和顾客用户名
    setStoreName(t_strStoreName);
    setUsername(t_strUsername);

    // 初始化数据库连接
    m_dbbill = QSqlDatabase::addDatabase("QSQLITE");
    m_dbbill.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbbill.open()) {
        qDebug() << "Database open error:" << m_dbbill.lastError();
    }

    // 设置 QR 码显示
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap("./photo/QRcode1.png"));
    ui->QRcode->setScene(scene);

    // 设置账单表格不可编辑
    ui->bill->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 加载数据库中菜品数据到账单表格
    loadBillData();
}

thirdwindow_toc::~thirdwindow_toc() {
    delete ui;
    m_dbbill.close();
}

// 设置店铺名称
void thirdwindow_toc::setStoreName(QString t_str) {
    m_strStoreName = t_str;
    qDebug() << "Store Name:" << m_strStoreName;
}

// 设置用户名
void thirdwindow_toc::setUsername(QString t_str) {
    m_strUsername = t_str;
    qDebug() << "Username:" << m_strUsername;
}

// 设置支付金额并更新账单显示
void thirdwindow_toc::setPayment(int t_iPayment) {
    m_iPayment = t_iPayment;

    // 添加一行展示配送费
    int row = ui->bill->rowCount();
    ui->bill->insertRow(row);
    ui->bill->setItem(row, 2, new QTableWidgetItem("Delivery Fee:"));

    // 生成随机配送费
    qsrand(QTime::currentTime().msec());
    double deliveryFee = (qrand() % 30 + 20) * 0.1;
    ui->bill->setItem(row, 3, new QTableWidgetItem(QString::number(deliveryFee, 'f', 2)));

    // 添加一行展示总价
    row = ui->bill->rowCount();
    ui->bill->insertRow(row);
    ui->bill->setItem(row, 2, new QTableWidgetItem("Total:"));
    ui->bill->setItem(row, 3, new QTableWidgetItem(QString::number(m_iPayment + deliveryFee, 'f', 2)));
}

// 加载数据库中的菜品信息到账单表格
void thirdwindow_toc::loadBillData() {
    QSqlQuery query(m_dbbill);
    QString queryStr = QString("SELECT name, price, quantity FROM %1 WHERE quantity > 0").arg(m_strStoreName);

    if (!query.exec(queryStr)) {
        qDebug() << "Database query error:" << query.lastError();
        return;
    }

    while (query.next()) {
        int row = ui->bill->rowCount();
        ui->bill->insertRow(row);

        QTableWidgetItem *itemName = new QTableWidgetItem(query.value("name").toString());
        QTableWidgetItem *itemPrice = new QTableWidgetItem(query.value("price").toString());
        QTableWidgetItem *itemQuantity = new QTableWidgetItem(query.value("quantity").toString());
        QTableWidgetItem *itemTotal = new QTableWidgetItem(
            QString::number(query.value("price").toInt() * query.value("quantity").toInt()));

        ui->bill->setItem(row, 0, itemName);
        ui->bill->setItem(row, 1, itemPrice);
        ui->bill->setItem(row, 2, itemQuantity);
        ui->bill->setItem(row, 3, itemTotal);
    }
}

// 提交订单并保存到数据库
void thirdwindow_toc::on_pushButton_clicked() {
    QSqlQuery query(m_dbbill);

    // 检查并添加用户字段到店铺表中
    query.prepare("PRAGMA table_info(" + m_strStoreName + ")");
    if (query.exec()) {
        bool fieldExists = false;
        while (query.next()) {
            if (query.value("name").toString() == m_strUsername) {
                fieldExists = true;
                break;
            }
        }

        if (!fieldExists) {
            QString addColumnQuery = QString("ALTER TABLE %1 ADD COLUMN %2 INTEGER DEFAULT 0")
                                         .arg(m_strStoreName)
                                         .arg(m_strUsername);
            if (!query.exec(addColumnQuery)) {
                qDebug() << "Add column error:" << query.lastError();
            }
        }
    }

    // 更新订单记录
    QString updateQuery = QString("UPDATE %1 SET %2 = %2 + quantity WHERE quantity > 0").arg(m_strStoreName).arg(m_strUsername);
    if (!query.exec(updateQuery)) {
        qDebug() << "Update order error:" << query.lastError();
    }

    // 重置订单数量
    QString resetQuery = QString("UPDATE %1 SET quantity = 0").arg(m_strStoreName);
    if (!query.exec(resetQuery)) {
        qDebug() << "Reset order error:" << query.lastError();
    }

    // 显示支付成功消息
    QMessageBox::information(this, "Payment Successful", "Returning to main window.");
    this->close();

    // 发送关闭信号
    emit closeSecondwindow();
}