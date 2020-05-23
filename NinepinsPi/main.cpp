#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "SerialComm.hpp"
#include "commprovider.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);


    QGuiApplication app(argc, argv);
    qmlRegisterSingletonType<CommProvider>("com.tylnesh.commprovider", 1, 0, "CommProvider",
                             [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        CommProvider *comm = new CommProvider();
        return comm;
    });
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
