#ifndef __X68SOUND_GLOBAL_H__
#define __X68SOUND_GLOBAL_H__

#include "x68sound_config.h"

#if X68SOUND_ENABLE_PORTABLE_CODE
	#ifdef _WIN32
		#include <windows.h>
		#include <stdint.h>
	#else
		#include <stdlib.h>
		#include <stdint.h>
		typedef unsigned int UINT;
		typedef uint32_t DWORD;
	#endif
	#include <assert.h>
#else
	#include	<stdio.h>
	#include	<stdlib.h>
	#include	<math.h>
	#ifdef _WIN32
		#include	<windows.h>
		#include	<windowsx.h>
		#include	<conio.h>
	#endif
#endif
#include	<ctype.h>

#define	NODEBUG

//#define	DEBUG

#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	DebugValue=0;
int	ErrorCode=0;
#endif

#define	N_CH	8




//#define	BUNKAINOU	(0)
//#define	ADPCM_POLY

#define	PRECISION_BITS	(10)
#define	PRECISION	(1<<PRECISION_BITS)
#define	SIZEALPHATBL_BITS	(10)
#define	SIZEALPHATBL	(1<<SIZEALPHATBL_BITS)

#define	SIZESINTBL_BITS	(10)
#define	SIZESINTBL	(1<<SIZESINTBL_BITS)
#define	MAXSINVAL	(1<<(SIZESINTBL_BITS+2))

#define	PI	3.1415926535897932
//#define	MAXELVAL_BITS	(30)
//#define	MAXELVAL	(1<<MAXELVAL_BITS)
//#define	MAXARTIME_BITS	(20)
//#define	SIZEARTBL	(100)
//#define	MAXARTIME	(SIZEARTBL*(1<<MAXARTIME_BITS))


#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	Samprate = 22050;
int	WaveOutSamp = 22050;
int OpmWait = 240;	// 24.0μｓ
int	OpmRate = 62500;	// 入力クロック÷64
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	STEPTBL[11*12*64];
//int	STEPTBL3[11*12*64];
	#define	ALPHAZERO	(SIZEALPHATBL*3)
