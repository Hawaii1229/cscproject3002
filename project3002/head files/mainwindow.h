#ifndef MAINWINDOW_TOB_H
#define MAINWINDOW_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class MainWindow_tob;
}

// 商家主页面类
class MainWindow_tob : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow_tob(QString username, QWidget *parent = nullptr);
    ~MainWindow_tob();

    void setStoreName(QString storeName); // 设置店铺名称
    void setUsername(QString username);  // 设置用户名

private slots:
    void on_pushButton_clicked();   // 菜单管理按钮槽函数
    void on_pushButton_2_clicked(); // 订单管理按钮槽函数
    void on_pushButton_3_clicked(); // 营收查看按钮槽函数
    void on_pushButton_4_clicked(); // 个人中心按钮槽函数

private:
    Ui::MainWindow_tob *ui;
    QString m_strStoreName;         // 店铺名称
    QString m_strUsername;          // 用户名
    QSqlDatabase m_dbtob;           // 数据库连接
};

#endif // MAINWINDOW_TOB_H