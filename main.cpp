#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GameManager.h"
#include "Card.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <iostream>
#include <QUrl>

static QFile logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!logFile.isOpen()) {
        QString logDirPath = QCoreApplication::applicationDirPath() + "/logs";
        QDir logDir(logDirPath);
        if (!logDir.exists()) {
            logDir.mkpath(".");
        }
        logFile.setFileName(logDirPath + "/session.txt");
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            std::cerr << "Fatal: Could not open log file: " << logFile.fileName().toStdString() << std::endl;
            return;
        }
    }

    QString logMessage;
    QTextStream out(&logMessage);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    switch (type) {
    case QtDebugMsg:
        out << " [DEBUG] ";
        break;
    case QtInfoMsg:
        out << " [INFO] ";
        break;
    case QtWarningMsg:
        out << " [WARNING] ";
        break;
    case QtCriticalMsg:
        out << " [CRITICAL] ";
        break;
    case QtFatalMsg:
        out << " [FATAL] ";
        break;
    }

    out << msg;
    if (context.file) {
        out << " (" << context.file << ":" << context.line << ", " << context.function << ")";
    }

    QTextStream logStream(&logFile);
    logStream << logMessage << Qt::endl;

    std::cout << logMessage.toStdString() << std::endl;

    if (type == QtFatalMsg) {
        abort();
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qInstallMessageHandler(messageHandler);

    qInfo() << "Application starting...";

    QQmlApplicationEngine engine;

    GameManager gameManager;
    engine.rootContext()->setContextProperty("gameManager", &gameManager);

    const QUrl appDirUrl = QUrl::fromLocalFile(QGuiApplication::applicationDirPath());
    engine.rootContext()->setContextProperty("appDirUrl", appDirUrl);

    // --- POCZĄTEK POPRAWKI ---
    // Rejestrujemy typ GameManager, aby QML znał jego enumy.
    // Robimy to jako "Uncreatable" (nie do tworzenia), bo mamy tylko jedną, globalną instancję.
    qmlRegisterUncreatableType<GameManager>("com.deckbuilderpro.game", 1, 0, "GameManager", "GameManager cannot be created in QML, use 'gameManager' context property.");
    // --- KONIEC POPRAWKI ---

    qmlRegisterType<Card>("com.deckbuilderpro.game", 1, 0, "Card");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    gameManager.prepareGame(2);

    qInfo() << "Application started successfully.";

    return app.exec();
}
