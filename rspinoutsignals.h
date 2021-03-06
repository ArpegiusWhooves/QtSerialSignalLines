#ifndef RSPINOUTSIGNALS_H
#define RSPINOUTSIGNALS_H

#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QtSerialPort/QSerialPort>
#include <array>
#include <QVariant>

class RSPinoutSignals : public QSerialPort
{
    Q_OBJECT
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)
    Q_PROPERTY(int intCount READ intCount WRITE setIntCount NOTIFY intCountChanged)
    Q_PROPERTY(QVariant pinList READ pinList CONSTANT)
    Q_PROPERTY(QString portName READ portName WRITE setupPortName NOTIFY portNameChanged)
    Q_PROPERTY(bool opened READ opened WRITE setOpened NOTIFY openedChanged)

public:

    static const int PinsCount = 9;

    explicit RSPinoutSignals(QObject *parent = nullptr);

    void timerEvent(QTimerEvent* e) override;

    int intCount() const
    {
        return m_intCount;
    }

    int interval() const
    {
        return m_interval;
    }

    QVariant pinList() const;

    Q_INVOKABLE bool pinStatus(unsigned i);

    bool opened() const
    {
        return m_opened;
    }

signals:

    void intCountChanged(int intCount);

    void intervalChanged(int interval);

    void portNameChanged(QString portName);

    void pinStatusChanged(int pin, bool state);

    void openedChanged(bool opened);

public slots:

    void setIntCount(int intCount);

    void setInterval(int interval);

    void setupPortName(QString portName);

    void setOpened(bool opened)
    {
        if (m_opened == opened)
            return;

        m_opened = opened;
        emit openedChanged(m_opened);
    }

private:
    int m_timer_id = 0;
    int m_intCount = 0;
    int m_interval = 0;
    std::array<int,PinsCount> m_integrals{};
    std::array<bool,PinsCount> m_states{};
    bool m_opened = false;
};

#endif // RSPINOUTSIGNALS_H
