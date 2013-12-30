// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include "expected_results.hpp"

// Small helper function to get the size of the arrays
template<class T, uint32_t N>
inline uint32_t dimension_of(T (&)[N])
{
    return N;
}

const expected_result_2<fifi::binary8>
multiply_results<fifi::binary8>::m_results[] =
{
    // arg1, arg2, result
    { 0,   0,   0   },
    { 0,   2,   0   },
    { 1,   2,   2   },
    { 2,   1,   2   },
    { 1,   1,   1   },
    { 1,   46,  46  },
    { 67,  1,   67  },
    { 5,   5,   17  },
    { 10,  10,  68  },
    { 10,  20,  136 },
    { 25,  10,  250 },
    { 25,  15,  135 },
    { 25,  66,  60  },
    { 12,  87,  195 },
    { 144, 66,  206 },
    { 4,   200, 7   },
    { 110, 7,   23  },
    { 30,  242, 78  },
    { 176, 132, 204 },
    { 200, 200, 158 },
    { 251, 133, 104 },
    { 254, 254, 227 },
    { 255, 255, 226 },
    { 110, 253, 232 }
};

const uint32_t multiply_results<fifi::binary8>::m_size =
    dimension_of(multiply_results<fifi::binary8>::m_results);


const expected_result_2<fifi::binary8>
divide_results<fifi::binary8>::m_results[] =
{
    // arg1, arg2, result
    { 0,   2,   0   },
    { 1,   2,   142 },
    { 2,   1,   2   },
    { 1,   1,   1   },
    { 1,   46,  51  },
    { 67,  1,   67  },
    { 5,   5,   1   },
    { 10,  10,  1   },
    { 10,  20,  142 },
    { 25,  10,  42  },
    { 25,  15,  199 },
    { 25,  66,  80  },
    { 12,  87,  182 },
    { 144, 66,  254 },
    { 4,   200, 111 },
    { 110, 7,   125 },
    { 30,  242, 199 },
    { 176, 132, 68  },
    { 200, 200, 1   },
    { 251, 133, 52  },
    { 254, 254, 1   },
    { 255, 255, 1   },
    { 110, 253, 52  }
};

const uint32_t divide_results<fifi::binary8>::m_size =
    dimension_of(divide_results<fifi::binary8>::m_results);

// Add binary8
//
// Computed using the following Matlab script:
//
//     m = 8
//     n = 2^m - 1
//     max = 20
//
//     for j = 0:max
//       v1 = uint8(rand(1) * n);
//       v2 = uint8(rand(1) * n);
//
//       gf1 = gf(v1, m);
//       gf2 = gf(v2, m);
//
//       xgf = gf1 + gf2;
//
//       str = sprintf('{ %4d, %4d, %4d }', uint8(gf1.x), uint8(gf2.x), uint8(xgf.x));
//       disp(str)
//     end
const expected_result_2<fifi::binary8>
add_results<fifi::binary8>::m_results[] =
{
    // arg1, arg2, result
    {  194,  135,   69 },
    {  163,   53,  150 },
    {   97,  200,  169 },
    {  174,  118,  216 },
    {  145,  203,   90 },
    {   15,  154,  149 },
    {   13,  106,  103 },
    {   78,  223,  145 },
    {    4,  196,  192 },
    {  248,  252,    4 },
    {  201,  112,  185 },
    {  127,   55,   72 },
    {  164,   82,  246 },
    {  245,  185,   76 },
    {  105,  190,  215 },
    {   68,  112,   52 },
    {  238,  174,   64 },
    {   54,  214,  224 },
    {  160,   34,  130 },
    {   53,  155,  174 },
    {  161,   94,  255 }
};

const uint32_t add_results<fifi::binary8>::m_size =
    dimension_of(add_results<fifi::binary8>::m_results);


