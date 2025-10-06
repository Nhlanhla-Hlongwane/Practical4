/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stm32f4xx.h"
#include "lcd_stm32f4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// TODO: Add values for below variables
#define NS 128 // Number of samples in LUT
#define TIM2CLK 8000000  // STM Clock frequency: Hint You might want to check the ioc file
#define F_SIGNAL 1// Frequency of output analog signal

/* USER CODE END PD */

/* Private macro ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;


/* USER CODE BEGIN PV */
// TODO: Add code for global variables, including LUTs
uint32_t Sin_LUT[NS] = {2047, 2097, 2147, 2198, 2248, 2298, 2347, 2397, 2446, 2496, 2545, 2593, 2641, 2689, 2737, 2784, 2831, 2877, 2922, 2968, 3012, 3056, 3100, 3142, 3185, 3226, 3267, 3307, 3346, 3384, 3422, 3459, 3495, 3530, 3564, 3597, 3630, 3661, 3692, 3721, 3749, 3777, 3803, 3829, 3853, 3876, 3898, 3919, 3939, 3957, 3975, 3991, 4006, 4020, 4033, 4045, 4055, 4064, 4072, 4079, 4085, 4089, 4092, 4094, 4095, 4094, 4092, 4089, 4085, 4079, 4072, 4064, 4055, 4045, 4033, 4020, 4006, 3991, 3975, 3957, 3939, 3919, 3898, 3876, 3853, 3829, 3803, 3777, 3749, 3721, 3692, 3661, 3630, 3597, 3564, 3530, 3495, 3459, 3422, 3384, 3346, 3307, 3267, 3226, 3185, 3142, 3100, 3056, 3012, 2968, 2922, 2877, 2831, 2784, 2737, 2689, 2641, 2593, 2545, 2496, 2446, 2397, 2347, 2298, 2248, 2198, 2147, 2097, 2047, 1997, 1947, 1896, 1846, 1796, 1747, 1697, 1648, 1598, 1549, 1501, 1453, 1405, 1357, 1310, 1263, 1217, 1172, 1126, 1082, 1038, 994, 952, 909, 868, 827, 787, 748, 710, 672, 635, 599, 564, 530, 497, 464, 433, 402, 373, 345, 317, 291, 265, 241, 218, 196, 175, 155, 137, 119, 103, 88, 74, 61, 49, 39, 30, 22, 15, 9, 5, 2, 0, 0, 0, 2, 5, 9, 15, 22, 30, 39, 49, 61, 74, 88, 103, 119, 137, 155, 175, 196, 218, 241, 265, 291, 317, 345, 373, 402, 433, 464, 497, 530, 564, 599, 635, 672, 710, 748, 787, 827, 868, 909, 952, 994, 1038, 1082, 1126, 1172, 1217, 1263, 1310, 1357, 1405, 1453, 1501, 1549, 1598, 1648, 1697, 1747, 1796, 1846, 1896, 1947, 1997};
uint32_t Saw_LUT[NS] = {0, 15, 31, 47, 63, 79, 95, 111, 127, 143, 159, 175, 191, 207, 223, 239, 255, 271, 287, 303, 319, 335, 351, 367, 383, 399, 415, 431, 447, 463, 479, 495, 511, 527, 543, 559, 575, 591, 607, 623, 639, 655, 671, 687, 703, 719, 735, 751, 767, 783, 799, 815, 831, 847, 863, 879, 895, 911, 927, 943, 959, 975, 991, 1007, 1023, 1039, 1055, 1071, 1087, 1103, 1119, 1135, 1151, 1167, 1183, 1199, 1215, 1231, 1247, 1263, 1279, 1295, 1311, 1327, 1343, 1359, 1375, 1391, 1407, 1423, 1439, 1455, 1471, 1487, 1503, 1519, 1535, 1551, 1567, 1583, 1599, 1615, 1631, 1647, 1663, 1679, 1695, 1711, 1727, 1743, 1759, 1775, 1791, 1807, 1823, 1839, 1855, 1871, 1887, 1903, 1919, 1935, 1951, 1967, 1983, 1999, 2015, 2031, 2047, 2063, 2079, 2095, 2111, 2127, 2143, 2159, 2175, 2191, 2207, 2223, 2239, 2255, 2271, 2287, 2303, 2319, 2335, 2351, 2367, 2383, 2399, 2415, 2431, 2447, 2463, 2479, 2495, 2511, 2527, 2543, 2559, 2575, 2591, 2607, 2623, 2639, 2655, 2671, 2687, 2703, 2719, 2735, 2751, 2767, 2783, 2799, 2815, 2831, 2847, 2863, 2879, 2895, 2911, 2927, 2943, 2959, 2975, 2991, 3007, 3023, 3039, 3055, 3071, 3087, 3103, 3119, 3135, 3151, 3167, 3183, 3199, 3215, 3231, 3247, 3263, 3279, 3295, 3311, 3327, 3343, 3359, 3375, 3391, 3407, 3423, 3439, 3455, 3471, 3487, 3503, 3519, 3535, 3551, 3567, 3583, 3599, 3615, 3631, 3647, 3663, 3679, 3695, 3711, 3727, 3743, 3759, 3775, 3791, 3807, 3823, 3839, 3855, 3871, 3887, 3903, 3919, 3935, 3951, 3967, 3983, 3999, 4015, 4031, 4047, 4063, 4079};
uint32_t Triangle_LUT[NS] = {0, 31, 63, 95, 127, 159, 191, 223, 255, 287, 319, 351, 383, 415, 447, 479, 511, 543, 575, 607, 639, 671, 703, 735, 767, 799, 831, 863, 895, 927, 959, 991, 1023, 1055, 1087, 1119, 1151, 1183, 1215, 1247, 1279, 1311, 1343, 1375, 1407, 1439, 1471, 1503, 1535, 1567, 1599, 1631, 1663, 1695, 1727, 1759, 1791, 1823, 1855, 1887, 1919, 1951, 1983, 2015, 2047, 2079, 2111, 2143, 2175, 2207, 2239, 2271, 2303, 2335, 2367, 2399, 2431, 2463, 2495, 2527, 2559, 2591, 2623, 2655, 2687, 2719, 2751, 2783, 2815, 2847, 2879, 2911, 2943, 2975, 3007, 3039, 3071, 3103, 3135, 3167, 3199, 3231, 3263, 3295, 3327, 3359, 3391, 3423, 3455, 3487, 3519, 3551, 3583, 3615, 3647, 3679, 3711, 3743, 3775, 3807, 3839, 3871, 3903, 3935, 3967, 3999, 4031, 4063, 4095, 4063, 4031, 3999, 3967, 3935, 3903, 3871, 3839, 3807, 3775, 3743, 3711, 3679, 3647, 3615, 3583, 3551, 3519, 3487, 3455, 3423, 3391, 3359, 3327, 3295, 3263, 3231, 3199, 3167, 3135, 3103, 3071, 3039, 3007, 2975, 2943, 2911, 2879, 2847, 2815, 2783, 2751, 2719, 2687, 2655, 2623, 2591, 2559, 2527, 2495, 2463, 2431, 2399, 2367, 2335, 2303, 2271, 2239, 2207, 2175, 2143, 2111, 2079, 2047, 2015, 1983, 1951, 1919, 1887, 1855, 1823, 1791, 1759, 1727, 1695, 1663, 1631, 1599, 1567, 1535, 1503, 1471, 1439, 1407, 1375, 1343, 1311, 1279, 1247, 1215, 1183, 1151, 1119, 1087, 1055, 1023, 991, 959, 927, 895, 863, 831, 799, 767, 735, 703, 671, 639, 607, 575, 543, 511, 479, 447, 415, 383, 351, 319, 287, 255, 223, 191, 159, 127, 95, 63, 31};
uint32_t Piano_LUT[] = { 1901, 1206, 1077, 2791, 1808, 1778, 1971, 1843, 1715, 2048, 1947, 1902, 1886, 1891, 1895, 1919, 1913, 1858, 1942, 1831, 1893, 1896, 1922, 1897, 1927, 1525, 1003, 1571, 1824, 2142, 1858, 1697, 1994, 2091, 1980, 1766, 1811, 1961, 1968, 1836, 1907, 2023, 1972, 2006, 1884, 1885, 2077, 2296, 1491, 2230, 2061, 1454, 2052, 2001, 1903, 1945, 1875, 1408, 1666, 1534, 1259, 2286, 1830, 1805, 1967, 1806, 1839, 1985, 1909, 1902, 1899, 1885, 1887, 2105, 1952, 1923, 1937, 1902, 1869, 1913, 1914, 1882, 1900, 1875, 1893, 1898, 1890, 1891, 1907, 1444, 1585, 1696, 1792, 1918, 1894, 1882, 1890, 1906, 2035, 2048, 2087, 2083, 1966, 1975, 1837, 2147, 1781, 2041, 2250, 1851, 2002, 1915, 1841, 1771, 1237, 2468, 1552, 2073, 1706, 2066, 1761, 1714, 1901, 2012, 1953, 1760, 1882, 1993, 1976, 1807, 2012, 1658, 2164, 1887, 1888, 1783, 2030, 1903, 1981, 1915, 1802, 1906, 1960, 1895, 1907, 1878, 1858, 1868, 1956, 1908, 1874, 1896, 1897, 2349, 2302, 1646, 2013, 2033, 1953, 1897, 1833, 1906, 1893, 2025, 1563, 2162, 1952, 1802, 1922, 1594, 1542, 1770, 1868, 2096, 2409, 1764, 1830, 2294, 2136, 1790, 2017, 1901, 1726, 1922, 2694, 2645, 2504, 2541, 1932, 1934, 1787, 2141, 1890, 1924, 1919, 1866, 1840, 1892, 1851, 1906, 1712, 1728, 1731, 1896, 1892, 1872, 1896, 1919, 1873, 1909, 1904, 1923, 1915, 1845, 1900, 1917, 2497, 2503, 2235, 2066, 1599, 2246, 1884, 1898, 1863, 1840, 2518, 1584, 1813, 1925, 1992, 1899, 1597, 1710, 1690, 1708, 1831, 1934, 1867, 1918, 2789, 1426, 2141, 1983, 1840, 1914, 1889, 1793, 2144, 2024, 1908, 1923, 1909, 1899, 1827, 1906, 904, 1421, 2696, 1877, 1795, 1839, 1923, 1672, 1991, 1996, 1915, 1888, 1912, 1875, 1906, 1901, 1850, 1968, 1872, 1885, 1913, 1941, 1888, 1905, 1541, 988, 1561, 1836, 2112, 1826, 1660, 2004, 2086, 2125, 1470, 1881, 1830, 1940, 1980, 1950, 2091, 2018, 1994, 1858, 1870, 2067, 2213, 1532, 2290, 2266, 1387, 1940, 2058, 1881, 1883, 1873, 1382, 1436, 1436, 1215, 2023, 1810, 1740, 1824, 1812, 1824, 1911, 2010, 1891, 1851, 1898, 1893, 2089, 1947, 1953, 1968, 1914, 1887, 1927, 1923, 1903, 1888, 1903, 1908, 1899, 1892, 1895, 1913, 1613, 1737, 1829, 1875, 1918, 1905, 1910, 1899, 2135, 1794, 2201, 1980, 2188, 1897, 1968, 1892, 2625, 2156, 2032, 2303, 1831, 1971, 1784, 1947, 1901, 1201, 2382, 1591, 2105, 1665, 2023, 1712, 2035, 1957, 2042, 1886, 1853, 1851, 1983, 1987, 2199, 1784, 1644, 2252, 1933, 1835, 1728, 2063, 1890, 1934, 1911, 1753, 1896, 1963, 1877, 1865, 1916, 1773, 1890, 1940, 1912, 1860, 1904, 1907, 2391, 2284, 1606, 1962, 2045, 2019, 1859, 1941, 1917, 1957, 1911, 1514, 2175, 1899, 1959, 1840, 1826, 1710, 1921, 1882, 1770, 2196, 1747, 1730, 2326, 1944, 1761, 2130, 1880, 1749, 1947, 2323, 2486, 2425, 2449, 1975, 2049, 1781, 2052, 1891, 1905, 1972, 1841, 1873, 1860, 1902, 1869, 1807, 1794, 1740, 1887, 1893, 1897, 1894, 1907, 1868, 1936, 1881, 1931, 1895, 1848, 1873, 1932, 2829, 2262, 2222, 1870, 1586, 2214, 1975, 1799, 1780, 2021, 2447, 1543, 1870, 1924, 1902, 1946, 1593, 1685, 1755, 1798, 1847, 1915, 1878, 1903, 2533, 1577, 2160, 1949, 1816, 1877, 1957, 1834, 1869, 1952, 1780, 1963, 1862, 1705, 2112, 1955, 1901 };
uint32_t Guitar_LUT[]= { 1970, 1970, 2198, 2106, 2111, 1466, 2086, 2308, 1957, 2009, 2123, 1702, 1732, 2178, 1981, 1953, 2234, 1882, 1683, 1998, 1984, 2000, 2201, 2173, 1821, 1771, 1968, 3074, 1395, 1217, 2555, 2245, 1658, 1308, 2091, 2092, 1966, 1972, 1893, 1926, 2019, 2011, 1963, 1980, 1957, 1944, 1950, 1982, 1952, 1972, 2186, 1744, 1890, 1737, 2026, 2150, 2092, 1837, 1946, 1972, 1988, 1980, 1961, 1953, 1969, 1955, 2260, 1790, 1466, 2577, 2025, 1803, 2058, 2171, 1723, 1778, 2305, 1745, 1728, 2413, 1888, 1702, 2343, 1773, 1663, 2167, 1929, 1839, 1974, 2055, 1973, 901, 2972, 2137, 2213, 2209, 1594, 1807, 2017, 2008, 1898, 1973, 1960, 1932, 2012, 1983, 1965, 1972, 1973, 1972, 1974, 1962, 1965, 1918, 2136, 1694, 1832, 1982, 1941, 2135, 2011, 2005, 1863, 1868, 1989, 1967, 1971, 1964, 1969, 1957, 2247, 1772, 1967, 1995, 2033, 2148, 2005, 1791, 1884, 2009, 2067, 2106, 1973, 1834, 1890, 1988, 2066, 2263, 2058, 1905, 2037, 2001, 1996, 2087, 2918, 1316, 1888, 1684, 2317, 2091, 1963, 1561, 2036, 1969, 2054, 1984, 1949, 1938, 1975, 1994, 1983, 1961, 1930, 1972, 1977, 1989, 2001, 1961, 1939, 1877, 1985, 1925, 1826, 2032, 2002, 1824, 2056, 2071, 1866, 1988, 1968, 1956, 1983, 1940, 1827, 2048, 2204, 1877, 1787, 1908, 2007, 2090, 2067, 1874, 1792, 1927, 2030, 2044, 2083, 1907, 1780, 2185, 2124, 2015, 2121, 1896, 1711, 2295, 2266, 2310, 2111, 2385, 1806, 1979, 2038, 2231, 1463, 1494, 2348, 1701, 1658, 2530, 1984, 1703, 2204, 2042, 1675, 1839, 2190, 1744, 1792, 2384, 1636, 2012, 2180, 1968, 1526, 2094, 2199, 1559, 1876, 2349, 1571, 2246, 1814, 1926, 2016, 2174, 1988, 1488, 1693, 2441, 2066, 2012, 2179, 1808, 1601, 2124, 2039, 1842, 2283, 2048, 1630, 2005, 1994, 2014, 2292, 2114, 1656, 1837, 2100, 1802, 2064, 2393, 1815, 1846, 2031, 1877, 1753, 2054, 1981, 2002, 1998, 1953, 1975, 1965, 1965, 1969, 2029, 1953, 1928, 1978, 1968, 1969, 1997, 2307, 2024, 1826, 1798, 1817, 2057, 2135, 2017, 1925, 1955, 1982, 1993, 1979, 1964, 1963, 1949, 2466, 1904, 1594, 2392, 1993, 1718, 2091, 2108, 1742, 1843, 2290, 1851, 1797, 2393, 1880, 1780, 2307, 1921, 1728, 1898, 2102, 1771, 1897, 2119, 1987, 1633, 2354, 2315, 1524, 2577, 1993, 1972, 1955, 1899, 1969, 2016, 2010, 1982, 1960, 1979, 1942, 1969, 1983, 1973, 1969, 1984, 1958, 1929, 2220, 1894, 2214, 2054, 2011, 1859, 1754, 1909, 1927, 2175, 1986, 1978, 1955, 1979, 1967, 1992, 2251, 1829, 1645, 1850, 2038, 2078, 2122, 1913, 1798, 1917, 1976, 2051, 2117, 1953, 1821, 1897, 1976, 2116, 2004, 1949, 1826, 1878, 1859, 2262, 1148, 2576, 1160, 1963, 2129, 2009, 1652, 2186, 1927, 1949, 1956, 1968, 1949, 1955, 1974, 1977, 2007, 1977, 1998, 1915, 1965, 1974, 1989, 1978, 1968, 1863, 1997, 1957, 1853, 2065, 2071, 1806, 2078, 2078, 1905, 1982, 1971, 1960, 1983, 1664, 1795, 2058, 2265, 2074, 1792, 1883, 1870, 2011, 2161, 2075, 1833, 1852, 1939, 2024, 2066, 2320, 1805, 1933, 1940, 1902, 2338, 2076, 1740, 1828, 1874, 1615, 2261, 1942, 1979, 1937, 2075, 1927, 1444, 1404, 2421, 1792, 1691, 2487, 2027, 1729, 1999, 2053, 1643, 1839, 2280, 1787, 1776, 2288, 2058, 1693, 2140, 2139, 1553, 2189, 1949, 1788, 2247, 2129, 1936, 1935, 2322, 2219, 1970 };
uint32_t Drum_LUT[] =  { 2047, 2052, 447, 4081, 3457, 2214, 2282, 1996, 1866, 1458, 1703, 2293, 2210, 1996, 1997, 2019, 2034, 1991, 4081, 3880, 3189, 2088, 2734, 2414, 1892, 1595, 1883, 2235, 2093, 1941, 2057, 2048, 2062, 1690, 2459, 1941, 2622, 2056, 1812, 2161, 2013, 2028, 2031, 2085, 2026, 2057, 2061, 2045, 2044, 4095, 1867, 711, 1612, 1739, 1446, 1731, 2320, 1934, 1858, 2355, 2184, 2006, 2046, 1985, 1986, 2118, 2225, 2478, 2111, 2021, 2044, 2033, 2048, 2006, 2078, 2059, 2047, 2048, 2052, 2049, 2048, 2033, 2137, 2109, 1975, 2095, 2061, 2050, 2046, 2048, 2024, 2065, 2093, 2084, 2046, 2046, 2052, 4086, 741, 2278, 1138, 2506, 2024, 1997, 2094, 2039, 2035, 2043, 2086, 2036, 2049, 2024, 2044, 2094, 2100, 2048, 2048, 2034, 2043, 2049, 2049, 4087, 4095, 4073, 1956, 1513, 2136, 1850, 1683, 58, 1510, 2683, 2356, 1101, 1306, 2025, 2361, 2130, 1886, 2156, 2180, 2002, 2001, 1980, 2045, 2070, 2045, 2191, 2060, 2073, 2063, 2033, 2034, 2041, 2052, 2060, 1992, 2047, 2049, 2042, 2052, 1779, 1725, 1890, 2258, 1807, 2184, 2006, 2044, 2038, 2055, 2038, 2059, 2056, 2055, 2047, 2027, 2382, 1955, 790, 2145, 2435, 2195, 2530, 2398, 1881, 1510, 2033, 2197, 2041, 2070, 2025, 2117, 1879, 2006, 2059, 2124, 2041, 2023, 2050, 2119, 2018, 2052, 2033, 2043, 2050, 2057, 2043, 2014, 1166, 2468, 3574, 2400, 2586, 2707, 1872, 1851, 2159, 2082, 1990, 1986, 2125, 2081, 2089, 4095, 916, 1740, 1660, 2104, 2200, 1954, 2094, 2050, 1998, 2057, 2060, 2041, 2064, 2058, 2080, 2062, 1914, 1969, 2051, 2024, 2030, 2054, 2046, 3825, 1518, 1479, 1463, 1288, 1166, 2141, 1987, 1565, 405, 0, 795, 2258, 2269, 1924, 2207, 2459, 2076, 1800, 1912, 2129, 2074, 2099, 2050, 3126, 1727, 0, 1621, 1997, 1744, 1831, 2416, 2384, 1952, 1893, 2029, 2104, 2053, 2052, 2038, 22, 2968, 2368, 2290, 1680, 2172, 2000, 2109, 2039, 2045, 2040, 2046, 2045, 2042, 2047, 2047, 4054, 4095, 2881, 2327, 3218, 2288, 1797, 1911, 2114, 2150, 1883, 1905, 2092, 2100, 2143, 2022, 1948, 1972, 2245, 2074, 2023, 2053, 2062, 2015, 2037, 2043, 2049, 2055, 2051, 2046, 2049, 2048, 1972, 2181, 1967, 1955, 1972, 2033, 2045, 2054, 2117, 2051, 2021, 2058, 2038, 2049, 2046, 1986, 1453, 2287, 1842, 2055, 2218, 1958, 2093, 2015, 2020, 2030, 2051, 2036, 2047, 2058, 2061, 1837, 2225, 2014, 2012, 2162, 2034, 2057, 2038, 2452, 349, 0, 1158, 2505, 2689, 1975, 2441, 2103, 731, 2389, 3461, 2396, 2551, 2694, 1875, 1881, 2178, 2079, 1978, 2000, 2127, 2085, 2088, 2181, 1856, 1969, 2110, 2032, 2034, 2047, 2067, 2070, 2030, 2063, 2084, 2096, 2045, 2048, 2047, 158, 2787, 1871, 2779, 1834, 2053, 2036, 2051, 2037, 2061, 2084, 2026, 2051, 2044, 2065, 2044, 4090, 2239, 2785, 2376, 1566, 1356, 2081, 1760, 1980, 2329, 2218, 2058, 1979, 2081, 2065, 2020, 2268, 2101, 2126, 2083, 2039, 2048, 2060, 2054, 2014, 2031, 2043, 2052, 2048, 2038, 2053, 2054, 430, 1712, 1913, 2183, 1244, 1451, 1925, 2470, 2313, 1887, 2042, 2144, 2049, 2020, 1999, 2031, 1412, 2562, 2641, 1925, 2190, 1892, 2139, 2014, 2100, 2016, 2052, 2033, 2056, 2029, 2032, 2048, 2356, 2216, 1982, 1957, 2094, 2060, 2042, 1968, 434, 2912, 3009, 2736, 2664, 2420, 1877, 2175, 2047 };


