#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SerialComm.hpp"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QGuiApplication app(argc, argv);
    qmlRegisterType<SerialComm>("com.tylnesh.serialcomm", 1, 0, "SerialComm");


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