// Subtract binary8
//
// Computed using the following Matlab script:
//
//     m = 8
//     n = 2^m - 1
//     max = 20
//
//     for j = 0:max
//       v1 = uint8(rand(1) * n);
//       v2 = uint8(rand(1) * n);
//
//       gf1 = gf(v1, m);
//       gf2 = gf(v2, m);
//
//       xgf = gf1 - gf2;
//
//       str = sprintf('{ %4d, %4d, %4d }', ...
//                     uint8(gf1.x), uint8(gf2.x), ...
//                     uint8(xgf.x));
//       disp(str)
//     end
const expected_result_2<fifi::binary8>
subtract_results<fifi::binary8>::m_results[] =
{
    // arg1, arg2, result
    {  147,  115,  224 },
    {   11,    7,   12 },
    {   80,    3,   83 },
    {   98,  174,  204 },
    {   24,    9,   17 },
    {  156,  155,    7 },
    {    4,    4,    0 },
    {   48,  150,  166 },
    {   15,   94,   81 },
    {  161,  183,   22 },
    {  177,   21,  164 },
    {  116,  113,    5 },
    {   90,   39,  125 },
    {  172,  178,   30 },
    {  186,  122,  192 },
    {  141,   31,  146 },
    {  115,  183,  196 },
    {  228,   70,  162 },
    {   65,  221,  156 },
    {   59,  205,  246 },
    {  232,   59,  211 }
};

const uint32_t subtract_results<fifi::binary8>::m_size =
    dimension_of(subtract_results<fifi::binary8>::m_results);


// Invert binary8
const expected_result_1<fifi::binary8>
invert_results<fifi::binary8>::m_results[] =
{
    // arg1, result
    { 1,   1   },
    { 2,   142 },
    { 5,   167 },
    { 10,  221 },
    { 15,  150 },
    { 20,  224 },
    { 25,  222 },
    { 46,  51  },
    { 66,  248 },
    { 67,  213 },
    { 87,  97  },
    { 110, 47  },
    { 132, 124 },
    { 133, 204 },
    { 144, 24  },
    { 200, 210 },
    { 242, 181 },
    { 253, 255 },
    { 254, 126 },
    { 255, 253 }
};

const uint32_t invert_results<fifi::binary8>::m_size =
    dimension_of(invert_results<fifi::binary8>::m_results);



const expected_result_2<fifi::binary16>
multiply_results<fifi::binary16>::m_results[] =
{
    // arg1, arg2, result
    { 0,   0,   0     },
    { 0,   2,   0     },
    { 1,   2,   2     },
    { 2,   1,   2     },
    { 1,   1,   1     },
    { 1,   46,  46    },
    { 67,  1,   67    },
    { 5,   5,   17    },
    { 10,  10,  68    },
    { 10,  20,  136   },
    { 25,  10,  250   },
    { 25,  15,  135   },
    { 25,  66,  1650  },
    { 12,  87,  996   },
    { 144, 66,  9504  },
    { 4,   200, 800   },
    { 110, 7,   266   },
    { 30,  242, 2716  },
    { 176, 132, 23232 },
    { 200, 200, 20544 },
    { 251, 133, 32407 },
    { 254, 254, 21844 },
    { 255, 255, 21845 },
    { 110, 253, 9478  }
};

const uint32_t multiply_results<fifi::binary16>::m_size =
    dimension_of(multiply_results<fifi::binary16>::m_results);



const expected_result_2<fifi::binary16>
divide_results<fifi::binary16>::m_results[] =
{
    // arg1, arg2, result
    { 0,   2,   0     },
    { 1,   2,   34821 },
    { 2,   1,   2     },
    { 1,   1,   1     },
    { 1,   46,  2983  },
    { 67,  1,   67    },
    { 5,   5,   1     },
    { 10,  10,  1     },
    { 10,  20,  34821 },
    { 25,  10,  55300 },
    { 25,  15,  24577 },
    { 25,  66,  55164 },
    { 12,  87,  46287 },
    { 144, 66,  54045 },
    { 4,   200, 28050 },
    { 110, 7,   54984 },
    { 30,  242, 34597 },
    { 176, 132, 2247  },
    { 200, 200, 1     },
    { 251, 133, 46436 },
    { 254, 254, 1     },
    { 255, 255, 1     },
    { 110, 253, 825   }
};

const uint32_t divide_results<fifi::binary16>::m_size =
    dimension_of(divide_results<fifi::binary16>::m_results);


