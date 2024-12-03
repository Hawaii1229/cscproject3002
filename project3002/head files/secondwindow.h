#ifndef SECONDWINDOW_TOC_H
#define SECONDWINDOW_TOC_H

#include <QMainWindow>
#include <QSqlDatabase>

// 顾客的第二页面（选餐页面）
namespace Ui {
class secondwindow_toc;
}

class secondwindow_toc : public QMainWindow {
    Q_OBJECT

public:
    explicit secondwindow_toc(QString t_strUsername, QString t_strStoreName, QWidget* parent = nullptr);
    ~secondwindow_toc();

    void setStoreName(QString t_str); // 设置店铺名
    void setUsername(QString t_str); // 设置用户名

private slots:
    void calPayment();               // 实时计算总价槽函数
    void on_pushButton_clicked();    // 确定订单按钮槽函数

private:
    Ui::secondwindow_toc* ui;
    QSqlDatabase m_dbdishes;         // 数据库连接
    QString m_strStoreName;          // 当前店铺名
    QString m_strUsername;           // 当前用户名
    int m_iPayment;                  // 当前总价
};

#endif // SECONDWINDOW_TOC_H