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

// Zmienna pliku logu, która będzie dostępna w całej aplikacji
static QFile logFile;

// Nasza niestandardowa funkcja do obsługi i przekierowywania logów
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Jeśli plik nie jest otwarty, otwórz go (stanie się to przy pierwszej wiadomości)
    if (!logFile.isOpen()) {
        QString logDirPath = QCoreApplication::applicationDirPath() + "/logs";
        QDir logDir(logDirPath);
        if (!logDir.exists()) {
            logDir.mkpath("."); // Utwórz katalog /logs jeśli nie istnieje
        }
        logFile.setFileName(logDirPath + "/session.log");
        // Otwórz plik w trybie zapisu, usuwając poprzednią zawartość (Truncate)
        if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            // Jeśli nie uda się otworzyć pliku, wypisz błąd na standardowe wyjście błędów
            std::cerr << "Fatal: Could not open log file: " << logFile.fileName().toStdString() << std::endl;
            return;
        }
    }

    // Przygotowanie sformatowanej wiadomości
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

    // Zapisz sformatowaną wiadomość do pliku
    QTextStream logStream(&logFile);
    logStream << logMessage << Qt::endl;

    // Opcjonalnie: Przekaż wiadomość również do oryginalnej konsoli (przydatne podczas debugowania)
    std::cout << logMessage.toStdString() << std::endl;

    // W przypadku błędu krytycznego (Fatal), przerwij aplikację
    if (type == QtFatalMsg) {
        abort();
    }
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Instalujemy naszą funkcję obsługi wiadomości na samym początku
    qInstallMessageHandler(messageHandler);

    qInfo() << "Application starting...";

    QQmlApplicationEngine engine;

    GameManager gameManager;

    engine.rootContext()->setContextProperty("gameManager", &gameManager);
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
