#ifndef DISHES_H
#define DISHES_H

#include <QWidget>
#include <QSqlDatabase>
#include <memory> // 使用智能指针

// 单个菜品模块
namespace Ui {
class Dishes;
}

class Dishes : public QWidget
{
    Q_OBJECT

public:
    explicit Dishes(QWidget *parent = nullptr); 
    ~Dishes();

    // 设置菜品属性
    void setName(const QString& name);                // 设置菜品名称
    void setIntroduction(const QString& introduction); // 设置菜品介绍
    void setPrice(int price);                         // 设置菜品单价
    void setStoreName(const QString& storeName);      // 设置店铺名称
    void setPhoto(const QString& photoPath);          // 设置菜品图片

signals:
    void signalToCalcPayment(); // 计算总价信号

public slots:
    void onQuantityValueChanged(int quantity); // 数量改变时触发的槽函数

private:
    Ui::Dishes *ui;
    QString m_name;
    QString m_introduction;
    QString m_storeName;
    QString m_photoPath;
    int m_price = 0;
    int m_quantity = 0;
    QSqlDatabase m_dbDish;
};

#endif // DISHES_H