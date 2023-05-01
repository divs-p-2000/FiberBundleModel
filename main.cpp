#include <iostream>
#include "GLSBundle.h"
#include "LLSBundle.h"
#include "functions.h"
#include "Simulation.h"
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

int main(int argc, char* argv[]) {

    if(argc != 4) {
        std::cout << "Please mention valid input and output file names and number of simulations as arguments. . ." << std::endl;
        return 1;
    }

    std::ifstream infile;
    std::ofstream outfile;

    infile.open(argv[1], std::ios_base::in);
    if(!infile.is_open()) {
        std::cout << "ERROR: Unable to open file: " << argv[1] << std::endl;
        return 1;
    }

    outfile.open(argv[2], std::ios_base::out);
    if(!outfile.is_open()) {
        std::cout << "ERROR: Unable to open file: " << argv[1] << std::endl;
        return 1;
    }

    int numberOfSimulations = std::stoi(argv[3]);

    std::string buffer;
    std::getline(infile, buffer);

    outfile << std::fixed;
    outfile << "BundleSize,IncrementalLoad,Alpha,Beta,Delta,Cutoff,BreakingTime,AverageAvalancheSize\n";

    while(std::getline(infile, buffer)) {
        std::cout << "Evaluating tuple: " << buffer << '\n';
        
        std::stringstream bufferStream(buffer);

        int N;
        double increment, alpha, beta, gamma, delta;

        // bufferStream >> N >> increment >> alpha >> beta >> gamma >> delta;
        bufferStream >> N >> increment >> alpha >> beta >> delta;

        // double maxIntervalLimit = 0;

        // if(beta > 1) {
        //     maxIntervalLimit = sqrt(M_PI) * pow((double) beta / (double) gamma, (double) 1 / (double) gamma);
        // }

        // else {
        //     maxIntervalLimit = (sqrt(M_PI) * gamma) / pow(2.0f, gamma / 2.0f);
        // }

        // Newton Raphson Iterations
        double maxIntervalLimit = 2 * pow((beta + 2.0)/(beta - 1.0), (double) 1 / (double) (beta - 1.0));
        double convConst = 2 * alpha * pow(beta / (M_E * (beta - 1)), beta / (beta - 1));
        double peak = pow(beta / (beta - 1), 1 / (beta - 1));

        FBM::ExponentialDistribution testFunc(alpha, beta, beta - 1, delta);

        double shift_prev = 0.0, shift = 0.0;

        for(int i = 0; i < 10; ++i) {
            shift_prev = shift;
            shift = (testFunc(maxIntervalLimit) - convConst) / testFunc.derivative(maxIntervalLimit);
            
            std::cout << "Guess: " << maxIntervalLimit << std::endl;
            std::cout << "Shift: " << shift << std::endl;

            if(shift_prev == 0.0) {
                if(fabs(shift / maxIntervalLimit) > 1) break;
            }
            else {
                if(fabs(shift / shift_prev) > 1) break;
            }

            if(maxIntervalLimit - shift < peak) break;

            maxIntervalLimit = maxIntervalLimit - shift;
        }

        maxIntervalLimit += 0.001;
        double breakingTimePopulationSum {0.0}, averageAvalancheSizeSum {0.0};

        // LLS code
        for(int i = 0; i < numberOfSimulations; ++i) {
            FBM::LLSBundle testBundle(N, 0.0, maxIntervalLimit,
            FBM::ExponentialDistribution(alpha, beta, beta - 1, delta),
            FBM::EuclideanTransferFunction((double) 1 / (double) N, 1),
            FBM::SquareLatticeGenerator45()
            );

            FBM::Simulation testSimulation(testBundle, delta, increment);

            testSimulation.run();

            breakingTimePopulationSum += testSimulation.timeElapsed();
            averageAvalancheSizeSum += (double) N / (double) testSimulation.totalAvalanches();

        }
        double meanBreakingTime = breakingTimePopulationSum / numberOfSimulations;
        double meanAverageAvalancheSize = averageAvalancheSizeSum / numberOfSimulations;
        outfile << N << ',' << increment << ',' 
                << alpha << ',' << beta << ',' << delta << ',' << maxIntervalLimit << ','
                << meanBreakingTime << ',' << meanAverageAvalancheSize << '\n';
        // outfile << N << ',' << increment << ',' 
        //         << alpha << ',' << beta << ',' << gamma << ',' << delta << ',' 
        //         << meanBreakingTime << ',' << meanAverageAvalancheSize << '\n';
    }

    //     // GLS code
    //     FBM::GLSBundle testBundle(N, 0.0, maxIntervalLimit, FBM::ExponentialDistribution(alpha, beta, beta - 1, delta));
    //     FBM::Simulation testSimulation(testBundle, delta, increment);
    //     testSimulation.run();
    //     double meanBreakingTime = testSimulation.timeElapsed();
    //     double meanAverageAvalancheSize = (double) N / (double) testSimulation.totalAvalanches();

    //     outfile << N << ',' << increment << ',' 
    //             << alpha << ',' << beta << ',' << delta << ',' << maxIntervalLimit << ','
    //             << meanBreakingTime << ',' << meanAverageAvalancheSize;
    //     // outfile << N << ',' << increment << ',' 
    //     //         << alpha << ',' << beta << ',' << gamma << ',' << delta << ',' 
    //     //         << meanBreakingTime << ',' << meanAverageAvalancheSize;
        
    //     for (auto avalanche : testSimulation.avalancheList()) {
    //         if (avalanche > 0) outfile << ',' << avalanche;
    //     }

    //     outfile << '\n';
    // }

    infile.close();
    if(infile.is_open()) {
        std::cout << "ERROR: Unable to close file: " << argv[1] << std::endl;
        return 1;
    }

    outfile.close();
    if(outfile.is_open()) {
        std::cout << "ERROR: Unable to close file: " << argv[2] << std::endl;
        return 1;
    }

    return 0;

    // 220 0.00488619 298 1.98 3.98 298

    // int N = 1000;
    // double increment = 0.01, alpha = 250, beta = 1.75, gamma = 0.75, delta = 100;
    // // int N = 220;
    // // double increment = 0.01, alpha = 1, beta = 2, gamma = 1, delta = 0;
    // // double maxIntervalLimit = 2 * pow((double) beta / (double) gamma, (double) 1 / (double) gamma);

    // double maxIntervalLimit = 2 * pow((beta + 2.0)/(beta - 1.0), (double) 1 / (double) (beta - 1.0));
    
    // // Newton Raphson Iterations

    // double convConst = 2 * alpha * pow(beta / (M_E * (beta - 1)), beta / (beta - 1));
    // FBM::ExponentialDistribution testFunc(alpha, beta, gamma, delta);

    // for(int i = 0; i < 10; ++i) {
    //     std::cout << "Guess: " << maxIntervalLimit << std::endl;
    //     double shift = (testFunc(maxIntervalLimit) - convConst) / testFunc.derivative(maxIntervalLimit);
    //     std::cout << "Shift: " << shift << std::endl;

    //     if(fabs(shift / maxIntervalLimit) > 1) break;

    //     maxIntervalLimit = maxIntervalLimit - shift;
    // }
    // maxIntervalLimit += 0.001;

    // FBM::LLSBundle testBundle(N, 0.0, maxIntervalLimit,
    // FBM::ExponentialDistribution(alpha, beta, gamma, delta),
    // FBM::EuclideanTransferFunction((double) 1 / (double) N, 1),
    // FBM::SquareLatticeGenerator45()
    // );

    // // FBM::GLSBundle testBundle(N, 0.0, maxIntervalLimit, FBM::ExponentialDistribution(alpha, beta, gamma, delta));
    // FBM::Simulation testSimulation(testBundle, delta, increment);
    
    // testSimulation.run();

    // auto breakingTimes = testSimulation.breakingTimes();
    // for(const auto& e : breakingTimes){
    //     std::cout << e.first  << ' ' << e.second << '\n';
    // }
    // std::cout << "Applied Load :" << testSimulation.appliedLoad() << std::endl;


    // return 0;
}