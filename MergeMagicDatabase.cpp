#include "MergeMagicDatabase.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

class ScopedDBConnection {
public:
    ScopedDBConnection(QSqlDatabase& db)
        : m_db(db)
    {
    }

    ~ScopedDBConnection()
    {
        m_db.close();
    }

    bool open()
    {
        return m_db.open();
    }

private:
    QSqlDatabase& m_db;
};

MergeMagicDatabase::MergeMagicDatabase(QObject* parent)
    : QObject(parent)
    , m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    m_db.setDatabaseName("/storage/emulated/0/Android/data/com.gramgames.mergemagic/files/mm_db.db");

    if (!m_db.isValid()) {
        m_lastError = "QSQLITE is not available on this platform.";
        return;
    }

    reload();
}

MergeMagicDatabase::~MergeMagicDatabase()
{
    QSqlDatabase::removeDatabase("QSQLITE");
}

QString MergeMagicDatabase::getLastError() const
{
    return m_lastError;
}

int MergeMagicDatabase::getCompasses() const
{
    if (m_root.contains("7")) {
        return m_root["7"].toInt();
    }
    return 0;
}

void MergeMagicDatabase::setCompasses(int value)
{
    if (m_root.contains("7") && m_root["7"].toInt() != value) {
        m_root["7"] = value;
        magicGemsChanged();
    }
}

int MergeMagicDatabase::getCoins() const
{
    if (m_root.contains("8")) {
        return m_root["8"].toInt();
    }
    return 0;
}

void MergeMagicDatabase::setCoins(int value)
{
    if (m_root.contains("8") && m_root["8"].toInt() != value) {
        m_root["8"] = value;
        coinsChanged();
    }
}

int MergeMagicDatabase::getWoodLogs() const
{
    if (m_root.contains("9")) {
        return m_root["9"].toInt();
    }
    return 0;
}

void MergeMagicDatabase::setWoodLogs(int value)
{
    if (m_root.contains("9") && m_root["9"].toInt() != value) {
        m_root["9"] = value;
        woodLogsChanged();
    }
}

int MergeMagicDatabase::getMagicGems() const
{
    if (m_root.contains("10")) {
        return m_root["10"].toInt();
    }
    return 0;
}

void MergeMagicDatabase::setMagicGems(int value)
{
    if (m_root.contains("10") && m_root["10"].toInt() != value) {
        m_root["10"] = value;
        magicGemsChanged();
    }
}

bool MergeMagicDatabase::premiumLandPurchased() const
{
    if (m_root.contains("55") && m_root["55"].isObject()) {
        QJsonObject object = m_root["55"].toObject();
        if (object.contains("3") && object.contains("9") && object.contains("10")) {
            return object["3"].toBool() && object["9"].toBool() && object["10"].toBool();
        }
    }
    return false;
}

void MergeMagicDatabase::setPremiumLandPurchased(bool value)
{
    if (m_root.contains("55") && m_root["55"].isObject()) {
        QJsonObject object = m_root["55"].toObject();
        if (object.contains("3") && object.contains("9") && object.contains("10")) {
            if (object["3"].toBool() != value || object["9"].toBool() != value || object["10"].toBool() != value) {
                object["3"] = value;
                object["9"] = value;
                object["10"] = value;
                m_root["55"] = object;
                premiumLandPurchasedChanged();
            }
        }
    }
}

void MergeMagicDatabase::reload()
{
    ScopedDBConnection db(m_db);
    bool ok = db.open();

    if (!ok) {
        m_lastError = m_db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("SELECT data FROM storage WHERE file = '_playerData'");

    if (!query.next()) {
        m_lastError = query.lastError().text();
        lastErrorChanged();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(query.value(0).toByteArray());
    if (!document.isObject()) {
        m_lastError = "Invalid JSON document found in database";
        lastErrorChanged();
        return;
    }

    m_root = document.object();

    compassesChanged();
    coinsChanged();
    woodLogsChanged();
    magicGemsChanged();
    premiumLandPurchasedChanged();
}

void MergeMagicDatabase::save()
{
    ScopedDBConnection db(m_db);
    bool ok = db.open();
    if (!ok) {
        m_lastError = m_db.lastError().text();
        return;
    }

    QJsonDocument saveDoc(m_root);

    std::string dataString = saveDoc.toJson(QJsonDocument::Compact).toStdString();

    QSqlQuery query;
    query.exec(QString("UPDATE storage SET data = '%1' WHERE file = '_playerData'").arg(dataString.c_str()));
}
