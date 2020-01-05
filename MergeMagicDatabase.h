#ifndef MERGEMAGICDATABASE_H
#define MERGEMAGICDATABASE_H

#include <QJsonObject>
#include <QObject>
#include <QSqlDatabase>

class MergeMagicDatabase : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString lastError READ getLastError NOTIFY lastErrorChanged)
    Q_PROPERTY(int compasses READ getCompasses WRITE setCompasses NOTIFY compassesChanged)
    Q_PROPERTY(int coins READ getCoins WRITE setCoins NOTIFY coinsChanged)
    Q_PROPERTY(int woodLogs READ getWoodLogs WRITE setWoodLogs NOTIFY woodLogsChanged)
    Q_PROPERTY(int magicGems READ getMagicGems WRITE setMagicGems NOTIFY magicGemsChanged)
    Q_PROPERTY(bool premiumLandPurchased READ premiumLandPurchased WRITE setPremiumLandPurchased NOTIFY premiumLandPurchasedChanged)

public:
    explicit MergeMagicDatabase(QObject* parent = nullptr);
    virtual ~MergeMagicDatabase();

    // read only properties
    QString getLastError() const;

    int getCompasses() const;
    void setCompasses(int compasses);

    int getCoins() const;
    void setCoins(int coins);

    int getWoodLogs() const;
    void setWoodLogs(int woodLogs);

    int getMagicGems() const;
    void setMagicGems(int magicGems);

    bool premiumLandPurchased() const;
    void setPremiumLandPurchased(bool value);

public slots:

    void reload();
    void save();

signals:

    void lastErrorChanged();
    void compassesChanged();
    void coinsChanged();
    void woodLogsChanged();
    void magicGemsChanged();
    void premiumLandPurchasedChanged();

private:
    QString m_lastError;
    QSqlDatabase m_db;
    QJsonObject m_root;
};

#endif // MERGEMAGICDATABASE_H
