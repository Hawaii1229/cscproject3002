#ifndef STORE_H
#define STORE_H

#include <QWidget>
#include <QSqlDatabase>

// 店铺的子模块，用于显示店铺信息
namespace Ui {
class store;
}

class store : public QWidget
{
    Q_OBJECT

public:
    explicit store(QString, QWidget *parent = nullptr);
    void setName(QString = "unknown");       // 设置店铺的名字并显示
    void setScore(double = 5.0);            // 设置店铺评分
    void setPrice(double = 0.0);            // 设置人均价格
    void setAddress(QString = "N/A");       // 设置店铺地址
    void setPhoto(QString);                 // 设置店铺图片
    ~store();
    void setUsername(QString);              // 记录顾客用户名并向下传递

private slots:
    void on_select_clicked();               // 选择店铺按钮槽函数

private:
    Ui::store *ui;
    QString m_strName, m_strAddress, m_strUsername, m_strPhoto;  // 店铺相关信息
    double m_dScore, m_dPrice;                                   // 店铺评分和人均价格
    QSqlDatabase m_dbStore;
};

#endif // STORE_H