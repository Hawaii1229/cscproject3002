#ifndef THIRDWINDOW_TOC_H
#define THIRDWINDOW_TOC_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class thirdwindow_toc;
}

class thirdwindow_toc : public QMainWindow {
    Q_OBJECT

public:
    explicit thirdwindow_toc(QString username, QString storeName, QWidget *parent = nullptr);
    ~thirdwindow_toc();

    void setStoreName(QString storeName); // 设置店铺名称
    void setUsername(QString username);  // 设置用户名
    void setPayment(int payment);        // 设置账单总价

private slots:
    void on_pushButton_clicked();        // 确认支付按钮槽函数

signals:
    void closeSecondwindow();            // 支付完成后关闭第二窗口信号

private:
    void loadBillData();                 // 加载账单数据

private:
    Ui::thirdwindow_toc *ui;
    QSqlDatabase m_dbbill;               // 数据库连接
    QString m_strStoreName;              // 店铺名称
    QString m_strUsername;               // 用户名
    int m_iPayment;                      // 总价
};

#endif // THIRDWINDOW_TOC_H