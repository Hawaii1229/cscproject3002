#ifndef ORDER_TOB_H
#define ORDER_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

// 店铺订单管理界面
namespace Ui {
class order_tob;
}

class order_tob : public QMainWindow {
    Q_OBJECT

public:
    explicit order_tob(QString storename, QWidget *parent = nullptr);
    ~order_tob();

    void setStorename(QString storename); // 设置店铺名的函数

private:
    Ui::order_tob *ui;
    QSqlDatabase m_dborder;  // 数据库连接
    QString m_strStorename;  // 店铺名称
};

#endif // ORDER_TOB_H