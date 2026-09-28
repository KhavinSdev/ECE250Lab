#include <iostream>
#include <sstream>
#include <fstream>
#include "data.hpp"


int main()
{
    // initialises a timeseries instance for testing
    TimeSeries test;

    while(true) {
        

        std::string input_command;
        std::getline(std::cin, input_command);
        
        
        // stringstream allows accessing space seperated arguments of input
        std::stringstream ss(input_command);
        std::string command_name;

        ss >> command_name;

        // parses input command - runs respective functions based on command arguments :
        if (command_name == "LOAD_P1") {
            std::cout << "success" << std::endl;

            std::string filepath;
            ss >> filepath;

            // std::cout << "filepath: " << filepath << std::endl;
            test.read_data(filepath);
            
        }
        
        if (command_name == "PRINT_P1") {

            test.print();
        }

        if (command_name == "ADD_P1") {

            std::string Y;
            std::string D;
            ss >> Y >> D;

            test.add(Y, D);
        }

        if (command_name == "UPDATE_P1") {

            std::string Y;
            std::string D;
            ss >> Y >> D;  

            test.update(Y, D);
        }

        if (command_name == "MEAN_P1") {
            
            double mean = test.data_mean();
            if(mean == 0) {
                std::cout << "failure" << std::endl;
            }
            else {
                std::cout << "mean is " << mean << std::endl;
            }

        }

        if (command_name == "MONOTONIC_P1") {

            if(test.get_valid_data_count() < 1) {
                std::cout << "failure" << std::endl;
            }

            else {
                bool result = test.is_monotonic();

                if(result) {
                    std::cout << "series is monotonic" << std::endl;
                }
                else {
                    std::cout << "series is not monotonic" << std::endl;
                }
            }


        }
        if (command_name == "FIT_P1") {
            double m;
            double b;
            bool result = test.best_fit(m, b);

            if(result) {
                std::cout << "slope is " << m << " intercept is " << b << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }

        }
        
        if (command_name == "EXIT") {
            break;
        }

   

    }
    
    

    return 0;
  
}