unsigned short	ALPHATBL[ALPHAZERO+SIZEALPHATBL+1];
short	SINTBL[SIZESINTBL];
#endif
#if X68SOUND_ENABLE_PORTABLE_CODE
static const int STEPTBL_O2[12*64] = {
#else
int STEPTBL_O2[12*64] = {
#endif
	1299,1300,1301,1302,1303,1304,1305,1306,
	1308,1309,1310,1311,1313,1314,1315,1316,
	1318,1319,1320,1321,1322,1323,1324,1325,
	1327,1328,1329,1330,1332,1333,1334,1335,
	1337,1338,1339,1340,1341,1342,1343,1344,
	1346,1347,1348,1349,1351,1352,1353,1354,
	1356,1357,1358,1359,1361,1362,1363,1364,
	1366,1367,1368,1369,1371,1372,1373,1374,
	1376,1377,1378,1379,1381,1382,1383,1384,
	1386,1387,1388,1389,1391,1392,1393,1394,
	1396,1397,1398,1399,1401,1402,1403,1404,
	1406,1407,1408,1409,1411,1412,1413,1414,
	1416,1417,1418,1419,1421,1422,1423,1424,
	1426,1427,1429,1430,1431,1432,1434,1435,
	1437,1438,1439,1440,1442,1443,1444,1445,
	1447,1448,1449,1450,1452,1453,1454,1455,
	1458,1459,1460,1461,1463,1464,1465,1466,
	1468,1469,1471,1472,1473,1474,1476,1477,
	1479,1480,1481,1482,1484,1485,1486,1487,
	1489,1490,1492,1493,1494,1495,1497,1498,
	1501,1502,1503,1504,1506,1507,1509,1510,
	1512,1513,1514,1515,1517,1518,1520,1521,
	1523,1524,1525,1526,1528,1529,1531,1532,
	1534,1535,1536,1537,1539,1540,1542,1543,
	1545,1546,1547,1548,1550,1551,1553,1554,
	1556,1557,1558,1559,1561,1562,1564,1565,
	1567,1568,1569,1570,1572,1573,1575,1576,
	1578,1579,1580,1581,1583,1584,1586,1587,
	1590,1591,1592,1593,1595,1596,1598,1599,
	1601,1602,1604,1605,1607,1608,1609,1610,
	1613,1614,1615,1616,1618,1619,1621,1622,
	1624,1625,1627,1628,1630,1631,1632,1633,
	1637,1638,1639,1640,1642,1643,1645,1646,
	1648,1649,1651,1652,1654,1655,1656,1657,
	1660,1661,1663,1664,1666,1667,1669,1670,
	1672,1673,1675,1676,1678,1679,1681,1682,
	1685,1686,1688,1689,1691,1692,1694,1695,
	1697,1698,1700,1701,1703,1704,1706,1707,
	1709,1710,1712,1713,1715,1716,1718,1719,
	1721,1722,1724,1725,1727,1728,1730,1731,
	1734,1735,1737,1738,1740,1741,1743,1744,
	1746,1748,1749,1751,1752,1754,1755,1757,
	1759,1760,1762,1763,1765,1766,1768,1769,
	1771,1773,1774,1776,1777,1779,1780,1782,
	1785,1786,1788,1789,1791,1793,1794,1796,
	1798,1799,1801,1802,1804,1806,1807,1809,
	1811,1812,1814,1815,1817,1819,1820,1822,
	1824,1825,1827,1828,1830,1832,1833,1835,
	1837,1838,1840,1841,1843,1845,1846,1848,
	1850,1851,1853,1854,1856,1858,1859,1861,
	1864,1865,1867,1868,1870,1872,1873,1875,
	1877,1879,1880,1882,1884,1885,1887,1888,
	1891,1892,1894,1895,1897,1899,1900,1902,
	1904,1906,1907,1909,1911,1912,1914,1915,
	1918,1919,1921,1923,1925,1926,1928,1930,
	1932,1933,1935,1937,1939,1940,1942,1944,
	1946,1947,1949,1951,1953,1954,1956,1958,
	1960,1961,1963,1965,1967,1968,1970,1972,
	1975,1976,1978,1980,1982,1983,1985,1987,
	1989,1990,1992,1994,1996,1997,1999,2001,
	2003,2004,2006,2008,2010,2011,2013,2015,
	2017,2019,2021,2022,2024,2026,2028,2029,
	2032,2033,2035,2037,2039,2041,2043,2044,
	2047,2048,2050,2052,2054,2056,2058,2059,
	2062,2063,2065,2067,2069,2071,2073,2074,
	2077,2078,2080,2082,2084,2086,2088,2089,
	2092,2093,2095,2097,2099,2101,2103,2104,
	2107,2108,2110,2112,2114,2116,2118,2119,
	2122,2123,2125,2127,2129,2131,2133,2134,
	2137,2139,2141,2142,2145,2146,2148,2150,
	2153,2154,2156,2158,2160,2162,2164,2165,
	2168,2170,2172,2173,2176,2177,2179,2181,
	2185,2186,2188,2190,2192,2194,2196,2197,
	2200,2202,2204,2205,2208,2209,2211,2213,
	2216,2218,2220,2222,2223,2226,2227,2230,
	2232,2234,2236,2238,2239,2242,2243,2246,
	2249,2251,2253,2255,2256,2259,2260,2263,
	2265,2267,2269,2271,2272,2275,2276,2279,
	2281,2283,2285,2287,2288,2291,2292,2295,
	2297,2299,2301,2303,2304,2307,2308,2311,
	2315,2317,2319,2321,2322,2325,2326,2329,
	2331,2333,2335,2337,2338,2341,2342,2345,
	2348,2350,2352,2354,2355,2358,2359,2362,
	2364,2366,2368,2370,2371,2374,2375,2378,
	2382,2384,2386,2388,2389,2392,2393,2396,
	2398,2400,2402,2404,2407,2410,2411,2414,
	2417,2419,2421,2423,2424,2427,2428,2431,
	2433,2435,2437,2439,2442,2445,2446,2449,
	2452,2454,2456,2458,2459,2462,2463,2466,
	2468,2470,2472,2474,2477,2480,2481,2484,
	2488,2490,2492,2494,2495,2498,2499,2502,
	2504,2506,2508,2510,2513,2516,2517,2520,
	2524,2526,2528,2530,2531,2534,2535,2538,
	2540,2542,2544,2546,2549,2552,2553,2556,
	2561,2563,2565,2567,2568,2571,2572,2575,
	2577,2579,2581,2583,2586,2589,2590,2593,
};
#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	D1LTBL[16];
int	DT1TBL[128+4];
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
static const int DT1TBL_org[128+4] = {
#else
int	DT1TBL_org[128+4] = {
#endif
	0, 0, 1, 2, 
	0, 0, 1, 2, 
	0, 0, 1, 2, 
	0, 0, 1, 2, 
	0, 1, 2, 2, 
	0, 1, 2, 3, 
	0, 1, 2, 3, 
	0, 1, 2, 3, 
	0, 1, 2, 4, 
	0, 1, 3, 4, 
	0, 1, 3, 4, 
	0, 1, 3, 5, 
	0, 2, 4, 5, 
	0, 2, 4, 6, 
	0, 2, 4, 6, 
	0, 2, 5, 7, 
	0, 2, 5, 8, 
	0, 3, 6, 8, 
	0, 3, 6, 9, 
	0, 3, 7, 10, 
	0, 4, 8, 11, 
	0, 4, 8, 12, 
	0, 4, 9, 13, 
	0, 5, 10, 14, 
	0, 5, 11, 16, 
	0, 6, 12, 17, 
	0, 6, 13, 19, 
	0, 7, 14, 20, 
	0, 8, 16, 22, 
	0, 8, 16, 22, 
	0, 8, 16, 22, 
	0, 8, 16, 22, 
};

typedef struct {
#if X68SOUND_ENABLE_PORTABLE_CODE
	int	m_and;
#else
	int	and;
#endif
	int	add;
}	XR_ELE;

#if X68SOUND_ENABLE_PORTABLE_CODE
static const XR_ELE XRTBL[64+32] = {
#else
XR_ELE XRTBL[64+32] = {
#endif
	{4095,8},
	{2047,5},{2047,6},{2047,7},{2047,8},
	{1023,5},{1023,6},{1023,7},{1023,8},
	{511,5},{511,6},{511,7},{511,8},
	{255,5},{255,6},{255,7},{255,8},
	{127,5},{127,6},{127,7},{127,8},
	{63,5},{63,6},{63,7},{63,8},
	{31,5},{31,6},{31,7},{31,8},
	{15,5},{15,6},{15,7},{15,8},
	{7,5},{7,6},{7,7},{7,8},
	{3,5},{3,6},{3,7},{3,8},
	{1,5},{1,6},{1,7},{1,8},
	{0,5},{0,6},{0,7},{0,8},
	{0,10},{0,12},{0,14},{0,16},
	{0,20},{0,24},{0,28},{0,32},
	{0,40},{0,48},{0,56},{0,64},
	{0,64},{0,64},{0,64},
	{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},
	{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},
	{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},
	{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},{0,64},
};


#if X68SOUND_ENABLE_PORTABLE_CODE
static const int DT2TBL[4] = { 0, 384, 500, 608 };
#else
int DT2TBL[4] = { 0, 384, 500, 608 };
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
unsigned short	NOISEALPHATBL[ALPHAZERO+SIZEALPHATBL+1];
#endif



















#if X68SOUND_ENABLE_PORTABLE_CODE
static const int dltLTBL[48+1]= {
#else
int dltLTBL[48+1]= {
#endif
	16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,
	73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,
	337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,
};
/*
int dltLTBL[48+7+1]= {
	16,17,19,21,23,25,28,31,34,37,41,45,50,55,60,66,
	73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,
	337,371,408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,
	1707,1878,2066,2272,2500,2750,3025
};
*/
#if X68SOUND_ENABLE_PORTABLE_CODE
static const int DCT[16]= {
#else
int DCT[16]= {
#endif
	-1,-1,-1,-1,2,4,6,8,
	-1,-1,-1,-1,2,4,6,8,
};


#if X68SOUND_ENABLE_PORTABLE_CODE
static const int ADPCMRATETBL[2][4] = {
	{2, 3, 4, 4},
	{0, 1, 2, 2},
#else
int ADPCMRATETBL[2][4] = {
	2, 3, 4, 4,
	0, 1, 2, 2,
#endif
};
#if X68SOUND_ENABLE_PORTABLE_CODE
static const int ADPCMRATEADDTBL[8] = {
#else
int	ADPCMRATEADDTBL[8] = {
#endif
	46875, 62500, 93750, 125000, 15625*12, 15625*12, 15625*12, 0,
};
#if X68SOUND_ENABLE_PORTABLE_CODE
static const int PCM8VOLTBL[16] = {
#else
int PCM8VOLTBL[16] = {
#endif
	2,3,4,5,6,8,10,12,16,20,24,32,40,48,64,80,
};

#define	PCM8_NCH	8


#if X68SOUND_ENABLE_PORTABLE_CODE
static inline uint32_t bswapl(uint32_t adrsOfs) {
	uint32_t tmp = adrsOfs;
	tmp = ((tmp & 0xFF) << 24) | ((tmp & 0xFF00) << 8) | ((tmp & 0xFF0000) >> 8) | ((tmp & 0xFF000000) >> 24);
	return tmp;
}
#else
unsigned char *bswapl(unsigned char *adrs) {
	__asm {
		mov	eax,adrs
		bswap	eax
	}
}
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
static inline unsigned short bswapw(unsigned short data) {
	uint16_t tmp = (uint16_t)data;
	tmp = ((tmp & 0xFF) << 8) | ((tmp & 0xFF00) >> 8);
	return tmp;
}
#else
unsigned short bswapw(unsigned short data) {
	__asm {
		mov ax,data
		ror ax,8
	}
}
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
static inline
int MemReadDefault(unsigned char *adrs) {
#else
int CALLBACK MemReadDefault(unsigned char *adrs) {
#endif
	return *adrs;
}
#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int (CALLBACK *MemRead)(unsigned char *adrs) = MemReadDefault;
#endif

#if X68SOUND_ENABLE_PORTABLE_CODE
/*
	x68sound_context.internal.h に移動
*/
#else
unsigned int irnd(void) {
	static unsigned int seed = 1;
	seed = seed * 1566083941UL + 1;
	return seed;
}
#endif



#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	TotalVolume;	// 音量 x/256

volatile static int Semapho=0;
volatile static int TimerSemapho=0;
#endif

#define	OPMLPF_COL	64

#define	OPMLPF_ROW_44	441
#if X68SOUND_ENABLE_PORTABLE_CODE
static const short OPMLOWPASS_44[OPMLPF_ROW_44][OPMLPF_COL] = {
#else
static double opmlowpass_dummy_44;	// 64bit境界合わせ
static short OPMLOWPASS_44[OPMLPF_ROW_44][OPMLPF_COL] = {
#endif
	#include "opmlowpass_44.dat"
};

#define	OPMLPF_ROW_48	96
#if X68SOUND_ENABLE_PORTABLE_CODE
static const short OPMLOWPASS_48[OPMLPF_ROW_48][OPMLPF_COL] = {
#else
static double opmlowpass_dummy_48;	// 64bit境界合わせ
static short OPMLOWPASS_48[OPMLPF_ROW_48][OPMLPF_COL] = {
#endif
	#include "opmlowpass_48.dat"
};

#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	OPMLPF_ROW = OPMLPF_ROW_44;
short	(*OPMLOWPASS)[OPMLPF_COL] = OPMLOWPASS_44;
#endif


#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
int	Betw_Time;		// 5 ms
int	Late_Time;		// (200+Bet_time) ms
int	Late_Samples;	// (44100*Late_Time/1000)
int Blk_Samples;	// 44100/N_waveblk
int	Betw_Samples_Slower;	// floor(44100.0*5/1000.0-rev)
int	Betw_Samples_Faster;	// ceil(44100.0*5/1000.0+rev)
int	Betw_Samples_VerySlower;	// floor(44100.0*5/1000.0-rev)/4.0
int Slower_Limit,Faster_Limit;
HWAVEOUT	hwo=NULL;
LPWAVEHDR	lpwh=NULL;
int	N_wavehdr=0;
WAVEFORMATEX	wfx;
unsigned int	TimerResolution=1;
//unsigned int	SamplesCounter=0;
//unsigned int	SamplesCounterRev=0;
unsigned int	nSamples;

HANDLE	thread_handle=NULL;
DWORD	thread_id=0;
volatile int thread_flag=0;
volatile int timer_start_flag=0;
//const int N_waveblk=8;
const int N_waveblk=4;
int waveblk=0;
int playingblk=0, playingblk_next=1;
volatile int	setPcmBufPtr=-1;
#endif

#if !X68SOUND_ENABLE_PORTABLE_CODE
	#define	THREADMES_WAVEOUTDONE	(WM_USER+1)
	#define	THREADMES_KILL	(WM_USER+2)

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, 
						  DWORD dwParam1, DWORD dwParam2);
DWORD WINAPI waveOutThread( LPVOID );
void CALLBACK OpmTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
#endif






#if X68SOUND_ENABLE_PORTABLE_CODE
static inline
void OpmFir_Normal(const short *p, const short *buf0, const short *buf1, int *result) {
#else
void OpmFir_Normal(short *p, short *buf0, short *buf1, int *result) {
#endif
			result[0] =  (int)buf0[0]*p[0]
						+(int)buf0[1]*p[1]
						+(int)buf0[2]*p[2]
						+(int)buf0[3]*p[3]
						+(int)buf0[4]*p[4]
						+(int)buf0[5]*p[5]
						+(int)buf0[6]*p[6]
						+(int)buf0[7]*p[7]
						+(int)buf0[8]*p[8]
						+(int)buf0[9]*p[9]
						+(int)buf0[10]*p[10]
						+(int)buf0[11]*p[11]
						+(int)buf0[12]*p[12]
						+(int)buf0[13]*p[13]
						+(int)buf0[14]*p[14]
						+(int)buf0[15]*p[15]
						+(int)buf0[16]*p[16]
						+(int)buf0[17]*p[17]
						+(int)buf0[18]*p[18]
						+(int)buf0[19]*p[19]
						+(int)buf0[20]*p[20]
						+(int)buf0[21]*p[21]
						+(int)buf0[22]*p[22]
						+(int)buf0[23]*p[23]
						+(int)buf0[24]*p[24]
						+(int)buf0[25]*p[25]
						+(int)buf0[26]*p[26]
						+(int)buf0[27]*p[27]
						+(int)buf0[28]*p[28]
						+(int)buf0[29]*p[29]
						+(int)buf0[30]*p[30]
						+(int)buf0[31]*p[31]
						+(int)buf0[32]*p[32]
						+(int)buf0[33]*p[33]
						+(int)buf0[34]*p[34]
						+(int)buf0[35]*p[35]
						+(int)buf0[36]*p[36]
						+(int)buf0[37]*p[37]
						+(int)buf0[38]*p[38]
						+(int)buf0[39]*p[39]
						+(int)buf0[40]*p[40]
						+(int)buf0[41]*p[41]
						+(int)buf0[42]*p[42]
						+(int)buf0[43]*p[43]
						+(int)buf0[44]*p[44]
						+(int)buf0[45]*p[45]
						+(int)buf0[46]*p[46]
						+(int)buf0[47]*p[47]
						+(int)buf0[48]*p[48]
						+(int)buf0[49]*p[49]
						+(int)buf0[50]*p[50]
						+(int)buf0[51]*p[51]
						+(int)buf0[52]*p[52]
						+(int)buf0[53]*p[53]
						+(int)buf0[54]*p[54]
						+(int)buf0[55]*p[55]
						+(int)buf0[56]*p[56]
						+(int)buf0[57]*p[57]
						+(int)buf0[58]*p[58]
						+(int)buf0[59]*p[59]
						+(int)buf0[60]*p[60]
						+(int)buf0[61]*p[61]
						+(int)buf0[62]*p[62]
						+(int)buf0[63]*p[63];
			result[0] >>= (16-1);
			result[1] =  (int)buf1[0]*p[0]
						+(int)buf1[1]*p[1]
						+(int)buf1[2]*p[2]
						+(int)buf1[3]*p[3]
						+(int)buf1[4]*p[4]
						+(int)buf1[5]*p[5]
						+(int)buf1[6]*p[6]
						+(int)buf1[7]*p[7]
						+(int)buf1[8]*p[8]
						+(int)buf1[9]*p[9]
						+(int)buf1[10]*p[10]
						+(int)buf1[11]*p[11]
						+(int)buf1[12]*p[12]
						+(int)buf1[13]*p[13]
						+(int)buf1[14]*p[14]
						+(int)buf1[15]*p[15]
						+(int)buf1[16]*p[16]
						+(int)buf1[17]*p[17]
						+(int)buf1[18]*p[18]
						+(int)buf1[19]*p[19]
						+(int)buf1[20]*p[20]
						+(int)buf1[21]*p[21]
						+(int)buf1[22]*p[22]
						+(int)buf1[23]*p[23]
						+(int)buf1[24]*p[24]
						+(int)buf1[25]*p[25]
						+(int)buf1[26]*p[26]
						+(int)buf1[27]*p[27]
						+(int)buf1[28]*p[28]
						+(int)buf1[29]*p[29]
						+(int)buf1[30]*p[30]
						+(int)buf1[31]*p[31]
						+(int)buf1[32]*p[32]
						+(int)buf1[33]*p[33]
						+(int)buf1[34]*p[34]
						+(int)buf1[35]*p[35]
						+(int)buf1[36]*p[36]
						+(int)buf1[37]*p[37]
						+(int)buf1[38]*p[38]
						+(int)buf1[39]*p[39]
						+(int)buf1[40]*p[40]
						+(int)buf1[41]*p[41]
						+(int)buf1[42]*p[42]
						+(int)buf1[43]*p[43]
						+(int)buf1[44]*p[44]
						+(int)buf1[45]*p[45]
						+(int)buf1[46]*p[46]
						+(int)buf1[47]*p[47]
						+(int)buf1[48]*p[48]
						+(int)buf1[49]*p[49]
						+(int)buf1[50]*p[50]
						+(int)buf1[51]*p[51]
						+(int)buf1[52]*p[52]
						+(int)buf1[53]*p[53]
						+(int)buf1[54]*p[54]
						+(int)buf1[55]*p[55]
						+(int)buf1[56]*p[56]
						+(int)buf1[57]*p[57]
						+(int)buf1[58]*p[58]
						+(int)buf1[59]*p[59]
						+(int)buf1[60]*p[60]
						+(int)buf1[61]*p[61]
						+(int)buf1[62]*p[62]
						+(int)buf1[63]*p[63];
			result[1] >>= (16-1);
}
/*
void OpmFir_MMX(short *p, short *buf0, short *buf1, int *result) {
	__asm {
		mov		ebx,p
		mov		ecx,buf0
		mov		edx,buf1

		movq	mm0,[ebx+8*0];
		movq	mm1,[ecx+8*0];
		movq	mm2,[edx+8*0];
		pmaddwd	mm1,mm0
		pmaddwd	mm2,mm0

		movq	mm0,[ebx+8*1];
		movq	mm3,[ecx+8*1];
		movq	mm4,[edx+8*1];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*2];
		movq	mm5,[ecx+8*2];
		movq	mm6,[edx+8*2];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*3];
		movq	mm3,[ecx+8*3];
		movq	mm4,[edx+8*3];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*4];
		movq	mm5,[ecx+8*4];
		movq	mm6,[edx+8*4];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*5];
		movq	mm3,[ecx+8*5];
		movq	mm4,[edx+8*5];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*6];
		movq	mm5,[ecx+8*6];
		movq	mm6,[edx+8*6];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*7];
		movq	mm3,[ecx+8*7];
		movq	mm4,[edx+8*7];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*8];
		movq	mm5,[ecx+8*8];
		movq	mm6,[edx+8*8];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*9];
		movq	mm3,[ecx+8*9];
		movq	mm4,[edx+8*9];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*10];
		movq	mm5,[ecx+8*10];
		movq	mm6,[edx+8*10];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*11];
		movq	mm3,[ecx+8*11];
		movq	mm4,[edx+8*11];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*12];
		movq	mm5,[ecx+8*12];
		movq	mm6,[edx+8*12];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*13];
		movq	mm3,[ecx+8*13];
		movq	mm4,[edx+8*13];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		movq	mm0,[ebx+8*14];
		movq	mm5,[ecx+8*14];
		movq	mm6,[edx+8*14];
		pmaddwd	mm5,mm0
		pmaddwd	mm6,mm0
		paddd	mm1,mm5
		paddd	mm2,mm6

		movq	mm0,[ebx+8*15];
		movq	mm3,[ecx+8*15];
		movq	mm4,[edx+8*15];
		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
		paddd	mm1,mm3
		paddd	mm2,mm4

		mov		ecx,result
		
		movd	eax,mm1
		psrlq	mm1,32
		movd	ebx,mm1
		add		eax,ebx
		sar		eax,16-1
		mov		[ecx+0],eax

		movd	eax,mm2
		psrlq	mm2,32
		movd	ebx,mm2
		add		eax,ebx
		sar		eax,16-1
		mov		[ecx+4],eax

		emms
	}
}
*/
#if !X68SOUND_ENABLE_PORTABLE_CODE
void OpmFir_MMX(short *p, short *buf0, short *buf1, int *result) {
	__asm {
		mov		ebx,p
		mov		ecx,buf0
		mov		edx,buf1

		movq	mm0,[ebx+8*0];
		movq	mm1,[ecx+8*0];
		movq	mm2,[edx+8*0];
		pmaddwd	mm1,mm0
		movq	mm3,[ecx+8*1];
		pmaddwd	mm2,mm0
		movq	mm0,[ebx+8*1];

		movq	mm4,[edx+8*1];
		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*2];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*2];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*2];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*3];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*3];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*3];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*4];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*4];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*4];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*5];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*5];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*5];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*6];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*6];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*6];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*7];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*7];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*7];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*8];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*8];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*8];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*9];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*9];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*9];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*10];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*10];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*10];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*11];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*11];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*11];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*12];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*12];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*12];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*13];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*13];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*13];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		movq	mm5,[ecx+8*14];
		pmaddwd	mm4,mm0
		movq	mm6,[edx+8*14];
		paddd	mm1,mm3
		movq	mm0,[ebx+8*14];
		paddd	mm2,mm4

		pmaddwd	mm5,mm0
		movq	mm3,[ecx+8*15];
		pmaddwd	mm6,mm0
		movq	mm4,[edx+8*15];
		paddd	mm1,mm5
		movq	mm0,[ebx+8*15];
		paddd	mm2,mm6

		pmaddwd	mm3,mm0
		pmaddwd	mm4,mm0
	
		mov		edi,result

		paddd	mm1,mm3
		paddd	mm2,mm4

		movd	eax,mm1
		psrlq	mm1,32
		movd	ecx,mm2
		psrlq	mm2,32
		movd	ebx,mm1
		movd	edx,mm2
		add		eax,ebx
		add		ecx,edx
		sar		eax,16-1
		sar		ecx,16-1
		mov		[edi+0],eax

		mov		[edi+4],ecx

		emms
	}
}
#endif


#if X68SOUND_ENABLE_PORTABLE_CODE
	/* X68SoundContext に移動 */
#else
void	(*OpmFir)(short *, short *, short *, int *) = OpmFir_Normal;
#endif

#if !X68SOUND_ENABLE_PORTABLE_CODE
void DetectMMX() {
	int flag;
	__asm {
		mov eax,1
		cpuid
		and edx,0x00800000
		mov flag,edx
	}
	if (flag) {
		OpmFir = OpmFir_MMX;
	} else {
		OpmFir = OpmFir_Normal;
	}
}
#endif

#endif //__X68SOUND_GLOBAL_H__
