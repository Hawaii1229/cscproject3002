#ifndef HOME_TOB_H
#define HOME_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QGraphicsScene>

namespace Ui {
class home;
}

class home : public QMainWindow
{
    Q_OBJECT

public:
    explicit home(QString t_strUsername, QWidget *parent = nullptr);
    ~home();

    void setUsername(QString t_str);
    void setStorename(QString t_str);

private slots:
    void on_change_clicked();
    void refresh();

signals:
    void closewindow();

private:
    Ui::home *ui;
    QSqlDatabase m_dbhome;
    QString m_strUsername;
    QString m_strStorename;
    QGraphicsScene *m_photoScene;
};

#endif // HOME_TOB_H