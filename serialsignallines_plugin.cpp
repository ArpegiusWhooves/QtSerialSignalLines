#include "serialsignallines_plugin.h"
#include "rspinoutsignals.h"

#include <qqml.h>

void SerialSignalLinesPlugin::registerTypes(const char *uri)
{
    // @uri com.github.arpegiuswhooves
    qmlRegisterType<RSPinoutSignals>(uri, 1, 0, "RSPinoutSignals");
}

