# AoC-2023-cplusplus

**ARM SBC Run Times**

| Model | Core | 
| --- | --- |
| radxa rock 5b, orange pi5 | 4 x cortex-a76 (2.4GHz), 4 x cortex-a55 (1.8GHz) |
| raspberry pi5 | 4 x cortex-a76 (2.4 GHz) |
| raspberry pi4b | 4 x cortex-a72 (1.8 GHz) |

Compiler: g++ (GCC) 14.0.1 20240208 (experimental)

with -O3 -std=c++23 -lpthread options

For puzzle definition see https://adventofcode.com/2023

| Day | Title | rock5b | opi5 | pi5 | pi4b |
| --- | --- | --- | --- | --- | --- |
| 1 | Trebuchet?! | 1.795 ms. | 1.818 ms. | **1.736 ms.** | 3.627 ms. |
| 2 | Cube Conundrum | 0.921 ms. | 0.92 ms. | **0.889 ms.** | 1.709 ms. |
| 3 | Gear Ratios | 0.342 ms. | 0.34 ms. | **0.293 ms.** | 0.593 ms. |
| 4 | Scratchcards | 1.054 ms. | 1.059 ms. | **0.962 ms.** | 2.023 ms. |
| 5 | If You Give A Seed A Fertilizer | 0.548 ms. | 0.551 ms. | **0.506 ms.** | 1.091 ms. |
| 6 | Wait For It | 0.081 ms. | 0.08 ms. | **0.066 ms.** | 0.204 ms. |
| 7 | Camel Cards | 4.206 ms. | 4.211 ms. | **4.001 ms.** | 7.874 ms. |
| 8 | Haunted Wasteland | 5.81 ms. | 5.806 ms. | **5.567 ms.** | 14.126 ms. |
| 9 | Mirage Maintenance | 1.403 ms. | 1.398 ms. | **1.322 ms.** | 2.822 ms. |
| 10 | Pipe Maze | 0.439 ms. | 0.428 ms. | **0.378 ms.** | 0.843 ms. |
| 11 | Cosmic Expansion | 0.221 ms. | 0.222 ms. | **0.188 ms.** | 0.379 ms. |
| 12 | Hot Springs | 21.871 ms. | 24.132 ms. | **18.001 ms.** | 38.167 ms. |
| 13 | Point of Incidence | 0.348 ms. | 0.351 ms. | **0.303 ms.** | 0.645 ms. |
| 14 | Parabolic Reflector Dish | 648.249 ms. | 657.557 ms. | **634.065 ms.** | 1210.36 ms. |
| 15 | Lens Library | 0.914 ms. | 0.888 ms. | **0.762 ms.** | 1.792 ms. |
| 16 | The Floor Will Be Lava | 59.187 ms. | 60.378 ms. | **56.678 ms.** | 78.518 ms. |
| 17 | Clumsy Crucible | 45.951 ms. | 46.146 ms. | **42.194 ms.** | 86.432 ms. |
| 18 | Lavaduct Lagoon | 0.781 ms. | 0.787 ms. | **0.761 ms.** | 1.557 ms. |
| 19 | Aplenty | 0.984 ms. | 0.991 ms. | **0.871 ms.** | 1.933 ms. |
| 20 | Pulse Propagation | 22.573 ms. | 22.891 ms. | **22.024 ms.** | 43.229 ms. |
| 21 | Step Counter | 524.63 ms. | 495.154 ms. | **442.973 ms.** | 1009.45 ms. |
| 22 | Sand Slabs | 21.101 ms. | 21.376 ms. | **20.236 ms.** | 40.206 ms. |
| 23 | A Long Walk | 42.244 ms. | 42.739 ms. | **40.948 ms.** | 80.318 ms. |
| 24 | Never Tell Me The Odds | 26.127 ms. | 23.268 ms. | **22.324 ms.** | 38.959 ms. |
| 25 | Snowverload | 78.223 ms. | 76.798 ms. | **73.474 ms.** | 157.066 ms. |
