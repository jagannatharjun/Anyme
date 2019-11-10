#include "anime.hpp"

int Anime::malId() const { return m_malId; }

int Anime::rand() const { return m_rank; }

int Anime::rank() const { return m_rank; }

QString Anime::title() const { return m_title; }

QUrl Anime::url() const { return m_url; }

QUrl Anime::imageUrl() const { return m_imageUrl; }

QString Anime::type() const { return m_type; }

int Anime::episodes() const { return m_episodes; }

QDate Anime::startDate() const { return m_startDate; }

QDate Anime::endDate() const { return m_endDate; }

int Anime::members() const { return m_members; }

double Anime::score() const { return m_score; }

void Anime::setMalId(int malId) { m_malId = malId; }

void Anime::setRank(int rank) { m_rank = rank; }

void Anime::setTitle(QString title) { m_title = std::move(title); }

void Anime::setUrl(QUrl url) { m_url = std::move(url); }

void Anime::setImageurl(QUrl imageUrl) { m_imageUrl = std::move(imageUrl); }

void Anime::setType(QString type) { m_type = std::move(type); }

void Anime::setEpisodes(int episodes) { m_episodes = episodes; }

void Anime::setStartdate(QDate startDate) { m_startDate = startDate; }

void Anime::setEnddate(QDate endDate) { m_endDate = endDate; }

void Anime::setMembers(int members) { m_members = members; }

void Anime::setScore(double score) { m_score = score; }