///hhj
// TODO: Equation to calculate TIM2_Ticks
uint32_t TIM2_Ticks = (TIM2CLK / (NS * F_SIGNAL))  ; // How often to write new LUT value
uint32_t DestAddress = (uint32_t) &(TIM3->CCR3); // Write LUT TO TIM3->CCR3 to modify PWM duty cycle


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void EXTI0_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
 
  // TODO: Start TIM3 in PWM mode on channel 3
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  // TODO: Start TIM2 in Output Compare (OC) mode on channel 1
  HAL_TIM_OC_Start(&htim2,TIM_CHANNEL_1);
  // TODO: Start DMA in IT mode on TIM2->CH1. Source is LUT and Dest is TIM3->CCR3; start with Sine LUT
 HAL_DMA_Start_IT(&hdma_tim2_ch1, (uint32_t)Sin_LUT, DestAddress , NS);

  // TODO: Write current waveform to LCD(Sine is the first waveform)
 init_LCD();
  lcd_command(CLEAR);
  lcd_command(CURSOR_HOME);
  lcd_putstring("Waveform: ");
  lcd_command(LINE_TWO);
  lcd_putstring("Sine");

  // TODO: Enable DMA (start transfer from LUT to CCR)

__HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM2_Ticks;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;

  sConfigOC.Pulse = TIM2_Ticks - 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* TIM2_CH1 DMA Init */
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_tim2_ch1.Instance = DMA1_Stream5;
  hdma_tim2_ch1.Init.Channel = DMA_CHANNEL_3;         // TIM2_CH1 is on channel 3
  hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH; // Memory -> TIM3->CCR3
  hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;    // Peripheral address fixed
  hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;        // Memory address increments
  hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim2_ch1.Init.Mode = DMA_CIRCULAR;            // Repeat LUT automatically
  hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
  {
      Error_Handler();
  }

  /* Link DMA handle to TIM2 handle */
  __HAL_LINKDMA(&htim2, hdma[TIM_DMA_ID_CC1], hdma_tim2_ch1);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 4095;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // -------------------------------
  // LCD pins configuration
  // -------------------------------
  // Configure PC14 (RS) and PC15 (E) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Configure PB8 (D4) and PB9 (D5) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configure PA12 (D6) and PA15 (D7) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set all LCD pins LOW initially
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);


  // -------------------------------
  // Button0 configuration (PA0)
  // -------------------------------
  GPIO_InitStruct.Pin = Button0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
  GPIO_InitStruct.Pull = GPIO_PULLUP;         // Use pull-up resistor
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable and set EXTI line 0 interrupt priority
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE END MX_GPIO_Init_2 */
}
volatile uint32_t lastDebounceTick = 0;
#define DEBOUNCE_DELAY 200  // ms
//extern uint32_t Sin_LUT[NS];

