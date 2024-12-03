#include "income_tob.h"
#include "ui_income_tob.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts>

// 店铺营收页面
Income::Income(QString t_strStorename, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Income) {
    ui->setupUi(this);

    // 记录店铺名称
    setStorename(t_strStorename);

    // 打开数据库
    m_dbIncome = QSqlDatabase::addDatabase("QSQLITE");
    m_dbIncome.setDatabaseName("C:\\Users\\86185\\Desktop\\CSC3170 project\\Project\\Project\\data\\waimai.db");

    if (!m_dbIncome.open()) {
        qDebug() << "Database open error:" << m_dbIncome.lastError();
        return;
    }

    // 创建图表视图并设置到布局中
    QChartView *chartview = new QChartView(this);
    ui->m_Layout->addWidget(chartview);  // 在布局中添加QChartView对象

    QChart *chart = new QChart;
    chartview->setChart(chart);  // 将QChart加入QChartView

    QBarSeries *series = new QBarSeries;  // 创建柱状图数据系列

    // 从数据库中获取各个菜品卖出的总数量，并计算营收
    QSqlQuery query(m_dbIncome);
    QString queryString = QString("SELECT * FROM %1 WHERE total != 0").arg(m_strStorename);

    if (!query.exec(queryString)) {
        qDebug() << "Query error:" << query.lastError();
        return;
    }

    int totalIncome = 0;

    // 构建柱状图，添加每个菜品的营收数据
    while (query.next()) {
        QBarSet *set = new QBarSet(query.value("name").toString());
        int totalPrice = query.value("total").toInt() * query.value("price").toInt();
        *set << totalPrice;  // 每个菜品的总收入

        series->append(set);  // 将柱子加入系列
        totalIncome += totalPrice;  // 计算总收入
    }

    ui->lcdNumber->display(totalIncome);  // 显示总收入

    // 将柱状图系列添加到图表
    chart->addSeries(series);
    chart->createDefaultAxes();  // 自动创建默认坐标轴

    // 设置Y轴标题
    chart->axisY()->setTitleText("收入/元");
}

Income::~Income() {
    delete ui;

    // 关闭数据库连接
    if (m_dbIncome.isOpen()) {
        m_dbIncome.close();
    }

    QSqlDatabase::removeDatabase("QSQLITE");
}

void Income::setStorename(QString t_str) {
    m_strStorename = t_str;
}