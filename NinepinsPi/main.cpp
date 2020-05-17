#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SerialComm.hpp"
#include "commprovider.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QGuiApplication app(argc, argv);
    //qmlRegisterType<SerialComm>("com.tylnesh.serialcomm", 1, 0, "SerialComm");
    qmlRegisterType<CommProvider>("com.tylnesh.commprovider", 1, 0, "CommProvider");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
