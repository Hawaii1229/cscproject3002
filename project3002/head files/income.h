#ifndef INCOME_TOB_H
#define INCOME_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

// 店铺营收页面
namespace Ui {
class Income;
}

class Income : public QMainWindow {
    Q_OBJECT

public:
    explicit Income(QString storename, QWidget *parent = nullptr);
    ~Income();

    void setStorename(QString storename);  // 记录店铺名的函数

private:
    Ui::Income *ui;
    QSqlDatabase m_dbIncome;  // 数据库连接
    QString m_strStorename;   // 店铺名称
};

#endif // INCOME_TOB_H