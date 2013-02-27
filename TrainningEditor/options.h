#ifndef OPTIONS_H
#define OPTIONS_H


enum SPEED_OPT{ kilometersPerHour, metersPerSecond };
enum ALT_OPT{ kilometers, meters };

class Options
{
private:
    SPEED_OPT speedOptins;
    ALT_OPT altOptions;

    bool speedOnMap;
    bool altOnMap;
    bool startTimeOnMap;

public:
    Options();
    SPEED_OPT getSpeedOptins();
    ALT_OPT getAltOptions();
    void setSpeedOptins( SPEED_OPT _speedOptins );
    void setAltOptions( ALT_OPT _altOptions );
    bool getSpeedOnMap();
    bool getAltOnMap();
    bool getStartTimeOnMap();
    void setSpeedOnMap( bool _speedOnMap);
    void setAltOnMap( bool _altOnMap);
    void setStartTimeOnMap(bool _startTimeOnMap );


};

#endif // OPTIONS_H
