#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtAndroid>

#include "MergeMagicDatabase.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<MergeMagicDatabase>("MergeMagicDatabase", 1, 0, "MergeMagicDatabase");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    auto result = QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));
    if (result == QtAndroid::PermissionResult::Denied) {
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({ "android.permission.WRITE_EXTERNAL_STORAGE" }));
        if (resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) {
            QCoreApplication::exit(-1);
        }
    }

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
