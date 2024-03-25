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
| 1 | Trebuchet?! | 1.755 ms. | 1.774 ms. | **1.713 ms.** | 4.451 ms. |
| 2 | Cube Conundrum | **0.725 ms.** | 0.769 ms. | 0.767 ms. | 1.39 ms. |
| 3 | Gear Ratios | 0.22 ms. | 0.223 ms. | **0.214 ms.** | 0.398 ms. |
| 4 | Scratchcards | **0.867 ms.** | 0.886 ms. | 0.879 ms. | 1.715 ms. |
| 5 | If You Give A Seed A Fertilizer | 0.475 ms. | 0.477 ms. | **0.459 ms.** | 0.876 ms. |
| 6 | Wait For It | 0.005 ms. | 0.005 ms. | **0.004 ms.** | 0.013 ms. |
| 7 | Camel Cards | 3.737 ms. | 3.772 ms. | **3.672 ms.** | 6.918 ms. |
| 8 | Haunted Wasteland | 5.569 ms. | 5.622 ms. | **5.417 ms.** | 14.967 ms. |
| 9 | Mirage Maintenance | 1.257 ms. | 1.271 ms. | **1.227 ms.** | 2.451 ms. |
| 10 | Pipe Maze | 0.305 ms. | 0.307 ms. | **0.295 ms.** | 0.551 ms. |
| 11 | Cosmic Expansion | 0.121 ms. | 0.123 ms. | **0.116 ms.** | 0.248 ms. |
| 12 | Hot Springs | 18.43 ms. | 18.558 ms. | **17.675 ms.** | 37.574 ms. |
| 13 | Point of Incidence | 0.256 ms. | 0.258 ms. | **0.245 ms.** | 0.487 ms. |
| 14 | Parabolic Reflector Dish | 656.442 ms. | 660.78 ms. | **636.121 ms.** | 1217.44 ms. |
| 15 | Lens Library | 0.649 ms. | 0.65 ms. | **0.63 ms.** | 1.193 ms. |
| 16 | The Floor Will Be Lava | 78.854 ms. | 79.674 ms. | **74.122 ms.** | 100.062 ms. |
| 17 | Clumsy Crucible | 44.728 ms. | 45.434 ms. | **41.901 ms.** | 90.54 ms. |
| 18 | Lavaduct Lagoon | 0.692 ms. | 0.68 ms. | **0.67 ms.** | 1.242 ms. |
| 19 | Aplenty | 0.685 ms. | 0.685 ms. | **0.659 ms.** | 1.379 ms. |
| 20 | Pulse Propagation | 21.666 ms. | 21.812 ms. | **20.945 ms.** | 40.409 ms. |
| 21 | Step Counter | 501.108 ms. | 480.054 ms. | **417.734 ms.** | 965.769 ms. |
| 22 | Sand Slabs | 20.929 ms. | 20.99 ms. | **20.427 ms.** | 41.423 ms. |
| 23 | A Long Walk | 42.504 ms. | 42.844 ms. | **41.164 ms.** | 80.178 ms. |
| 24 | Never Tell Me The Odds | 22.868 ms. | 23.048 ms. | **22.168 ms.** | 38.229 ms. |
| 25 | Snowverload | 79.159 ms. | 77.152 ms. | **75.597 ms.** | 154.219 ms. |
