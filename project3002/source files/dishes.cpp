#include "dishes.h"
#include "ui_dishes.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QGraphicsScene>

// 单个菜品模块构造函数
Dishes::Dishes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dishes)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("Dishes Module");

    // 设置菜品图片显示的样式
    ui->dishesPhoto->setStyleSheet("border: 1px solid #ccc; background-color: #f9f9f9;");

    // 连接数据库
    m_dbDish = QSqlDatabase::addDatabase("QSQLITE", "dish_connection");
    m_dbDish.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbDish.open()) {
        qDebug() << "Database open error:" << m_dbDish.lastError();
    }
}

// 析构函数
Dishes::~Dishes()
{
    delete ui;
    m_dbDish.close();
}

// 设置菜品名称
void Dishes::setName(const QString& name)
{
    m_name = name;
    ui->name->setText(name);
}

// 设置菜品介绍
void Dishes::setIntroduction(const QString& introduction)
{
    m_introduction = introduction;
    ui->introduction->setText(introduction);
}

// 设置菜品价格
void Dishes::setPrice(int price)
{
    m_price = price;
    ui->price->setText(QString("%1 r").arg(price));
}

// 设置店铺名称
void Dishes::setStoreName(const QString& storeName)
{
    m_storeName = storeName;
}

// 设置菜品图片
void Dishes::setPhoto(const QString& photoPath)
{
    m_photoPath = photoPath;

    // 加载图片并设置缩放大小
    QPixmap pixmap(photoPath);
    pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);

    // 使用智能指针管理 QGraphicsScene
    auto scene = std::make_unique<QGraphicsScene>();
    scene->addPixmap(pixmap);

    // 显示图片
    ui->dishesPhoto->setScene(scene.release());
}

// 数量改变时触发的槽函数
void Dishes::onQuantityValueChanged(int quantity)
{
    m_quantity = quantity;

    QSqlQuery query(m_dbDish);
    QString sql = QString("UPDATE %1 SET quantity = %2 WHERE name = '%3'")
                    .arg(m_storeName)
                    .arg(quantity)
                    .arg(m_name);

    // 更新数据库中的数量
    if (!query.exec(sql)) {
        qDebug() << "Update quantity error:" << m_dbDish.lastError();
    }

    // 发出信号通知总价更新
    emit signalToCalcPayment();
}