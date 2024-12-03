#include "home_tob.h"
#include "ui_home_tob.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>

// 构造函数
home::home(QString t_strUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::home),
    m_photoScene(new QGraphicsScene(this))
{
    ui->setupUi(this);

    // 记录用户名
    setUsername(t_strUsername);

    // 打开数据库
    m_dbhome = QSqlDatabase::addDatabase("QSQLITE");
    m_dbhome.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbhome.open()) {
        qDebug() << "Database open error!";
    }

    // 初始化样式
    ui->storename->setPlaceholderText("请输入店铺名称");
    ui->storeaddress->setPlaceholderText("请输入店铺地址");
    ui->storephoto->setPlaceholderText("图片路径 (可选)");

    ui->storename->setStyleSheet("padding: 6px; border: 1px solid #ccc; border-radius: 5px;");
    ui->storeaddress->setStyleSheet("padding: 6px; border: 1px solid #ccc; border-radius: 5px;");
    ui->storephoto->setStyleSheet("padding: 6px; border: 1px solid #ccc; border-radius: 5px;");
    ui->change->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px; padding: 10px 20px;");
    ui->refresh->setStyleSheet("background-color: #2196F3; color: white; border-radius: 5px; padding: 10px 20px;");
    ui->photo->setStyleSheet("border: 2px solid #ddd; border-radius: 10px;");

    // 设置初始图片显示区域
    ui->photo->setScene(m_photoScene);

    // 显示个人信息
    refresh();
}

// 析构函数
home::~home()
{
    delete ui;
    m_dbhome.close();
}

// 设置用户名
void home::setUsername(QString t_str)
{
    m_strUsername = t_str;
}

// 设置店铺名
void home::setStorename(QString t_str)
{
    m_strStorename = t_str;
}

// 刷新页面信息
void home::refresh()
{
    QSqlQuery query(m_dbhome);
    if (!query.exec(QString("SELECT storename, address, photo FROM store WHERE username = '%1'").arg(m_strUsername))) {
        qDebug() << "Query error!" << m_strUsername;
        return;
    }

    if (query.next()) {
        ui->storename->setText(query.value("storename").toString());
        ui->storeaddress->setText(query.value("address").toString());
        ui->storephoto->setText(query.value("photo").toString());
        setStorename(query.value("storename").toString());

        // 显示店铺图片
        QString photoPath = query.value("photo").toString();
        QPixmap pixmap(photoPath);
        if (!pixmap.isNull()) {
            pixmap = pixmap.scaled(180, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPixmap roundedPixmap(180, 180);
            roundedPixmap.fill(Qt::transparent);
            QPainter painter(&roundedPixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            QBrush brush(pixmap);
            painter.setBrush(brush);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(0, 0, 180, 180, 20, 20);

            m_photoScene->clear();
            m_photoScene->addPixmap(roundedPixmap);
        }
    }
}

// 修改信息按钮
void home::on_change_clicked()
{
    if (ui->storename->toPlainText().isEmpty() || ui->storeaddress->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "失败", "店铺名称和地址不能为空！", QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_dbhome);
    query.exec(QString("UPDATE store SET storename = '%1', address = '%2' WHERE username = '%3'")
                   .arg(ui->storename->toPlainText())
                   .arg(ui->storeaddress->toPlainText())
                   .arg(m_strUsername));

    if (!ui->storephoto->toPlainText().isEmpty()) {
        query.exec(QString("UPDATE store SET photo = '%1' WHERE username = '%2'")
                       .arg(ui->storephoto->toPlainText())
                       .arg(m_strUsername));
    }

    query.exec(QString("ALTER TABLE %1 RENAME TO %2")
                   .arg(m_strStorename)
                   .arg(ui->storename->toPlainText()));

    QMessageBox::information(this, "成功", "信息已成功修改！", QMessageBox::Ok);
    emit closewindow();
    close();
}