#include "store.h"
#include "ui_store.h"
#include "secondwindow_toc.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QGraphicsScene>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// 构造函数
store::store(QString t_str, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::store)
{
    ui->setupUi(this);

    // 记录用户名
    setUsername(t_str);

    // 连接数据库
    m_dbStore = QSqlDatabase::addDatabase("QSQLITE", "store_connection");
    m_dbStore.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbStore.open()) {
        qDebug() << "Database open error!" << m_dbStore.lastError();
    }

    // 美化界面布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 店铺图片
    QLabel *photoLabel = new QLabel(this);
    photoLabel->setFixedSize(120, 120);
    photoLabel->setStyleSheet("border: 1px solid gray; margin: 10px;");
    ui->store_photo = photoLabel;

    // 店铺信息
    QVBoxLayout *infoLayout = new QVBoxLayout();
    QLabel *nameLabel = new QLabel("店铺名称: ", this);
    ui->name = new QLabel("unknown", this);

    QLabel *scoreLabel = new QLabel("评分: ", this);
    ui->score = new QLabel("5.0/5.0", this);

    QLabel *priceLabel = new QLabel("人均消费: ", this);
    ui->price = new QLabel("0.0 r/人", this);

    QLabel *addressLabel = new QLabel("地址: ", this);
    ui->address = new QLabel("N/A", this);
    ui->address->setAlignment(Qt::AlignRight);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(ui->name);
    infoLayout->addWidget(scoreLabel);
    infoLayout->addWidget(ui->score);
    infoLayout->addWidget(priceLabel);
    infoLayout->addWidget(ui->price);
    infoLayout->addWidget(addressLabel);
    infoLayout->addWidget(ui->address);

    // 图片和信息水平布局
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(photoLabel);
    topLayout->addLayout(infoLayout);

    // 按钮
    QPushButton *selectButton = new QPushButton("选择店铺", this);
    selectButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px; border-radius: 5px;");
    connect(selectButton, &QPushButton::clicked, this, &store::on_select_clicked);

    // 整体布局
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(selectButton);
}

// 设置店铺名称
void store::setName(QString t_str)
{
    m_strName = t_str;
    ui->name->setText(m_strName);
}

// 设置评分
void store::setScore(double t_d)
{
    m_dScore = t_d;
    ui->score->setText(QString("%1/5.0").arg(t_d));
}

// 设置人均消费
void store::setPrice(double t_d)
{
    m_dPrice = t_d;
    ui->price->setText(QString("%1 r/人").arg(t_d));
}

// 设置地址
void store::setAddress(QString t_str)
{
    m_strAddress = t_str;
    ui->address->setText(m_strAddress);
}

// 设置图片
void store::setPhoto(QString t_str)
{
    m_strPhoto = t_str;
    QPixmap pixmap(m_strPhoto);
    pixmap = pixmap.scaled(ui->store_photo->size(), Qt::KeepAspectRatio);
    ui->store_photo->setPixmap(pixmap);
}

// 析构函数
store::~store()
{
    delete ui;
    m_dbStore.close();
}

// 设置用户名
void store::setUsername(QString t_str)
{
    m_strUsername = t_str;
}

// 选择店铺按钮槽函数
void store::on_select_clicked()
{
    QSqlQuery query(m_dbStore);
    query.exec(QString("update %1 set quantity = 0").arg(m_strName));

    // 打开顾客选餐页面
    secondwindow_toc *t_ptrsw = new secondwindow_toc(m_strUsername, this->m_strName, this);
    t_ptrsw->show();
}