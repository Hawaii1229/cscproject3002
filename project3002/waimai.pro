QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 启用 C++17 标准
CONFIG += c++17

# 启用翻译功能
TRANSLATIONS += \
    waimai_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# 启用资源文件支持（如图标、图片等）
RESOURCES += resources.qrc

# 添加警告信息，禁用已弃用的 API
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# 源文件列表
SOURCES += \
    dishes.cpp \
    home_tob.cpp \
    income_tob.cpp \
    login.cpp \
    main.cpp \
    mainwindow_tob.cpp \
    mainwindow_toc.cpp \
    menu_tob.cpp \
    oneorder.cpp \
    order_tob.cpp \
    secondwindow_toc.cpp \
    signup.cpp \
    store.cpp \
    thirdwindow_toc.cpp

# 头文件列表
HEADERS += \
    dishes.h \
    home_tob.h \
    income_tob.h \
    login.h \
    mainwindow_tob.h \
    mainwindow_toc.h \
    menu_tob.h \
    oneorder.h \
    order_tob.h \
    secondwindow_toc.h \
    signup.h \
    store.h \
    thirdwindow_toc.h

# UI 文件列表
FORMS += \
    dishes.ui \
    home_tob.ui \
    income_tob.ui \
    login.ui \
    mainwindow_tob.ui \
    mainwindow_toc.ui \
    menu_tob.ui \
    oneorder.ui \
    order_tob.ui \
    secondwindow_toc.ui \
    signup.ui \
    store.ui \
    thirdwindow_toc.ui

# 项目路径设置
INCLUDEPATH += $$PWD

# 数据库和图表支持
QT += sql charts

# Windows 部署规则（根据具体环境设置）
win32: CONFIG(release, debug|release): DESTDIR = $$OUT_PWD/release
else: unix:!android: target.path = /opt/$${TARGET}/bin

# 如果包含资源文件
!isEmpty(RESOURCES): RESOURCES += resources.qrc

# 默认规则
!isEmpty(target.path): INSTALLS += target