// Add binary16
//
// Computed using the following Matlab script:
//
//     m = 16
//     n = 2^m - 1
//     prime = 69643
//     max = 20
//
//     for j = 0:max
//       v1 = uint16(rand(1) * n);
//       v2 = uint16(rand(1) * n);
//
//       gf1 = gf(v1, m, prime);
//       gf2 = gf(v2, m, prime);
//
//       xgf = gf1 + gf2;
//
//       str = sprintf('{ %6dU, %6dU, %6dU }', ...
//                     uint16(gf1.x), uint16(gf2.x), ...
//                     uint16(xgf.x));
//       disp(str)
//     end
const expected_result_2<fifi::binary16>
add_results<fifi::binary16>::m_results[] =
{
    // arg1, arg2, result
    {  11335U,  64208U,  54935U },
    {  17789U,  16536U,   1509U },
    {  57392U,  48319U,  23695U },
    {   8947U,    770U,   8689U },
    {  58582U,  13051U,  54829U },
    {  19577U,  43348U,  58669U },
    {  18639U,  30751U,  12496U },
    {   4245U,  64771U,  60822U },
    {  38193U,  27754U,  63835U },
    {  33784U,  21886U,  54918U },
    {  28371U,  14808U,  22283U },
    {  37998U,  49831U,  22217U },
    {  34722U,  41977U,   9307U },
    {  13701U,  24891U,  21694U },
    {  51335U,  44619U,  26316U },
    {  30218U,  37213U,  59223U },
    {  52049U,   3879U,  50294U },
    {  39509U,   3294U,  38539U },
    {  27222U,  19988U,   9282U },
    {  57302U,    984U,  56334U },
    {  50328U,  63624U,  15376U }
};

const uint32_t add_results<fifi::binary16>::m_size =
    dimension_of(add_results<fifi::binary16>::m_results);



// Subtract binary16
//
// Computed using the following Matlab script:
//
//     m = 16
//     n = 2^m - 1
//     prime = 69643
//     max = 20
//
//     for j = 0:max
//       v1 = uint16(rand(1) * n);
//       v2 = uint16(rand(1) * n);
//
//       gf1 = gf(v1, m, prime);
//       gf2 = gf(v2, m, prime);
//
//       xgf = gf1 - gf2;
//
//       str = sprintf('{ %6dU, %6dU, %6dU }', ...
//                     uint16(gf1.x), uint16(gf2.x), ...
//                     uint16(xgf.x));
//       disp(str)
//     end
const expected_result_2<fifi::binary16>
subtract_results<fifi::binary16>::m_results[] =
{
    // arg1,    arg2,    result
    {  47030U,  45394U,   1764U },
    {   5510U,  29776U,  25046U },
    {  28955U,  23150U,  11125U },
    {  10067U,  44278U,  35749U },
    {  45823U,  47677U,   2242U },
    {  31351U,  36362U,  62589U },
    {   7933U,  29540U,  28057U },
    {  46915U,  58512U,  21459U },
    {  17898U,  16696U,   1234U },
    {  56727U,  15227U,  59116U },
    {  52747U,  59532U,   9863U },
    {  15197U,  15683U,   1566U },
    {   3261U,   5137U,   6316U },
    {  41996U,  12510U,  38098U },
    {  55303U,  11397U,  62594U },
    {  11193U,  65161U,  54576U },
    {  28822U,  22285U,  10139U },
    {  20592U,  23925U,   3333U },
    {  25771U,  38766U,  62405U },
    {   7848U,   2499U,   5995U },
    {  30054U,  57007U,  43977U }
};

const uint32_t subtract_results<fifi::binary16>::m_size =
    dimension_of(subtract_results<fifi::binary16>::m_results);



// Invert binary16
const expected_result_1<fifi::binary16>
invert_results<fifi::binary16>::m_results[] =
{
    // arg1, result
    { 1,   1     },
    { 2,   34821 },
    { 5,   20482 },
    { 10,  10241 },
    { 15,  49159 },
    { 20,  39941 },
    { 25,  56100 },
    { 46,  2983  },
    { 66,  19094 },
    { 67,  56530 },
    { 87,  55062 },
    { 110, 61399 },
    { 132, 9547  },
    { 133, 63805 },
    { 144, 25710 },
    { 200, 37729 },
    { 242, 46215 },
    { 253, 38174 },
    { 254, 12174 },
    { 255, 7372  }
};

