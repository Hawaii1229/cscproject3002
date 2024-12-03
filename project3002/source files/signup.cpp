#include "signup.h"
#include "ui_signup.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>
#include <QPalette>

Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    setWindowTitle("Signup");

    // 设置窗口背景颜色
    QPalette palette;
    palette.setColor(QPalette::Background, QColor("#f5f5f5"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 设置输入框提示和样式
    ui->password->setPlaceholderText("Enter numeric password");
    ui->username->setPlaceholderText("Enter your username");
    ui->ensure_password->setPlaceholderText("Confirm your password");
    ui->storename->setPlaceholderText("Enter store name (if business)");
    ui->password->setEchoMode(QLineEdit::Password);
    ui->ensure_password->setEchoMode(QLineEdit::Password);

    // 隐藏商家输入框和标签
    ui->storename->hide();
    ui->label_4->hide();

    // 按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: #4CAF50;"
                          "color: white;"
                          "border-radius: 8px;"
                          "padding: 10px;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #45a049;"
                          "}";
    ui->signup->setStyleSheet(buttonStyle);
    ui->back->setStyleSheet(buttonStyle);

    // 连接数据库
    m_dbSignup = QSqlDatabase::addDatabase("QSQLITE");
    m_dbSignup.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbSignup.open()) {
        qDebug() << "Database connection failed:" << m_dbSignup.lastError();
    }
}

Signup::~Signup()
{
    delete ui;
    m_dbSignup.close();
}

// 注册按钮逻辑
void Signup::on_signup_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
    QString confirmPassword = ui->ensure_password->text();
    QString storeName = ui->storename->text();

    // 验证输入
    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill all required fields.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Warning", "Passwords do not match.");
        ui->password->clear();
        ui->ensure_password->clear();
        return;
    }

    QSqlQuery query(m_dbSignup);
    query.prepare("SELECT username FROM users WHERE username = :username");
    query.bindValue(":username", username);
    query.exec();

    if (query.next()) {
        QMessageBox::warning(this, "Warning", "Username already exists.");
        ui->username->clear();
        return;
    }

    // 如果选择商家账户
    if (ui->checktype->isChecked()) {
        if (storeName.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please enter your store name.");
            return;
        }

        query.prepare("INSERT INTO users (username, password, type) VALUES (:username, :password, 'business')");
        query.bindValue(":username", username);
        query.bindValue(":password", password.toInt());
        query.exec();

        query.prepare("INSERT INTO store (username, storename, address) VALUES (:username, :storename, 'cuhksz')");
        query.bindValue(":username", username);
        query.bindValue(":storename", storeName);
        query.exec();
    } else {
        // 顾客账户
        query.prepare("INSERT INTO users (username, password, type) VALUES (:username, :password, 'customer')");
        query.bindValue(":username", username);
        query.bindValue(":password", password.toInt());
        query.exec();
    }

    QMessageBox::information(this, "Success", "Registration successful!");
    this->close();
}

// 返回按钮逻辑
void Signup::on_back_clicked()
{
    m_dbSignup.close();
    this->close();
}

// 密码显示模式切换
void Signup::on_password_show_stateChanged(int arg1)
{
    ui->password->setEchoMode(arg1 == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password);
}

void Signup::on_ensure_password_show_stateChanged(int arg1)
{
    ui->ensure_password->setEchoMode(arg1 == Qt::Checked ? QLineEdit::Normal : QLineEdit::Password);
}

// 商家账户选项切换
void Signup::on_checktype_stateChanged(int arg1)
{
    bool isChecked = (arg1 == Qt::Checked);
    ui->label_4->setVisible(isChecked);
    ui->storename->setVisible(isChecked);
}