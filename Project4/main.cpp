#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "dataP1.hpp"
#include "tscontainer.hpp"
#include "DataTree.hpp"
#include "AllTimeSeries.hpp"
 


int main() {

    AllTimeSeries allTimeSeries = AllTimeSeries();

    freopen("test_custom.in", "r", stdin);

    while(true) {
        

        std::string input_command;
        std::getline(std::cin, input_command);
        
        
        // stringstream allows accessing space seperated arguments of input
        std::stringstream ss(input_command);
        std::string command_name;

        ss >> command_name;

        if (command_name == "LOAD_P3") {
            std::cout << "success" << std::endl;

            allTimeSeries.load();
            
        }

        else if (command_name == "LIST_P3") {
            
            std::string Country_Name;
            std::getline(ss >> std::ws, Country_Name, '\0');

            allTimeSeries.list(Country_Name);
        }

        else if (command_name == "COUNTRY_MIN_P3") {

            std::string Country_Code;
            ss >> Country_Code;

            if(!allTimeSeries.country_min(Country_Code)) {
                std::cout << "failure" << std::endl;
            }

        }

        else if (command_name == "RANGE_P3") {
            std::string Series_Code;
            ss >> Series_Code;

            allTimeSeries.range(Series_Code);
        }

        else if (command_name == "BUILD_P3") {
            std::string Series_Code;

            ss >> Series_Code;
            std::cout << "success" << std::endl;
            
            allTimeSeries.build_tree(Series_Code);

            
            
        }

        else if (command_name == "FIND_P3") {
            std::string mean;
            std::string operation;
            ss >> mean >> operation;



            allTimeSeries.find_tree(std::stod(mean), operation);
            

        }

        else if (command_name == "DELETE_P3") {
            std::string Country_Name;
            std::getline(ss >> std::ws, Country_Name, '\0');

            allTimeSeries.delete_tree(Country_Name);
        }

        else if (command_name == "LIMITS_P3") {

            std::string condition;
            ss >> condition;

            allTimeSeries.limits_tree(condition);

            
        }

        else if (command_name == "TRACE_P3") {
            
            std::string Country_Name;
            std::getline(ss >> std::ws, Country_Name, '\0');

            allTimeSeries.trace_tree(Country_Name);
        }

        else if (command_name == "LOOKUP_P4") {
            
            std::string Country_Code;
            ss >> Country_Code;

            auto [x,y] = allTimeSeries.search_hash_table(Country_Code);

            if(x == -1) {
                std::cout << "failure" << std::endl;
            }
            else {
                std::cout << "index " << x << " searches " << y << std::endl;
            }
        }

        else if (command_name == "REMOVE_P4") {
            
            std::string Country_Code;
            ss >> Country_Code;

            if(allTimeSeries.remove_from_hashtable(Country_Code)) {
                std::cout << "success" << std::endl;
            }

            else {
                std::cout << "failure" << std::endl;
            }
        }

        else if (command_name == "INSERT_P4") {
            
            std::string Country_Code;
            ss >> Country_Code;

            if(allTimeSeries.insert_into_hashtable(Country_Code)) {
                std::cout << "success" << std::endl;
            }

            else {
                std::cout << "failure" << std::endl;
            }
        }

        else if (command_name == "CLEAN_P4") {
            allTimeSeries.clean_hashtable();
            std::cout << "success" << std::endl; 
        }

        else if (command_name == "EXIT") {
        
            break;
        }

    }
}