const uint32_t invert_results<fifi::binary16>::m_size =
    dimension_of(invert_results<fifi::binary16>::m_results);




const expected_result_2<fifi::prime2325>
multiply_results<fifi::prime2325>::m_results[] =
{
    // arg1,       arg2,        result
    { 0U,          0U,          0U          },
    { 4294967290U, 1U,          4294967290U },
    { 4294967290U, 2U,          4294967289U },
    { 4294967290U, 4294967290U, 1U          },
    { 3301510135U, 2380300242U, 3460816498U },
    { 2747836187U, 606517521U,  4005335094U },
    { 4071004727U, 3893610017U, 3098938562U },
    { 3216464703U, 2321361148U, 2159784831U },
    { 3948329266U, 1846670630U, 2958898160U },
    { 549534826U,  3692094631U, 3813011302U },
    { 890246292U,  3115294715U, 3374921530U },
    { 4057832578U, 783228777U,  1061113545U },
    { 366785572U,  3273833947U, 1357321858U },
    { 484124441U,  2513732051U, 2038062075U },
    { 3916985401U, 2885578550U, 3986996714U },
    { 8961698U,    1454316048U, 2844647930U },
    { 1923821210U, 4910817U,    1727198145U },
    { 628824443U,  2882587157U, 2414550839U },
    { 1921500106U, 1581124167U, 2064202209U },
    { 1241752606U, 2682856520U, 1016247841U },
    { 2865873554U, 4023346111U, 1390523154U },
    { 714307256U,  4017788735U, 2552564636U },
    { 1135999486U, 2298616897U, 2948135396U },
    { 1516764886U, 2487733786U, 1221426309U },
    { 2755140095U, 3176782815U, 3071678684U },
    { 2898832361U, 3285799624U, 205888686U  },
    { 1319528055U, 3213225118U, 472935279U  },
    { 1501780748U, 772682526U,  464840319U  },
    { 591588585U,  2298808855U, 2016341846U },
    { 3791215484U, 823219598U,  3413971131U },
    { 188377222U,  2440371958U, 1837383177U }
};

const uint32_t multiply_results<fifi::prime2325>::m_size =
    dimension_of(multiply_results<fifi::prime2325>::m_results);



const expected_result_2<fifi::prime2325>
divide_results<fifi::prime2325>::m_results[] =
{
    // arg1,       arg2,        result
    { 4294967290U, 1U,          4294967290U },
    { 4294967290U, 2U,          2147483645U },
    { 1U,          4294967290U, 4294967290U },
    { 2U,          4294967290U, 4294967289U },
    { 4294967290U, 4294967290U, 1U          },
    { 3170796863U, 1056595135U, 2349732681U },
    { 3846022732U, 2429951133U, 2542996908U },
    { 1190843567U, 1126933466U, 3113318947U },
    { 4188684512U, 871209300U,  4103154428U },
    { 3235144123U, 393597417U,  3381961331U },
    { 1336522198U, 1280630940U, 91488717U   },
    { 2639979264U, 1669621279U, 3855275193U },
    { 4031746231U, 1150048672U, 2067171649U },
    { 1078074232U, 1671507801U, 2817715425U },
    { 2622046040U, 209860049U,  1117407655U },
    { 4026952922U, 589957415U,  1126790683U },
    { 623898484U,  59902254U,   1708820736U },
    { 1380709308U, 3151385314U, 4242837144U },
    { 4161263482U, 1084683978U, 57951883U   },
    { 2019008287U, 1889765579U, 2374943388U },
    { 2271514114U, 542032264U,  2085796361U },
    { 163139403U,  1828949500U, 2835205604U }
};


const uint32_t divide_results<fifi::prime2325>::m_size =
    dimension_of(divide_results<fifi::prime2325>::m_results);


