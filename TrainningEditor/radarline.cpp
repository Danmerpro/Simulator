#include "radarline.h"

RadarLine::RadarLine()
{
    angle = 0;
    radius = 300;
 //   connect(&timer, SIGNAL(timeout()), this, SLOT(scan()));
    this->startTimer(1000/framesPerSec);
}

void RadarLine::scan()
{
    setLine(0,0,radius*cos(angle),radius*sin(angle));
    angle+=2*M_PI*(360/10)/(1000/framesPerSec)/180;
    if( angle > M_PI*2)
        angle = 0;
}

void RadarLine::timerEvent(QTimerEvent *)
{
    setLine(300,300,300+radius*cos(angle),300+radius*sin(angle));
    setPen(QPen(Qt::green));
    angle+=2*M_PI*(360/10)/(1000/framesPerSec)/180;
    if( angle > M_PI*2)
        angle = 0;
}
