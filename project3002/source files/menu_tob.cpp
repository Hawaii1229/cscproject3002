#include "menu_tob.h"
#include "ui_menu_tob.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

// 构造函数
Menu::Menu(QString t_strStorename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    // 设置店铺名
    setStoreName(t_strStorename);

    // 打开数据库
    m_dbMenu = QSqlDatabase::addDatabase("QSQLITE");
    m_dbMenu.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");
    if (!m_dbMenu.open()) {
        qDebug() << "Database open error!";
    }

    // 初始化样式
    ui->name->setPlaceholderText("菜品名称");
    ui->price->setMinimum(0);
    ui->price->setSuffix(" ¥");
    ui->introduction->setPlaceholderText("菜品介绍");
    ui->photo->setPlaceholderText("图片路径 (可选)");
    ui->id->setPlaceholderText("菜品ID (仅删除/修改时)");

    ui->pushButton->setText("添加");
    ui->pushButton_2->setText("刷新");
    ui->pushButton_3->setText("删除");
    ui->pushButton_4->setText("修改");

    ui->pushButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;");
    ui->pushButton_2->setStyleSheet("background-color: #2196F3; color: white; border-radius: 5px;");
    ui->pushButton_3->setStyleSheet("background-color: #f44336; color: white; border-radius: 5px;");
    ui->pushButton_4->setStyleSheet("background-color: #FFC107; color: white; border-radius: 5px;");

    ui->tableView->setStyleSheet("border: 1px solid #ddd; border-radius: 5px;");

    // 展示菜单
    refresh();
}

// 析构函数
Menu::~Menu()
{
    delete ui;
    m_dbMenu.close();
}

// 设置店铺名
void Menu::setStoreName(QString t_str)
{
    m_strStoreName = t_str;
}

// 刷新菜单
void Menu::refresh()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(QString("SELECT id, name, price, introduction, photo FROM %1").arg(m_strStoreName), m_dbMenu);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "菜品");
    model->setHeaderData(2, Qt::Horizontal, "单价");
    model->setHeaderData(3, Qt::Horizontal, "介绍");
    model->setHeaderData(4, Qt::Horizontal, "图片路径");

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

// 添加菜品
void Menu::on_pushButton_clicked()
{
    if (ui->name->text().isEmpty() || ui->price->value() == 0 || ui->introduction->toPlainText().isEmpty()) {
        QMessageBox::warning(this, "警告", "请填写完整信息 (名称、单价、介绍)", QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_dbMenu);
    QString photo = ui->photo->toPlainText();
    bool success = photo.isEmpty()
        ? query.exec(QString("INSERT INTO %1 (name, price, introduction, quantity, total) VALUES ('%2', %3, '%4', 0, 0)")
                     .arg(m_strStoreName)
                     .arg(ui->name->text())
                     .arg(ui->price->value())
                     .arg(ui->introduction->toPlainText()))
        : query.exec(QString("INSERT INTO %1 (name, price, introduction, photo, quantity, total) VALUES ('%2', %3, '%4', '%5', 0, 0)")
                     .arg(m_strStoreName)
                     .arg(ui->name->text())
                     .arg(ui->price->value())
                     .arg(ui->introduction->toPlainText())
                     .arg(photo));

    if (!success) {
        qDebug() << "Insert error!";
    } else {
        QMessageBox::information(this, "成功", "菜品已成功添加！", QMessageBox::Ok);
        refresh();
    }
}

// 刷新按钮
void Menu::on_pushButton_2_clicked()
{
    refresh();
}

// 删除菜品
void Menu::on_pushButton_3_clicked()
{
    if (ui->id->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入要删除的菜品ID", QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_dbMenu);
    query.exec(QString("SELECT COUNT(*) FROM %1 WHERE id = %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
    query.next();

    if (query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "警告", "ID不存在", QMessageBox::Ok);
        return;
    }

    query.exec(QString("DELETE FROM %1 WHERE id = %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
    QMessageBox::information(this, "成功", "菜品已成功删除！", QMessageBox::Ok);
    refresh();
}

// 修改菜品
void Menu::on_pushButton_4_clicked()
{
    if (ui->id->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入要修改的菜品ID", QMessageBox::Ok);
        return;
    }

    QSqlQuery query(m_dbMenu);
    query.exec(QString("SELECT COUNT(*) FROM %1 WHERE id = %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
    query.next();

    if (query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "警告", "ID不存在", QMessageBox::Ok);
        return;
    }

    if (!ui->name->text().isEmpty()) {
        query.exec(QString("UPDATE %1 SET name = '%2' WHERE id = %3").arg(m_strStoreName).arg(ui->name->text()).arg(ui->id->text().toInt()));
    }
    if (ui->price->value() != 0) {
        query.exec(QString("UPDATE %1 SET price = %2 WHERE id = %3").arg(m_strStoreName).arg(ui->price->value()).arg(ui->id->text().toInt()));
    }
    if (!ui->introduction->toPlainText().isEmpty()) {
        query.exec(QString("UPDATE %1 SET introduction = '%2' WHERE id = %3").arg(m_strStoreName).arg(ui->introduction->toPlainText()).arg(ui->id->text().toInt()));
    }
    if (!ui->photo->toPlainText().isEmpty()) {
        query.exec(QString("UPDATE %1 SET photo = '%2' WHERE id = %3").arg(m_strStoreName).arg(ui->photo->toPlainText()).arg(ui->id->text().toInt()));
    }

    QMessageBox::information(this, "成功", "菜品已成功修改！", QMessageBox::Ok);
    refresh();
}