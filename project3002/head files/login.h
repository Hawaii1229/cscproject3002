#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>
#include <opencv2/opencv.hpp>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_login_clicked();                // 登录按钮槽函数
    void on_signup_clicked();               // 注册按钮槽函数
    void on_password_show_stateChanged(int arg1); // 显示密码勾选框槽函数
    void on_face_login_clicked();          // 人脸登录按钮槽函数

private:
    void promptForMissingFiles();          // 检测文件是否存在并提示
    bool validateFaceLogin();              // 人脸识别逻辑

    Ui::Login *ui;
    QSqlDatabase m_dbLogin;
    QString m_faceCascadePath;             // 人脸检测器文件路径
};

#endif // LOGIN_H