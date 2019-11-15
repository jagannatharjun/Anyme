#ifndef REQUESTSTATUS_HPP
#define REQUESTSTATUS_HPP

#include <QObject>

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

private:
    RequestStatus m_status = RequestStatus::Completed;
    QString m_errorString;

protected:
    void setStatus(RequestStatus status, const QString &err = {}) {
        if (m_status == status)
            return;
        m_status = status;

        if (status == RequestStatus::Error) {
            m_errorString = err;
        } else {
            Q_ASSERT(err.isEmpty());
            m_errorString = QString{};
        }

        emit errorStringChanged(err);
        emit statusChanged();
    }
};

#endif // REQUESTSTATUS_HPP
