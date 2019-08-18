/*
 * @file mazda_miata_1_6.cpp
 *
 *
 * https://github.com/rusefi/rusefi_documentation/wiki/Frankenso_MazdaMiataNA6_pnp
 *
 *
 * Frankenso board
 * set engine_type 41
 *
 * @date Apr 16, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_template.h"
#include "custom_engine.h"
#include "fsio_impl.h"
#include "thermistors.h"
#include "mazda_miata_1_6.h"

EXTERN_CONFIG;

// todo:
// extern const float ve16RpmBins[FUEL_RPM_COUNT];


static const float ve16RpmBins[FUEL_RPM_COUNT] = {
650.0,1100.0,1550.0,2000.0,
2450.0,2900.0,3350.0,3800.0,
4250.0,4700.0,5150.0,5650.0,
6100.0,6550.0,7000.0,7450.0};

static const float ve16LoadBins[FUEL_LOAD_COUNT] = {
        30.0, 35.0, 40.0, 45.0,
        50.0,         60.0,        80.0,        100.0,
        120.0,        140.0,        150.0,        160.0,
        170.0,         180.0,        190.0,        200.0};

#if DEFAULT_FUEL_LOAD_COUNT == FUEL_LOAD_COUNT
static const fuel_table_t mapBased16VeTable = {
		/* Generated by TS2C on Sun Oct 02 19:40:18 EDT 2016*/
		{/* 0 30.000	*//* 0 650.0*/23.348,	/* 1 1100.0*/32.544,	/* 2 1550.0*/50.000,	/* 3 2000.0*/57.000,	/* 4 2450.0*/53.000,	/* 5 2900.0*/53.000,	/* 6 3350.0*/57.000,	/* 7 3800.0*/59.000,	/* 8 4250.0*/59.000,	/* 9 4700.0*/59.000,	/* 10 5150.0*/59.000,	/* 11 5650.0*/61.000,	/* 12 6100.0*/65.000,	/* 13 6550.0*/65.000,	/* 14 7000.0*/65.000,	/* 15 7450.0*/65.000,	},
		{/* 1 35.000	*//* 0 650.0*/31.877,	/* 1 1100.0*/36.888,	/* 2 1550.0*/50.000,	/* 3 2000.0*/57.000,	/* 4 2450.0*/53.000,	/* 5 2900.0*/53.000,	/* 6 3350.0*/57.000,	/* 7 3800.0*/59.000,	/* 8 4250.0*/59.000,	/* 9 4700.0*/59.000,	/* 10 5150.0*/59.000,	/* 11 5650.0*/61.000,	/* 12 6100.0*/65.000,	/* 13 6550.0*/65.000,	/* 14 7000.0*/65.000,	/* 15 7450.0*/65.000,	},
		{/* 2 40.000	*//* 0 650.0*/40.406,	/* 1 1100.0*/41.232,	/* 2 1550.0*/50.452,	/* 3 2000.0*/57.000,	/* 4 2450.0*/53.847,	/* 5 2900.0*/53.909,	/* 6 3350.0*/57.900,	/* 7 3800.0*/56.264,	/* 8 4250.0*/57.443,	/* 9 4700.0*/58.621,	/* 10 5150.0*/59.799,	/* 11 5650.0*/61.000,	/* 12 6100.0*/62.285,	/* 13 6550.0*/63.463,	/* 14 7000.0*/64.642,	/* 15 7450.0*/65.818,	},
		{/* 3 45.000	*//* 0 650.0*/39.632,	/* 1 1100.0*/41.232,	/* 2 1550.0*/46.203,	/* 3 2000.0*/55.525,	/* 4 2450.0*/49.387,	/* 5 2900.0*/51.656,	/* 6 3350.0*/53.514,	/* 7 3800.0*/56.264,	/* 8 4250.0*/57.443,	/* 9 4700.0*/57.728,	/* 10 5150.0*/59.799,	/* 11 5650.0*/61.107,	/* 12 6100.0*/61.252,	/* 13 6550.0*/62.515,	/* 14 7000.0*/63.530,	/* 15 7450.0*/64.545,	},
		{/* 4 50.000	*//* 0 650.0*/38.858,	/* 1 1100.0*/42.271,	/* 2 1550.0*/41.954,	/* 3 2000.0*/53.756,	/* 4 2450.0*/44.928,	/* 5 2900.0*/49.404,	/* 6 3350.0*/49.128,	/* 7 3800.0*/56.362,	/* 8 4250.0*/57.214,	/* 9 4700.0*/56.834,	/* 10 5150.0*/58.917,	/* 11 5650.0*/59.863,	/* 12 6100.0*/60.221,	/* 13 6550.0*/61.566,	/* 14 7000.0*/62.417,	/* 15 7450.0*/63.270,	},
		{/* 5 60.000	*//* 0 650.0*/49.634,	/* 1 1100.0*/42.053,	/* 2 1550.0*/49.601,	/* 3 2000.0*/47.654,	/* 4 2450.0*/52.109,	/* 5 2900.0*/48.840,	/* 6 3350.0*/53.876,	/* 7 3800.0*/55.312,	/* 8 4250.0*/52.585,	/* 9 4700.0*/54.218,	/* 10 5150.0*/58.088,	/* 11 5650.0*/58.660,	/* 12 6100.0*/59.175,	/* 13 6550.0*/59.690,	/* 14 7000.0*/60.206,	/* 15 7450.0*/60.720,	},
		{/* 6 80.000	*//* 0 650.0*/45.650,	/* 1 1100.0*/52.751,	/* 2 1550.0*/51.744,	/* 3 2000.0*/58.603,	/* 4 2450.0*/56.427,	/* 5 2900.0*/55.073,	/* 6 3350.0*/57.869,	/* 7 3800.0*/55.562,	/* 8 4250.0*/54.198,	/* 9 4700.0*/56.585,	/* 10 5150.0*/56.427,	/* 11 5650.0*/56.253,	/* 12 6100.0*/56.095,	/* 13 6550.0*/55.938,	/* 14 7000.0*/55.779,	/* 15 7450.0*/55.623,	},
		{/* 7 100.000	*//* 0 650.0*/45.541,	/* 1 1100.0*/43.911,	/* 2 1550.0*/69.598,	/* 3 2000.0*/69.534,	/* 4 2450.0*/56.299,	/* 5 2900.0*/66.710,	/* 6 3350.0*/64.073,	/* 7 3800.0*/59.968,	/* 8 4250.0*/62.489,	/* 9 4700.0*/68.074,	/* 10 5150.0*/73.266,	/* 11 5650.0*/75.644,	/* 12 6100.0*/53.636,	/* 13 6550.0*/55.891,	/* 14 7000.0*/58.144,	/* 15 7450.0*/60.398,	},
		{/* 8 120.000	*//* 0 650.0*/51.418,	/* 1 1100.0*/52.427,	/* 2 1550.0*/53.436,	/* 3 2000.0*/54.445,	/* 4 2450.0*/55.453,	/* 5 2900.0*/57.040,	/* 6 3350.0*/59.458,	/* 7 3800.0*/63.820,	/* 8 4250.0*/63.933,	/* 9 4700.0*/70.255,	/* 10 5150.0*/76.251,	/* 11 5650.0*/75.242,	/* 12 6100.0*/58.230,	/* 13 6550.0*/60.543,	/* 14 7000.0*/62.858,	/* 15 7450.0*/65.172,	},
		{/* 9 140.000	*//* 0 650.0*/57.118,	/* 1 1100.0*/59.040,	/* 2 1550.0*/60.962,	/* 3 2000.0*/62.884,	/* 4 2450.0*/64.806,	/* 5 2900.0*/69.796,	/* 6 3350.0*/68.924,	/* 7 3800.0*/72.413,	/* 8 4250.0*/76.775,	/* 9 4700.0*/79.393,	/* 10 5150.0*/81.138,	/* 11 5650.0*/80.267,	/* 12 6100.0*/73.067,	/* 13 6550.0*/76.288,	/* 14 7000.0*/79.510,	/* 15 7450.0*/82.730,	},
		{/* 10 150.000	*//* 0 650.0*/59.967,	/* 1 1100.0*/62.346,	/* 2 1550.0*/64.725,	/* 3 2000.0*/67.104,	/* 4 2450.0*/69.796,	/* 5 2900.0*/84.629,	/* 6 3350.0*/77.647,	/* 7 3800.0*/79.393,	/* 8 4250.0*/85.499,	/* 9 4700.0*/88.990,	/* 10 5150.0*/87.246,	/* 11 5650.0*/84.629,	/* 12 6100.0*/80.487,	/* 13 6550.0*/84.160,	/* 14 7000.0*/87.836,	/* 15 7450.0*/91.511,	},
		{/* 11 160.000	*//* 0 650.0*/62.816,	/* 1 1100.0*/65.652,	/* 2 1550.0*/68.486,	/* 3 2000.0*/71.322,	/* 4 2450.0*/74.158,	/* 5 2900.0*/86.373,	/* 6 3350.0*/80.267,	/* 7 3800.0*/82.010,	/* 8 4250.0*/87.246,	/* 9 4700.0*/93.352,	/* 10 5150.0*/93.352,	/* 11 5650.0*/93.352,	/* 12 6100.0*/87.905,	/* 13 6550.0*/92.032,	/* 14 7000.0*/96.161,	/* 15 7450.0*/100.287,	},
		{/* 12 170.000	*//* 0 650.0*/63.690,	/* 1 1100.0*/63.690,	/* 2 1550.0*/63.690,	/* 3 2000.0*/81.138,	/* 4 2450.0*/86.373,	/* 5 2900.0*/87.246,	/* 6 3350.0*/87.246,	/* 7 3800.0*/87.246,	/* 8 4250.0*/86.373,	/* 9 4700.0*/87.246,	/* 10 5150.0*/87.246,	/* 11 5650.0*/87.246,	/* 12 6100.0*/95.323,	/* 13 6550.0*/99.904,	/* 14 7000.0*/104.485,	/* 15 7450.0*/109.066,	},
		{/* 13 180.000	*//* 0 650.0*/64.562,	/* 1 1100.0*/64.562,	/* 2 1550.0*/64.562,	/* 3 2000.0*/82.010,	/* 4 2450.0*/89.862,	/* 5 2900.0*/90.735,	/* 6 3350.0*/90.735,	/* 7 3800.0*/90.735,	/* 8 4250.0*/90.735,	/* 9 4700.0*/90.735,	/* 10 5150.0*/90.735,	/* 11 5650.0*/90.735,	/* 12 6100.0*/103.355,	/* 13 6550.0*/108.388,	/* 14 7000.0*/113.422,	/* 15 7450.0*/118.458,	},
		{/* 14 190.000	*//* 0 650.0*/65.435,	/* 1 1100.0*/65.435,	/* 2 1550.0*/65.435,	/* 3 2000.0*/82.884,	/* 4 2450.0*/90.735,	/* 5 2900.0*/92.479,	/* 6 3350.0*/93.352,	/* 7 3800.0*/93.352,	/* 8 4250.0*/93.352,	/* 9 4700.0*/93.352,	/* 10 5150.0*/93.352,	/* 11 5650.0*/93.352,	/* 12 6100.0*/110.774,	/* 13 6550.0*/116.262,	/* 14 7000.0*/121.749,	/* 15 7450.0*/127.237,	},
		{/* 15 200.000	*//* 0 650.0*/66.307,	/* 1 1100.0*/66.307,	/* 2 1550.0*/66.307,	/* 3 2000.0*/83.756,	/* 4 2450.0*/91.608,	/* 5 2900.0*/93.352,	/* 6 3350.0*/94.224,	/* 7 3800.0*/94.224,	/* 8 4250.0*/94.224,	/* 9 4700.0*/94.224,	/* 10 5150.0*/94.224,	/* 11 5650.0*/94.224,	/* 12 6100.0*/118.805,	/* 13 6550.0*/124.742,	/* 14 7000.0*/130.684,	/* 15 7450.0*/136.627,	},
};
#endif

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const ignition_table_t mapBased16IgnitionTable = {
		/* Generated by TS2C on Wed Jun 15 21:22:11 EDT 2016*/
		{/* 0 18.000	*//* 0 650.0*/10.000,	/* 1 1100.0*/11.432,	/* 2 1550.0*/12.863,	/* 3 2000.0*/14.295,	/* 4 2450.0*/15.726,	/* 5 2900.0*/17.157,	/* 6 3350.0*/18.589,	/* 7 3800.0*/20.021,	/* 8 4250.0*/21.452,	/* 9 4700.0*/22.884,	/* 10 5150.0*/24.315,	/* 11 5650.0*/25.906,	/* 12 6100.0*/27.337,	/* 13 6550.0*/28.769,	/* 14 7000.0*/30.200,	/* 15 7450.0*/30.200,	},
		{/* 1 24.000	*//* 0 650.0*/10.222,	/* 1 1100.0*/11.575,	/* 2 1550.0*/12.927,	/* 3 2000.0*/14.279,	/* 4 2450.0*/15.632,	/* 5 2900.0*/16.984,	/* 6 3350.0*/18.337,	/* 7 3800.0*/19.689,	/* 8 4250.0*/21.055,	/* 9 4700.0*/22.437,	/* 10 5150.0*/23.819,	/* 11 5650.0*/25.354,	/* 12 6100.0*/26.736,	/* 13 6550.0*/28.118,	/* 14 7000.0*/29.500,	/* 15 7450.0*/29.500,	},
		{/* 2 33.000	*//* 0 650.0*/10.556,	/* 1 1100.0*/11.789,	/* 2 1550.0*/13.023,	/* 3 2000.0*/14.257,	/* 4 2450.0*/15.490,	/* 5 2900.0*/16.724,	/* 6 3350.0*/17.958,	/* 7 3800.0*/19.192,	/* 8 4250.0*/20.426,	/* 9 4700.0*/21.659,	/* 10 5150.0*/22.893,	/* 11 5650.0*/24.264,	/* 12 6100.0*/25.497,	/* 13 6550.0*/26.731,	/* 14 7000.0*/27.965,	/* 15 7450.0*/27.965,	},
		{/* 3 39.000	*//* 0 650.0*/10.778,	/* 1 1100.0*/11.932,	/* 2 1550.0*/13.087,	/* 3 2000.0*/14.242,	/* 4 2450.0*/15.396,	/* 5 2900.0*/16.551,	/* 6 3350.0*/17.706,	/* 7 3800.0*/18.860,	/* 8 4250.0*/20.015,	/* 9 4700.0*/21.169,	/* 10 5150.0*/22.324,	/* 11 5650.0*/23.607,	/* 12 6100.0*/24.761,	/* 13 6550.0*/25.916,	/* 14 7000.0*/27.071,	/* 15 7450.0*/27.071,	},
		{/* 4 45.000	*//* 0 650.0*/11.000,	/* 1 1100.0*/12.076,	/* 2 1550.0*/13.151,	/* 3 2000.0*/14.227,	/* 4 2450.0*/15.302,	/* 5 2900.0*/16.378,	/* 6 3350.0*/17.453,	/* 7 3800.0*/18.529,	/* 8 4250.0*/19.604,	/* 9 4700.0*/20.680,	/* 10 5150.0*/21.755,	/* 11 5650.0*/22.950,	/* 12 6100.0*/24.026,	/* 13 6550.0*/25.101,	/* 14 7000.0*/26.177,	/* 15 7450.0*/26.177,	},
		{/* 5 51.000	*//* 0 650.0*/11.000,	/* 1 1100.0*/11.572,	/* 2 1550.0*/12.624,	/* 3 2000.0*/13.676,	/* 4 2450.0*/14.727,	/* 5 2900.0*/15.779,	/* 6 3350.0*/16.831,	/* 7 3800.0*/17.883,	/* 8 4250.0*/18.929,	/* 9 4700.0*/19.969,	/* 10 5150.0*/21.009,	/* 11 5650.0*/22.164,	/* 12 6100.0*/23.203,	/* 13 6550.0*/24.243,	/* 14 7000.0*/25.283,	/* 15 7450.0*/25.283,	},
		{/* 6 60.000	*//* 0 650.0*/9.800,	/* 1 1100.0*/10.816,	/* 2 1550.0*/11.833,	/* 3 2000.0*/12.849,	/* 4 2450.0*/13.865,	/* 5 2900.0*/14.882,	/* 6 3350.0*/15.898,	/* 7 3800.0*/16.914,	/* 8 4250.0*/17.917,	/* 9 4700.0*/18.903,	/* 10 5150.0*/19.889,	/* 11 5650.0*/20.984,	/* 12 6100.0*/21.970,	/* 13 6550.0*/22.956,	/* 14 7000.0*/23.941,	/* 15 7450.0*/23.941,	},
		{/* 7 66.000	*//* 0 650.0*/9.320,	/* 1 1100.0*/10.313,	/* 2 1550.0*/11.305,	/* 3 2000.0*/12.298,	/* 4 2450.0*/13.291,	/* 5 2900.0*/14.283,	/* 6 3350.0*/15.276,	/* 7 3800.0*/16.269,	/* 8 4250.0*/17.242,	/* 9 4700.0*/18.192,	/* 10 5150.0*/19.142,	/* 11 5650.0*/20.198,	/* 12 6100.0*/21.147,	/* 13 6550.0*/22.097,	/* 14 7000.0*/23.047,	/* 15 7450.0*/23.047,	},
		{/* 8 72.000	*//* 0 650.0*/8.840,	/* 1 1100.0*/9.809,	/* 2 1550.0*/10.778,	/* 3 2000.0*/11.747,	/* 4 2450.0*/12.716,	/* 5 2900.0*/13.685,	/* 6 3350.0*/14.654,	/* 7 3800.0*/15.623,	/* 8 4250.0*/16.567,	/* 9 4700.0*/17.481,	/* 10 5150.0*/18.395,	/* 11 5650.0*/19.411,	/* 12 6100.0*/20.325,	/* 13 6550.0*/21.239,	/* 14 7000.0*/22.153,	/* 15 7450.0*/22.153,	},
		{/* 9 78.000	*//* 0 650.0*/8.360,	/* 1 1100.0*/9.305,	/* 2 1550.0*/10.251,	/* 3 2000.0*/11.196,	/* 4 2450.0*/12.141,	/* 5 2900.0*/13.086,	/* 6 3350.0*/14.032,	/* 7 3800.0*/14.977,	/* 8 4250.0*/15.892,	/* 9 4700.0*/16.771,	/* 10 5150.0*/17.649,	/* 11 5650.0*/18.624,	/* 12 6100.0*/19.502,	/* 13 6550.0*/20.381,	/* 14 7000.0*/21.259,	/* 15 7450.0*/21.259,	},
		{/* 10 84.000	*//* 0 650.0*/7.880,	/* 1 1100.0*/8.802,	/* 2 1550.0*/9.723,	/* 3 2000.0*/10.645,	/* 4 2450.0*/11.566,	/* 5 2900.0*/12.488,	/* 6 3350.0*/13.410,	/* 7 3800.0*/14.331,	/* 8 4250.0*/15.253,	/* 9 4700.0*/16.175,	/* 10 5150.0*/17.096,	/* 11 5650.0*/18.120,	/* 12 6100.0*/19.042,	/* 13 6550.0*/19.963,	/* 14 7000.0*/20.885,	/* 15 7450.0*/20.885,	},
		{/* 11 93.000	*//* 0 650.0*/7.160,	/* 1 1100.0*/8.046,	/* 2 1550.0*/8.932,	/* 3 2000.0*/9.818,	/* 4 2450.0*/10.704,	/* 5 2900.0*/11.590,	/* 6 3350.0*/12.477,	/* 7 3800.0*/13.363,	/* 8 4250.0*/14.249,	/* 9 4700.0*/15.135,	/* 10 5150.0*/16.021,	/* 11 5650.0*/17.005,	/* 12 6100.0*/17.891,	/* 13 6550.0*/18.778,	/* 14 7000.0*/19.664,	/* 15 7450.0*/19.664,	},
		{/* 12 99.000	*//* 0 650.0*/6.680,	/* 1 1100.0*/6.967,	/* 2 1550.0*/8.405,	/* 3 2000.0*/9.000,	/* 4 2450.0*/9.937,	/* 5 2900.0*/10.992,	/* 6 3350.0*/11.854,	/* 7 3800.0*/12.717,	/* 8 4250.0*/13.579,	/* 9 4700.0*/14.442,	/* 10 5150.0*/15.304,	/* 11 5650.0*/16.262,	/* 12 6100.0*/17.125,	/* 13 6550.0*/17.987,	/* 14 7000.0*/18.850,	/* 15 7450.0*/18.850,	},
		{/* 13 105.000	*//* 0 650.0*/6.200,	/* 1 1100.0*/7.039,	/* 2 1550.0*/7.877,	/* 3 2000.0*/8.716,	/* 4 2450.0*/9.555,	/* 5 2900.0*/10.394,	/* 6 3350.0*/11.232,	/* 7 3800.0*/12.071,	/* 8 4250.0*/12.910,	/* 9 4700.0*/13.749,	/* 10 5150.0*/14.587,	/* 11 5650.0*/15.519,	/* 12 6100.0*/16.358,	/* 13 6550.0*/17.197,	/* 14 7000.0*/18.035,	/* 15 7450.0*/18.035,	},
		{/* 14 111.000	*//* 0 650.0*/5.720,	/* 1 1100.0*/6.535,	/* 2 1550.0*/7.350,	/* 3 2000.0*/8.165,	/* 4 2450.0*/8.980,	/* 5 2900.0*/9.795,	/* 6 3350.0*/10.610,	/* 7 3800.0*/11.425,	/* 8 4250.0*/12.240,	/* 9 4700.0*/13.055,	/* 10 5150.0*/13.870,	/* 11 5650.0*/14.776,	/* 12 6100.0*/15.591,	/* 13 6550.0*/16.406,	/* 14 7000.0*/17.221,	/* 15 7450.0*/17.221,	},
		{/* 15 120.000	*//* 0 650.0*/5.000,	/* 1 1100.0*/5.780,	/* 2 1550.0*/6.559,	/* 3 2000.0*/7.339,	/* 4 2450.0*/8.118,	/* 5 2900.0*/8.898,	/* 6 3350.0*/9.677,	/* 7 3800.0*/10.457,	/* 8 4250.0*/11.236,	/* 9 4700.0*/12.016,	/* 10 5150.0*/12.795,	/* 11 5650.0*/13.661,	/* 12 6100.0*/14.441,	/* 13 6550.0*/15.220,	/* 14 7000.0*/16.000,	/* 15 7450.0*/16.000,	},
};
#endif