// Add other LUTs here
typedef enum {
    WAVE_SINE = 0,
    WAVE_SAW,
    WAVE_TRIANGLE,
    WAVE_PIANO,
    WAVE_GUITAR,
    WAVE_DRUM,
    WAVE_COUNT
} WaveformType;

volatile WaveformType currentWave = WAVE_SINE;
// enum to track which waveform is active
uint32_t* waveforms[WAVE_COUNT] = {
    Sin_LUT,     // WAVE_SINE
    Saw_LUT,     // WAVE_SAW
    Triangle_LUT,     // WAVE_TRIANGLE
    Piano_LUT,   // WAVE_PIANO
    Guitar_LUT,  // WAVE_GUITAR
    Drum_LUT     // WAVE_DRUM
};
//A table of pointers for the wave forms
const char* waveNames[WAVE_COUNT] = {
    "Sine", "Sawtooth", "Triangle", "Piano", "Guitar", "Drum"
};


/* USER CODE BEGIN 4 */
void EXTI0_IRQHandler(void){

	// TODO: Debounce using HAL_GetTick()
	// TODO: Disable DMA transfer and abort IT, then start DMA in IT mode with new LUT and re-enable transfer
	// HINT: Consider using C's "switch" function to handle LUT changes
HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); // Clear interrupt

    uint32_t tick = HAL_GetTick();
    if ((tick - lastDebounceTick) > DEBOUNCE_DELAY)
    {
        lastDebounceTick = tick;

        // Stop current DMA
        __HAL_TIM_DISABLE_DMA(&htim2, TIM_DMA_CC1);
        HAL_DMA_Abort_IT(&hdma_tim2_ch1);

        // Move to next waveform
        currentWave = (currentWave + 1) % WAVE_COUNT;

      HAL_DMA_Start_IT(&hdma_tim2_ch1,(uint32_t)waveforms[currentWave],DestAddress, NS);

        __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        // Update LCD
        lcd_command(CLEAR);
        lcd_command(CURSOR_HOME);
        lcd_putstring("Waveform:");
        lcd_command(LINE_TWO);
        lcd_putstring((char*)waveNames[currentWave]);
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
