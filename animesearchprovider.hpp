#ifndef ANIMESERCHPROVIDER_HPP
#define ANIMESERCHPROVIDER_HPP

#include "animelistmodel.hpp"
#include "animelistrequest.hpp"

class AnimeSearchProvider : public AnimeListRequest {
    Q_OBJECT
    Q_PROPERTY(QString keyWord READ keyWord WRITE setKeyWord NOTIFY keyWordChanged)
    Q_PROPERTY(
        QVector<int> typeIndices READ typeIndices WRITE setTypeIndices NOTIFY typeIndicesChanged)
    Q_PROPERTY(QVector<int> statusIndices READ statusIndices WRITE setStatusIndices NOTIFY
                   statusIndicesChanged)
    Q_PROPERTY(QVector<int> ratingIndices READ ratingIndices WRITE setRatingIndices NOTIFY
                   ratingIndicesChanged)
    Q_PROPERTY(QVector<int> genreIndices READ genreIndices WRITE setGenreIndices NOTIFY
                   genreIndicesChanged)
    Q_PROPERTY(int orderByIndex READ orderByIndex WRITE setOrderByIndex NOTIFY orderByIndexChanged)
    Q_PROPERTY(int sortIndex READ sortIndex WRITE setSortIndex NOTIFY sortIndexChanged)
    Q_PROPERTY(AnimeListModel *model READ model CONSTANT)

public:
    AnimeSearchProvider(QNetworkAccessManager *i, QObject *parent = nullptr);

    Q_INVOKABLE static QStringList types();
    Q_INVOKABLE static QStringList status();
    Q_INVOKABLE static QStringList rating();
    Q_INVOKABLE static QStringList orderBy();
    Q_INVOKABLE static QStringList sort();
    Q_INVOKABLE static QStringList genre();

    Q_INVOKABLE void nextPage();

    QString keyWord() const;
    AnimeListModel *model() const;
    QVector<int> typeIndices() const;
    QVector<int> statusIndices() const;
    QVector<int> ratingIndices() const;
    QVector<int> genreIndices() const;
    int orderByIndex() const;
    int sortIndex() const;

    static bool isValidKeyword(const QString &k) { return k.size() >= 3 || k.isEmpty(); }
    Q_INVOKABLE bool isValidKeyword() { return isValidKeyword(m_keyWord); }

public slots:
    void beginSearch();

    void setKeyWord(QString keyWord);
    void setTypeIndices(QVector<int> typeIndices);
    void setStatusIndices(QVector<int> statusIndices);
    void setRatingIndices(QVector<int> ratingIndices);
    void setGenreIndices(QVector<int> genreIndices);
    void setOrderByIndex(int orderByIndex);
    void setSortIndex(int sortIndex);

signals:
    void keyWordChanged(QString keyWord);
    void typeIndicesChanged(QVector<int> typeIndices);
    void statusIndicesChanged(QVector<int> statusIndices);
    void ratingIndicesChanged(QVector<int> ratingIndices);
    void genreIndicesChanged(QVector<int> genreIndices);
    void orderByIndexChanged(int orderByIndex);
    void sortIndexChanged(int sortIndex);

private:
    int m_page = 1;

    AnimeListModel *m_model;
    QString m_keyWord;
    QVector<int> m_typeIndices;
    QVector<int> m_statusIndices;
    QVector<int> m_ratingIndices;
    QVector<int> m_genreIndices;
    int m_orderByIndex;
    int m_sortIndex;

    QUrl createRequestUrl();

    // AnimeListRequest interface
protected:
    void addAnime(Anime anime) override;
    void parseJsonReply(const QJsonDocument &doc) override;
};

#endif // ANIMESERCHPROVIDER_HPP
