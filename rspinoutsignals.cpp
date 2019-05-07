#include "rspinoutsignals.h"
#include <QTimerEvent>
#include <QMetaEnum>
#include <QDebug>

static constexpr std::array<QSerialPort::PinoutSignal,9> RSPinout {
        QSerialPort::DataCarrierDetectSignal, //1
        QSerialPort::NoSignal,//2
        QSerialPort::NoSignal,//3
        QSerialPort::DataTerminalReadySignal,//4
        QSerialPort::NoSignal, //5
        QSerialPort::DataSetReadySignal, //6
        QSerialPort::ClearToSendSignal, //7
        QSerialPort::RequestToSendSignal, //8
        QSerialPort::RingIndicatorSignal  //9
};

RSPinoutSignals::RSPinoutSignals(QObject *parent) : QSerialPort(parent)
{
}

void RSPinoutSignals::timerEvent(QTimerEvent *e)
{
    if( e->timerId() == m_timer_id ) {
        if(!isOpen()){
            if(!m_opened) return;
            m_opened=false;
            emit openStatus(false);
            return;
        }

        QSerialPort::PinoutSignals pins = pinoutSignals();

        for(size_t i=0; i<PinsCount; ++i ) {
            QSerialPort::PinoutSignal sig = RSPinout[i];
            if( sig == QSerialPort::NoSignal ) continue;
            if(pins.testFlag(sig)) {
                if(m_integrals[i] < m_intCount) {
                    ++m_integrals[i];
                    continue;
                }
                if(!m_states[i]) {
                    m_states[i] = true;
                    emit pinStatusChanged(int(i),true);
                }
            }
            else {
                if(m_integrals[i] > 0) {
                    --m_integrals[i];
                    continue;
                }
                if(m_states[i]) {
                    m_states[i] = false;
                    emit pinStatusChanged(int(i),false);
                }
            }
        }
    }
}

QVariant RSPinoutSignals::pinList() const
{
    QVariantList vl;
    for(size_t i=0; i<PinsCount; ++i ) {
        vl.push_back( QLatin1String(QMetaEnum::fromType<QSerialPort::PinoutSignal>().valueToKey(RSPinout[i])) );
    }
    return vl;
}

bool RSPinoutSignals::pinStatus(unsigned i)
{
    if(i>=PinsCount)return false;
    return m_states[i];
}

void RSPinoutSignals::setIntCount(int intCount)
{
    if (m_intCount == intCount)
        return;

    m_intCount = intCount;
    emit intCountChanged(m_intCount);
}

void RSPinoutSignals::setInterval(int interval)
{
    if (m_interval == interval)
        return;

    m_interval = interval;
    emit intervalChanged(m_interval);

    if(m_timer_id) killTimer(m_timer_id);
    m_timer_id = startTimer(m_interval);

}

void RSPinoutSignals::setupPortName(QString p_portName)
{
    if (portName() == p_portName)
        return;

    if(isOpen()) close();

    setPortName(p_portName);
    emit portNameChanged(p_portName);

    if(p_portName.isEmpty()) return;
    if(!open(QIODevice::ReadWrite)){
       qWarning() << "Cannot open port:" << errorString();
       if(!m_opened) return;
       m_opened=false;
       emit openStatus(false);
       return;
    }
    m_opened=true;
    emit openStatus(true);
}
