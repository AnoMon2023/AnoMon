#include <bits/stdc++.h>
#include "Benchmark_kll.h"
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

int main(int argc, char** argv)
{
    double epsilon = 0.01;
    
    std::string path_seattle = "../dataset/Seattle/SeattleData_all";
    std::string path_caida = "/share/datasets/CAIDA2018/dataset/130000.dat";
    std::string path_webget = "../dataset/5_zendo/webget-all-simplify.dat";

    // SeattleBenchmark benchmark(path_seattle);
    //WebgetBenchmark benchmark(path_webget);
    CAIDABenchmark benchmark(path_caida);
    benchmark.Run(atoi(argv[1]),atoi(argv[2]));
    
}
