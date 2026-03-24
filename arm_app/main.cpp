#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "prucontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<PruController>("PRU", 1, 0, "PruController");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}