#include "rspinoutsignals.h"
#include <QTimerEvent>
#include <QMetaEnum>
#include <QDebug>
#include <array>

static constexpr std::array<QSerialPort::PinoutSignal,9> RSPinout {
        QSerialPort::DataCarrierDetectSignal, //1
        QSerialPort::TransmittedDataSignal,//2
        QSerialPort::ReceivedDataSignal,//3
        QSerialPort::DataTerminalReadySignal,//4
        QSerialPort::NoSignal, //5
        QSerialPort::DataSetReadySignal, //6
        QSerialPort::ClearToSendSignal, //7
        QSerialPort::RequestToSendSignal, //8
        QSerialPort::RingIndicatorSignal  //9
};

RSPinoutSignals::RSPinoutSignals(QObject *parent) : QObject(parent)
{
}

void RSPinoutSignals::timerEvent(QTimerEvent *e)
{
    if( e->timerId() == m_timer_id ) {
        if(!m_port.isOpen()) return;

        QSerialPort::PinoutSignals pins = m_port.pinoutSignals();

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

//        QSerialPort::PinoutSignals ps =  m_port.pinoutSignals();
//        QMetaEnum en = QMetaEnum::fromType<QSerialPort::PinoutSignal>();

//        for(int i=0; i<en.keyCount(); ++i) {
//            if(ps.testFlag( QSerialPort::PinoutSignal(en.value(i)) ) )
//            {
//                qDebug() << en.valueToKeys()
//            }
//        }

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

void RSPinoutSignals::setPortName(QString portName)
{
    if (m_portName == portName)
        return;

    m_portName = portName;
    emit portNameChanged(m_portName);

    if(m_port.isOpen()) m_port.close();
    if(!m_portName.isEmpty()) {
        m_port.setPortName(m_portName);
        if(!m_port.open(QIODevice::ReadWrite)){
            qWarning() << "Cannot open port:" << m_port.errorString();
        }
    }
}
