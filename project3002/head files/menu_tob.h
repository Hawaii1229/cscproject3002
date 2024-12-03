#ifndef MENU_TOB_H
#define MENU_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QString t_strStorename, QWidget *parent = nullptr);
    ~Menu();

    void setStoreName(QString t_str);

private slots:
    void refresh();
    void on_pushButton_clicked();   // 添加菜品
    void on_pushButton_2_clicked(); // 刷新菜单
    void on_pushButton_3_clicked(); // 删除菜品
    void on_pushButton_4_clicked(); // 修改菜品信息

private:
    Ui::Menu *ui;
    QSqlDatabase m_dbMenu;
    QString m_strStoreName;
};

#endif // MENU_TOB_H