static const fuel_table_t miataNA6_maf_fuel_table = {
		/* Generated by TS2C on Tue Mar 12 13:59:29 EDT 2019*/
		{/* 0 1.200	*//* 0 800.0*/9.000,	/* 1 1213.33*/10.846,	/* 2 1626.6699*/11.574,	/* 3 2040.0*/11.420,	/* 4 2453.3298*/8.813,	/* 5 2866.67*/6.000,	/* 6 3280.0*/6.000,	/* 7 3693.3298*/6.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 1 1.410	*//* 0 800.0*/8.276,	/* 1 1213.33*/10.727,	/* 2 1626.6699*/9.000,	/* 3 2040.0*/6.118,	/* 4 2453.3298*/5.913,	/* 5 2866.67*/6.000,	/* 6 3280.0*/6.000,	/* 7 3693.3298*/6.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 2 1.630	*//* 0 800.0*/4.888,	/* 1 1213.33*/5.283,	/* 2 1626.6699*/3.780,	/* 3 2040.0*/3.454,	/* 4 2453.3298*/3.936,	/* 5 2866.67*/4.466,	/* 6 3280.0*/6.000,	/* 7 3693.3298*/6.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 3 1.840	*//* 0 800.0*/6.811,	/* 1 1213.33*/4.178,	/* 2 1626.6699*/2.534,	/* 3 2040.0*/2.181,	/* 4 2453.3298*/1.490,	/* 5 2866.67*/6.000,	/* 6 3280.0*/6.000,	/* 7 3693.3298*/6.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 4 2.050	*//* 0 800.0*/5.569,	/* 1 1213.33*/3.145,	/* 2 1626.6699*/1.370,	/* 3 2040.0*/1.205,	/* 4 2453.3298*/1.490,	/* 5 2866.67*/6.000,	/* 6 3280.0*/6.000,	/* 7 3693.3298*/6.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 5 2.270	*//* 0 800.0*/4.499,	/* 1 1213.33*/2.668,	/* 2 1626.6699*/1.407,	/* 3 2040.0*/1.375,	/* 4 2453.3298*/1.490,	/* 5 2866.67*/3.000,	/* 6 3280.0*/3.000,	/* 7 3693.3298*/7.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 6 2.480	*//* 0 800.0*/2.967,	/* 1 1213.33*/1.951,	/* 2 1626.6699*/2.439,	/* 3 2040.0*/4.185,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 7 2.690	*//* 0 800.0*/2.645,	/* 1 1213.33*/1.764,	/* 2 1626.6699*/1.819,	/* 3 2040.0*/2.000,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 8 2.910	*//* 0 800.0*/3.000,	/* 1 1213.33*/2.012,	/* 2 1626.6699*/1.957,	/* 3 2040.0*/3.000,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 9 3.120	*//* 0 800.0*/3.000,	/* 1 1213.33*/3.000,	/* 2 1626.6699*/3.000,	/* 3 2040.0*/3.000,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 10 3.330	*//* 0 800.0*/3.000,	/* 1 1213.33*/3.000,	/* 2 1626.6699*/3.000,	/* 3 2040.0*/3.000,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 11 3.550	*//* 0 800.0*/3.000,	/* 1 1213.33*/3.000,	/* 2 1626.6699*/3.000,	/* 3 2040.0*/3.000,	/* 4 2453.3298*/3.000,	/* 5 2866.67*/4.000,	/* 6 3280.0*/4.000,	/* 7 3693.3298*/4.000,	/* 8 4106.67*/4.000,	/* 9 4520.0*/4.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 12 3.760	*//* 0 800.0*/7.672,	/* 1 1213.33*/7.000,	/* 2 1626.6699*/7.000,	/* 3 2040.0*/7.000,	/* 4 2453.3298*/7.000,	/* 5 2866.67*/7.000,	/* 6 3280.0*/7.000,	/* 7 3693.3298*/7.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 13 3.970	*//* 0 800.0*/7.858,	/* 1 1213.33*/7.000,	/* 2 1626.6699*/7.000,	/* 3 2040.0*/7.000,	/* 4 2453.3298*/7.000,	/* 5 2866.67*/7.000,	/* 6 3280.0*/7.000,	/* 7 3693.3298*/7.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 14 4.190	*//* 0 800.0*/7.000,	/* 1 1213.33*/7.000,	/* 2 1626.6699*/7.000,	/* 3 2040.0*/7.000,	/* 4 2453.3298*/7.000,	/* 5 2866.67*/7.000,	/* 6 3280.0*/7.000,	/* 7 3693.3298*/7.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
		{/* 15 4.400	*//* 0 800.0*/7.000,	/* 1 1213.33*/7.000,	/* 2 1626.6699*/7.000,	/* 3 2040.0*/7.000,	/* 4 2453.3298*/7.000,	/* 5 2866.67*/7.000,	/* 6 3280.0*/7.000,	/* 7 3693.3298*/7.000,	/* 8 4106.67*/6.000,	/* 9 4520.0*/6.000,	/* 10 4933.33*/6.000,	/* 11 5346.67*/6.000,	/* 12 5760.0*/6.000,	/* 13 6173.33*/6.000,	/* 14 6586.67*/6.000,	/* 15 7000.0*/6.000,	},
};

static void miataNAcommonEngineSettings(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->debugMode = DBG_TRIGGER_COUNTERS;

	setCommonNTCSensor(&engineConfiguration->clt);
	setCommonNTCSensor(&engineConfiguration->iat);

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTimingTable(mapBased16IgnitionTable, config->ignitionTable);
#endif

	boardConfiguration->idle.solenoidFrequency = 160;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
}

void miataNAcommon(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	miataNAcommonEngineSettings(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;

	boardConfiguration->idle.solenoidPin = GPIOB_9; // this W61 <> W61 jumper, pin 3W

	boardConfiguration->ignitionPins[0] = GPIOE_14; // Frankenso high side - pin 1G
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOC_7; // Frankenso high side - pin 1H
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
}

static void setMiataNA6_settings(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->bc.isFasterEngineSpinUpEnabled = true;

	memcpy(config->veRpmBins, ve16RpmBins, sizeof(ve16RpmBins));
	memcpy(config->veLoadBins, ve16LoadBins, sizeof(ve16LoadBins));
#if DEFAULT_FUEL_LOAD_COUNT == FUEL_LOAD_COUNT
	copyFuelTable(mapBased16VeTable, config->veTable);
#endif

	setWholeFuelMap(6 PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->idleMode = IM_AUTO;
	// below 20% this valve seems to be opening for fail-safe idle air
	engineConfiguration->idleRpmPid.minValue = 20;
	engineConfiguration->idleRpmPid.pFactor = 0.01;
	engineConfiguration->idleRpmPid.iFactor = 0.00001;
	engineConfiguration->idleRpmPid.dFactor = 0.0001;
	engineConfiguration->idleRpmPid.periodMs = 100;

	/**
	 * http://miataturbo.wikidot.com/fuel-injectors
	 * 90-93 (Blue) - #195500-1970
	 */
	engineConfiguration->injector.flow = 230;

	// set cranking_timing_angle 10
	engineConfiguration->crankingTimingAngle = 10;

	engineConfiguration->map.sensor.type = MT_GM_3_BAR;

	// chartsize 200
	engineConfiguration->engineChartSize = 200;

	// maybe adjust CLT correction?
	// set cranking_fuel 8
	engineConfiguration->cranking.baseFuel = 1;
	config->crankingFuelCoef[0] = 28; // base cranking fuel adjustment coefficient
	config->crankingFuelBins[0] = -20; // temperature in C
	config->crankingFuelCoef[1] = 22;
	config->crankingFuelBins[1] = -10;
	config->crankingFuelCoef[2] = 18;
	config->crankingFuelBins[2] = 5;
	config->crankingFuelCoef[3] = 15;
	config->crankingFuelBins[3] = 30;

	config->crankingFuelCoef[4] = 10;
	config->crankingFuelBins[4] = 35;
	config->crankingFuelCoef[5] = 10;
	config->crankingFuelBins[5] = 50;
	config->crankingFuelCoef[6] = 10;
	config->crankingFuelBins[6] = 65;
	config->crankingFuelCoef[7] = 10;
	config->crankingFuelBins[7] = 90;

	engineConfiguration->specs.displacement = 1.6;

	// my car was originally a manual so proper TPS
	engineConfiguration->tpsMin = 93; // convert 12to10 bit (ADC/4)
	engineConfiguration->tpsMax = 656; // convert 12to10 bit (ADC/4)

	engineConfiguration->injectionMode = IM_BATCH;
	copyFuelTable(miataNA6_maf_fuel_table, config->fuelTable);
}

void setMiataNA6_MAP_Frankenso(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	boardConfiguration->isHip9011Enabled = false;
	boardConfiguration->isSdCardEnabled = false;

	setMiataNA6_settings(PASS_CONFIG_PARAMETER_SIGNATURE);

	// Frankenso middle plug 2J, W32 top <> W47 bottom "#5 Green" jumper, not OEM
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

	engineConfiguration->mafAdcChannel = EFI_ADC_0;

	// Wide band oxygen (from middle plug) to W52
	engineConfiguration->afr.hwChannel = EFI_ADC_13; // PA3

	engineConfiguration->vbattDividerCoeff = 9.75;// ((float) (8.2 + 33)) / 8.2 * 2;

	boardConfiguration->isSdCardEnabled = true;

//	/**
//	 * oil pressure line
//	 * adc4/pa4/W47
//	 */
//	engineConfiguration->fsioAdc[0] = EFI_ADC_NONE;


	// warning light
	/**
	 * to test
	 * set_fsio_setting 1 1800
	 * set_fsio_setting 2 95
	 * set_fsio_setting 4 14
	 *
	 * set_rpn_expression 1 "rpm 0 fsio_setting > coolant 1 fsio_setting > | vbatt 2 fsio_setting < |"
	 * rpn_eval "rpm 1 fsio_setting > coolant 2 fsio_setting > | vbatt 4 fsio_setting < |"
	 */
	/*
//WARNING:	these indeces are off
	boardConfiguration->fsio_setting[0] = 6400; // RPM threshold
	boardConfiguration->fsio_setting[1] = 100; // CLT threshold, fsio_setting #2
	boardConfiguration->fsio_setting[2] = 13.0; // voltage threshold, fsio_setting #3

	//	set_fsio_setting 4 3000
	boardConfiguration->fsio_setting[3] = 3000; // oil pressure RPM, fsio_setting #4
	// set_fsio_setting 5 0.52
	boardConfiguration->fsio_setting[4] = 0.52; // oil pressure threshold, fsio_setting #5
*/

//	 * set_rpn_expression 1 "rpm 3 fsio_setting >"
	// rpn_eval "rpm 1 fsio_setting >"
	// rpn_eval "fsio_analog_input"

	// set_fsio_expression 0 "((rpm > fsio_setting(4) & (fsio_analog_input < fsio_setting(5)) | rpm > fsio_setting(1) | (coolant > fsio_setting(2)  > | (vbatt < fsio_setting(3)"

#if EFI_FSIO
	// todo: convert
	setFsio(0, GPIOC_13, COMBINED_WARNING_LIGHT PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */

	boardConfiguration->injectionPins[0] = GPIOD_3; // #1&3 pin 3U
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2&4 pin 3V
	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	// white wire from 1E - TOP of W4 to BOTTOM W62
	boardConfiguration->malfunctionIndicatorPin = GPIOD_5;

	// yellow wire from 1V/W22 to bottom of W48
	boardConfiguration->clutchDownPin = GPIOA_3;
	boardConfiguration->clutchDownPinMode = PI_PULLUP;


	// 110mm red wire from 1N/W14 to bottom of W45
	engineConfiguration->throttlePedalUpPin = GPIOA_7;

	// green wire from 1Q/W17 to bottom of W46
	engineConfiguration->acSwitchAdc = EFI_ADC_6; // PA6

	miataNAcommon(PASS_CONFIG_PARAMETER_SIGNATURE);
}

void setMiataNA6_VAF_Frankenso(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setMiataNA6_MAP_Frankenso(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->fuelAlgorithm = LM_PLAIN_MAF;

	/**
	 * Stage 0 we only have OEM TPS switch
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	// todo: MAF-based ignition table?
}

/**
 * set engine_type 12
 */
void setMiataNA6_VAF_MRE(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// CLT: "18 - AN temp 1"
	// IAT: "23 - AN temp 2"
	// MAF/VAF: "19 - AN volt 4"
	engineConfiguration->mafAdcChannel = EFI_ADC_12;

	//boardConfiguration->triggerInputPins[0] = GPIOC_6;
	boardConfiguration->triggerInputPins[1] = GPIOA_5;
	engineConfiguration->camInputs[0] = GPIO_UNASSIGNED;

	boardConfiguration->ignitionPins[0] = GPIOD_7;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOD_6;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	// tps = "20 - AN volt 5"
	//engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;



	// TLE8888_PIN_24: "43 - GP Out 4"
	// MIL check engine
	boardConfiguration->malfunctionIndicatorPin = TLE8888_PIN_24;

	// IAC: GPIOE_9:  "7 - Lowside 1"


	boardConfiguration->isHip9011Enabled = false;
	boardConfiguration->isSdCardEnabled = false;

	setMiataNA6_settings(PASS_CONFIG_PARAMETER_SIGNATURE);
	miataNAcommonEngineSettings(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->fuelAlgorithm = LM_PLAIN_MAF;

}
