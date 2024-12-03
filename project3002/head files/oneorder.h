#ifndef ONEORDER_H
#define ONEORDER_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class oneOrder;
}

// 店铺订单的单个模块
class oneOrder : public QWidget {
    Q_OBJECT

public:
    explicit oneOrder(QString storename, QString username, QWidget *parent = nullptr);
    ~oneOrder();

    void showOrder(QString orderStr);  // 显示订单字符串
    void setUsername(QString username); // 设置用户名
    void setStorename(QString storename); // 设置店铺名

private slots:
    void on_complete_clicked(); // 完成订单按钮槽函数

private:
    Ui::oneOrder *ui;
    QString m_strUsername;     // 用户名
    QString m_strStorename;    // 店铺名
    QSqlDatabase m_dboneorder; // 数据库连接
};

#endif // ONEORDER_H