const expected_result_2<fifi::prime2325>
add_results<fifi::prime2325>::m_results[] =
{
    // arg1,       arg2,        result
    { 0U,          0U,          0U          },
    { 4294967290U, 1U,          0U          },
    { 4294967290U, 2U,          1U          },
    { 4294967290U, 4294967290U, 4294967289U },
    { 3549012440U, 3097460608U, 2351505757U },
    { 1304171116U, 2058347943U, 3362519059U },
    { 2761881503U, 3727769459U, 2194683671U },
    { 416764808U,  2952672146U, 3369436954U },
    { 2228876704U, 3769080616U, 1702990029U },
    { 3393423153U, 2569499151U, 1667955013U },
    { 2721360764U, 3099915689U, 1526309162U },
    { 3733613168U, 4250233244U, 3688879121U },
    { 13775737U,   3832303341U, 3846079078U },
    { 3275759048U, 318206772U,  3593965820U },
    { 954547203U,  3403230939U, 62810851U   },
    { 3860097427U, 3766812989U, 3331943125U },
    { 3164597666U, 249577394U,  3414175060U },
    { 2699559732U, 4130167990U, 2534760431U },
    { 1997027658U, 1158755251U, 3155782909U },
    { 3562900696U, 433428U,     3563334124U },
    { 2780503337U, 508086692U,  3288590029U }

};

const uint32_t add_results<fifi::prime2325>::m_size =
    dimension_of(add_results<fifi::prime2325>::m_results);



const expected_result_2<fifi::prime2325>
subtract_results<fifi::prime2325>::m_results[] =
{
    // arg1,       arg2,        result
    { 0U,          0U,          0U          },
    { 4294967290U, 1U,          4294967289U },
    { 4294967290U, 2U,          4294967288U },
    { 1U,          4294967290U, 2U          },
    { 2U,          4294967290U, 3U          },
    { 4294967290U, 4294967290U, 0U          },
    { 2021566441U, 1563544409U, 458022032U  },
    { 1376464298U, 3719246614U, 1952184975U },
    { 1646681944U, 2090570642U, 3851078593U },
    { 1718415100U, 3764394937U, 2248987454U },
    { 1555616877U, 417999038U,  1137617839U },
    { 3448691623U, 2270397823U, 1178293800U },
    { 3267843829U, 3990959519U, 3571851601U },
    { 4181356062U, 2914712094U, 1266643968U },
    { 2486487802U, 4101049708U, 2680405385U },
    { 2313466124U, 1242826905U, 1070639219U },
    { 3850830140U, 4242429948U, 3903367483U },
    { 815150013U,  2783628454U, 2326488850U },
    { 3601154686U, 238248285U,  3362906401U },
    { 2279203252U, 2364542188U, 4209628355U },
    { 3007334859U, 2068575905U, 938758954U  },
    { 1591755985U, 760357706U,  831398279U  },
    { 1111197069U, 3288497105U, 2117667255U }
};

const uint32_t subtract_results<fifi::prime2325>::m_size =
    dimension_of(subtract_results<fifi::prime2325>::m_results);


// Invert prime2325
const expected_result_1<fifi::prime2325>
invert_results<fifi::prime2325>::m_results[] =
{
    // arg1,       result
    {          1U,          1U },
    {          2U, 2147483646U },
    { 4294967290U, 4294967290U },
    { 1909520067U,  527117708U },
    { 1879748638U, 4180067651U },
    { 3909187862U, 1505740374U },
    { 3795789800U,  915126934U },
    { 2001693299U, 2659441565U },
    { 2832328698U, 1639852349U },
    { 1444146446U,  811575873U },
    { 1353929616U, 2406667616U },
    { 1351905135U, 4218730939U },
    { 3319742702U, 2900579629U },
    { 4175463158U, 2780563524U },
    { 3477593642U, 2778157346U },
    { 2037479732U,  770197497U },
    { 2399533030U,  403603339U },
    { 1630048742U, 3481329838U },
    {  272572125U, 1887955081U },
    { 2968542519U, 2207640096U }
};

const uint32_t invert_results<fifi::prime2325>::m_size =
    dimension_of(invert_results<fifi::prime2325>::m_results);



