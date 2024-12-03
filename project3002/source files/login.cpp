#include "login.h"
#include "ui_login.h"
#include "mainwindow_toc.h"
#include "mainwindow_tob.h"
#include "signup.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <opencv2/opencv.hpp>

// 登录界面构造函数
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("Login");

    // 设置人脸检测器路径
    m_faceCascadePath = QCoreApplication::applicationDirPath() + "/resources/models/haarcascade_frontalface_default.xml";

    // 检测文件是否存在
    promptForMissingFiles();

    // 其他初始化逻辑
    QPushButton *faceLoginButton = new QPushButton("Face Login", this);
    connect(faceLoginButton, &QPushButton::clicked, this, &Login::on_face_login_clicked);
}

void Login::promptForMissingFiles()
{
    if (!QFile::exists(m_faceCascadePath)) {
        QMessageBox::critical(
            this,
            "Missing Files",
            QString("The required file `haarcascade_frontalface_default.xml` is missing. Please download it manually from:\n\n"
                    "https://github.com/opencv/opencv/raw/master/data/haarcascades/haarcascade_frontalface_default.xml\n\n"
                    "Save the file to: %1").arg(QFileInfo(m_faceCascadePath).absolutePath())
        );
    }
}


void Login::on_face_login_clicked()
{
    if (!QFile::exists(m_faceCascadePath)) {
        QMessageBox::critical(this, "Error", "Face detection model file is missing!");
        return;
    }

    if (validateFaceLogin()) {
        QMessageBox::information(this, "Success", "Face login successful!");
        this->close();

        // 打开主窗口（根据用户类型调整逻辑）
        MainWindow_toc *mainWindow = new MainWindow_toc("FaceUser", this);
        mainWindow->show();
    } else {
        QMessageBox::warning(this, "Fail", "Face login failed! No face detected or unauthorized face.");
    }
}

bool Login::validateFaceLogin()
{
    // 初始化摄像头
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        QMessageBox::critical(this, "Error", "Failed to open camera!");
        return false;
    }

    // 加载人脸检测器
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load(m_faceCascadePath.toStdString())) {
        QMessageBox::critical(this, "Error", "Failed to load face cascade!");
        return false;
    }

    cv::Mat frame;
    bool faceDetected = false;

    QMessageBox::information(this, "Info", "Look at the camera for face login.");

    // 开始捕捉并检测人脸
    for (int i = 0; i < 100; ++i) { // 尝试捕捉 100 帧
        camera >> frame;
        if (frame.empty())
            continue;

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(frame, faces, 1.1, 5);

        if (!faces.empty()) {
            faceDetected = true;
            break;
        }

        cv::imshow("Face Login", frame);
        if (cv::waitKey(30) == 27) { // 按下 ESC 退出
            break;
        }
    }

    camera.release();
    cv::destroyAllWindows();

    return faceDetected;
}
