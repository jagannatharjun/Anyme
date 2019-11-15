#ifndef REQUESTSTATUS_HPP
#define REQUESTSTATUS_HPP

#include <QObject>

#include <QUrl>


class AnimeRequest : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isLoading READ isLoading NOTIFY statusChanged)
    Q_PROPERTY(bool isError READ isError NOTIFY statusChanged)
    Q_PROPERTY(bool isCompleted READ isCompleted NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)

public:
    AnimeRequest(QObject *parent) : QObject(parent) {}

    enum RequestStatus { InProgress, Completed, Error };

    bool isLoading() const { return m_status == InProgress; }
    bool isError() const { return m_status == Error; }
    bool isCompleted() const { return m_status == Completed; }
    QString errorString() const { return m_errorString; }

signals:
    void statusChanged();
    void errorStringChanged(QString errorString);
    void completed();
    void errored();

private:
    RequestStatus m_status = RequestStatus::Completed;
    QString m_errorString;

protected slots:
    virtual void parseNetworkReply(class QNetworkReply *) = 0;

protected:
    void setStatus(RequestStatus status, const QString &err = {}) {
        if (m_status == status)
            return;
        m_status = status;

        if (status == RequestStatus::Completed) {
            emit completed();
        } else if (status == RequestStatus::Error) {
            m_errorString = err;
            emit errored();
        } else {
            Q_ASSERT(err.isEmpty());
            m_errorString = QString{};
        }

        emit errorStringChanged(err);
        emit statusChanged();
    }
};

#endif // REQUESTSTATUS_HPP
