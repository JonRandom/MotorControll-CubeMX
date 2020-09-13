
#define propgain 1024
#define intgain 16
#define acclim 5
#define declim 5
#define polepairs 5

#define alignmentdc 100  
#define rampupdc 500



#define zcthreshold 200 // 50
//#define rampuprate 1600
#define alignmenttime 10000
#define demagallowance 64 // 0-255 => 0 to 255/256 of a step time
#define holdrpm 65
#define holdtime 2000
#define startuprpmpersecond 300
#define overloadseconds 10
#define overloadsecondsreset 10
#define continuouscurrent 5000 // continuous current in milliamps


#define   minstep (200000/polepairs/holdrpm)
#define intclampscaler (1024/intgain)
#define rampuprate (startuprpmpersecond * polepairs)
#define overloaduprate   (1000/overloadseconds)
#define overloaddownrate (1000/overloadsecondsreset)
#define overloadthreshold (continuouscurrent*2/5)
#define overloadsetpoint overloadthreshold/4
