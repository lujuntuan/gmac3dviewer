/*********************************************************************************
 *Copyright(C): Juntuan.Lu, 2020-2030, All rights reserved.
 *Author:  Juntuan.Lu
 *Version: 1.0
 *Date:  2021/10/23
 *Email: 931852884@qq.com
 *Description:
 *Others:
 *Function List:
 *History:
 **********************************************************************************/

#include "gosg/gosgcontrol.h"
#include "gosg/gosgrenderitem.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QSharedMemory>

static bool checkSingletonProcess(const QString& name)
{
#ifdef Q_OS_UNIX
    static QSharedMemory removeMem;
    removeMem.setKey(name);
    if (removeMem.attach(QSharedMemory::ReadOnly) == true) {
        removeMem.detach();
    }
#endif
    static QSharedMemory createMem;
    createMem.setKey(name);
    if (createMem.attach(QSharedMemory::ReadOnly) == true) {
        createMem.detach();
        QMessageBox::warning(nullptr, "Warning", "The program has started !", "Exit");
        return false;
    } else {
        if (createMem.create(1, QSharedMemory::ReadOnly) == false) {
            QMessageBox::warning(nullptr, "Warning", "Start singleton process check failed !", "Exit");
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    if (!checkSingletonProcess("_gmac_3d_viewer_memorycheck")) {
        return -1;
    }
    QDir::setCurrent(app.applicationDirPath());
#ifdef WIN32
    app.setFont(QFont(QStringLiteral("Microsoft YaHei UI")));
#endif
    qmlRegisterType<GOsgControl>("GOsg", 1, 0, "GOsgControl");
    qmlRegisterType<GOsgRenderItem>("GOsg", 1, 0, "GOsgRenderItem");
    QUrl mainUrl = QUrl::fromLocalFile("./qml/main.qml");
    if (!QFile::exists(mainUrl.toLocalFile())) {
        mainUrl = QUrl("qrc:/qml/main.qml");
    }
    auto qmlQuitFunction = [](QObject* obj, const QUrl& objUrl) -> void {
        if (!obj) {
            QMessageBox::warning(nullptr, "Warning", "Qml component creat failed !", "Exit");
            qApp->exit(-3);
        }
    };
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, qmlQuitFunction, Qt::QueuedConnection);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QApplication::quit, Qt::QueuedConnection);
    engine.load(mainUrl);
    return app.exec();
}
