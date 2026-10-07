# Task 3

В файле `task3.c` содержится собственная реализация rwlock. В файле `pth_ll_rwl_custom.c` содержится апробация на связном списке.

## Results

Attemps: 1

| Machine | Algorithm | Time (sec) |
| -------- |-----------|----- |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 2.115901e-01 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 1.947100e-01 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 5.633143e+00 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 5.353818e+00 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 2.825281e+00 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 2.664295e+00 |

## Logs

### Machine: Macbook air M4, MacOS 26.5.2, Chip M4, ARM64

Test pthread realization:

    ``` bash
    ./build/task3_orig 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 2.115901e-01 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

Test custom realization:

    ``` bash
    ./build/task3_custom 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 1.947100e-01 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

### Machine: Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V


Test pthread realization:

    ``` bash
    ./build/task3_orig 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 5.633143e+00 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

Test custom realization:

    ```bash 
    ./build/task3_custom 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 5.353818e+00 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

### Machine: Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V


Test pthread realization:

    ``` bash
    ./build/task3_orig 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 2.825281e+00 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

Test custom realization:

    ```bash 
    ./build/task3_custom 8
    How many keys should be inserted in the main thread?
    10000
    How many ops total should be executed?
    10000
    Percent of ops that should be searches? (between 0 and 1)
    0.2
    Percent of ops that should be inserts? (between 0 and 1)
    0.5
    Inserted 10000 keys in empty list
    Elapsed time = 2.664295e+00 seconds
    Total ops = 10000
    member ops = 2057
    insert ops = 4925
    delete ops = 3018
    ```

## Benchmarking

| Machine | Realization | Attemps | Params | Min(sec) | Max(sec) | Avg(sec) | Median(sec) |
|---------|-------------|---------|--------|----------|----------|----------|-------------|
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Pthread | 100 | 2, 10000, 10000, 0.2, 0.5 | 0.142577200 | 0.190249000 | 0.148007388 | 0.146031100 |
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Custom | 100 | 2, 10000, 10000, 0.2, 0.5 | 0.126572100 | 0.161752900 | 0.132018544 | 0.129942200 |
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 0.169235000 | 0.241265100 | 0.175419252 | 0.173445900 |
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 0.162714000 | 0.210891000 | 0.170041745 | 0.168800800 |
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 0.181911000 | 0.245281000 | 0.187118457 | 0.185747900 |
|Macbook air M4, MacOS 26.5.2, Chip M4, ARM64| Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 0.176239000 | 0.229551100 | 0.182182792 | 0.181371000 |
|Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V| Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 2.841351000 | 5.009806000 | 4.666033320 | 4.696493000 |
|Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V| Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 2.913200000 | 4.755957000 | 4.458640880 | 4.474904000 |
|Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V| Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 5.163628000 | 5.633967000 | 5.285062660 | 5.275908000 |
|Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V| Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 4.887433000 | 5.225200000 | 5.035182550 | 5.028210000 |
|Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V| Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 2.420579000 | 2.723288000 | 2.495965090 | 2.467225000 |
|Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V| Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 2.245941000 | 2.608129000 | 2.326503530 | 2.312386000 |
|Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V| Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 2.641357000 | 2.940952000 | 2.742136200 | 2.730703000 |
|Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V| Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 2.510140000 | 2.940922000 | 2.685262570 | 2.675115000 |

After fix

| Machine | Realization | Attemps | Params | Min(sec) | Max(sec) | Avg(sec) | Median(sec) |
|---------|-------------|---------|--------|----------|----------|----------|-------------|
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 2, 10000, 10000, 0.99, 0.005 | 0.048350100 | 0.094024180 | 0.055078959 | 0.054627535 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 2, 10000, 10000, 0.99, 0.005 | 0.001085043 | 0.002697945 | 0.001220498 | 0.001188397 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 2, 10000, 10000, 0.2, 0.5 | 0.140946100 | 0.424449900 | 0.153496315 | 0.145204550 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 2, 10000, 10000, 0.2, 0.5 | 0.075176000 | 0.143632900 | 0.087214967 | 0.083606485 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 4, 10000, 10000, 0.99, 0.005 | 0.028616910 | 0.060552120 | 0.031968839 | 0.029431465 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 4, 10000, 10000, 0.99, 0.005 | 0.001275063 | 0.002411127 | 0.001490798 | 0.001445532 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 0.175329200 | 0.206543900 | 0.180970830 | 0.179927950 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 0.131178900 | 0.168086100 | 0.151232538 | 0.152143450 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 8, 10000, 10000, 0.99, 0.005 | 0.023073910 | 0.027284860 | 0.024376032 | 0.024373530 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 8, 10000, 10000, 0.99, 0.005 | 0.002115965 | 0.002674818 | 0.002444680 | 0.002449036 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 0.186024000 | 0.223182900 | 0.192172443 | 0.190556000 |
| Macbook air M4, MacOS 26.5.2, Chip M4, ARM64 | Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 0.184385100 | 0.204238900 | 0.193127363 | 0.193228000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 2, 10000, 10000, 0.99, 0.005 | 1.825109000 | 3.246229000 | 2.440776410 | 1.976128500 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 2, 10000, 10000, 0.99, 0.005 | 1.837730000 | 3.221136000 | 2.479371070 | 1.980267000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 2, 10000, 10000, 0.2, 0.5 | 2.270227000 | 4.831683000 | 3.601579420 | 3.094637000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 2, 10000, 10000, 0.2, 0.5 | 2.451801000 | 4.462201000 | 3.632574690 | 3.999635500 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 4, 10000, 10000, 0.99, 0.005 | 1.770023000 | 2.845857000 | 2.467800810 | 2.501805500 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 4, 10000, 10000, 0.99, 0.005 | 1.345695000 | 1.733033000 | 1.613219180 | 1.631669000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 2.849463000 | 5.002620000 | 4.642493440 | 4.682354000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 3.846633000 | 4.795254000 | 4.463672210 | 4.477432500 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 8, 10000, 10000, 0.99, 0.005 | 2.189579000 | 2.626607000 | 2.458251140 | 2.458932500 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 8, 10000, 10000, 0.99, 0.005 | 0.945766900 | 0.973666900 | 0.960147692 | 0.960414150 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 5.086217000 | 5.646216000 | 5.297330760 | 5.287871000 |
| Banana PI 3, Ubuntu 22.04, SpacemiT K1, RISC-V | Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 4.789676000 | 5.086703000 | 4.932334860 | 4.931597000 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 2, 10000, 10000, 0.99, 0.005 | 0.726722000 | 0.765687000 | 0.731935421 | 0.731406450 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 2, 10000, 10000, 0.99, 0.005 | 0.727235100 | 0.741317000 | 0.733074887 | 0.733094550 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 2, 10000, 10000, 0.2, 0.5 | 1.568219000 | 1.768664000 | 1.607944520 | 1.593582000 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 2, 10000, 10000, 0.2, 0.5 | 1.473800000 | 1.566412000 | 1.496875600 | 1.490588500 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 4, 10000, 10000, 0.99, 0.005 | 0.729078100 | 0.793143000 | 0.764250882 | 0.765285500 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 4, 10000, 10000, 0.99, 0.005 | 0.405674000 | 0.424286100 | 0.409827951 | 0.409251450 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 4, 10000, 10000, 0.2, 0.5 | 1.760925000 | 1.990591000 | 1.830251970 | 1.821377000 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 4, 10000, 10000, 0.2, 0.5 | 1.581321000 | 1.876617000 | 1.646330100 | 1.627892500 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 8, 10000, 10000, 0.99, 0.005 | 0.697692900 | 0.765888000 | 0.728336243 | 0.726353500 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 8, 10000, 10000, 0.99, 0.005 | 0.412633200 | 0.453162900 | 0.430838393 | 0.429786550 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Pthread | 100 | 8, 10000, 10000, 0.2, 0.5 | 1.925533000 | 2.181470000 | 2.013321580 | 2.001473000 |
| Lichee PI 4a, Ubuntu 22.04, T-Head TH1520, RISC-V | Custom | 100 | 8, 10000, 10000, 0.2, 0.5 | 1.812458000 | 2.153785000 | 1.936835250 | 1.926914500 |


