#include "options.h"

Options::Options()
{
    speedOptins = kilometersPerHour;
    altOptions = meters;

    speedOnMap = true;
    altOnMap = true;
    startTimeOnMap = true;
}

SPEED_OPT Options::getSpeedOptins()
{
    return speedOptins;
}

ALT_OPT Options::getAltOptions()
{
    return altOptions;
}

void Options::setSpeedOptins( SPEED_OPT _speedOptins )
{
    speedOptins = _speedOptins;
}

void Options::setAltOptions( ALT_OPT _altOptions )
{
    altOptions = _altOptions;
}

bool Options::getSpeedOnMap()
{
    return speedOnMap;
}

bool Options::getAltOnMap()
{
    return altOnMap;
}

bool Options::getStartTimeOnMap()
{
    return startTimeOnMap;
}

void Options::setSpeedOnMap( bool _speedOnMap)
{
    speedOnMap = _speedOnMap;
}

void Options::setAltOnMap( bool _altOnMap)
{
    altOnMap = _altOnMap;
}

void Options::setStartTimeOnMap(bool _startTimeOnMap )
{
    startTimeOnMap = _startTimeOnMap;
}
