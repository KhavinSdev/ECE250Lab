#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Project1/data.hpp"

class TSContainer {

    private:

        TimeSeries *array_ptr = nullptr;
        std::string country_name_{""};
        std::string country_code_{""};
        int size_{0};
        int cap_{0};

    public:

        // void load_country_series(std::string country_name);
        // void list();
        // void add(std::string Series_Code, std::string Y, std::string D);
        // void update(std::string Series_Code, std::string Y, std::string D);
        // void print_series(std::string Series_Code);
        // void delete_series(std::string Series_Code);
        // void biggest();
        // void TS(std::string Series_Code);


        TSContainer() {
            array_ptr = new TimeSeries[10];
            cap_ = 10;
        }

        ~TSContainer() {

  
            delete[] array_ptr;
            array_ptr = nullptr;

            
        }

        
        void reset() {
            delete[] array_ptr;
            array_ptr = nullptr;

            array_ptr = new TimeSeries[10];
            cap_ = 10;
            size_ = 0;
            country_name_ = "";
            country_code_ = "";
        }

        void extend_collection() {
            int new_cap = cap_ * 2;

            TimeSeries* new_array_ptr = new TimeSeries[new_cap];

            for(int i = 0; i < size_; i = i + 1) {
                new_array_ptr[i] = TimeSeries(array_ptr[i]);
            }

            delete[] array_ptr;
            array_ptr = nullptr;
            array_ptr = new_array_ptr;
            cap_ = new_cap;


        }

        void load_country_series(std::string country_name) {
            std::fstream file;
            std::string data_line;
            std::string data_item;

            reset();

            file.open("../lab2_multidata.csv");

            if (!file.is_open()) {
                std::cerr << "Failed to open file: lab2_multidata.csv" << "\n";
                std::cerr << std::filesystem::current_path() << std::endl;
                return;
            }

            if(country_name == "") {
                file.close();
                return;
            }

            int loop_counter1 = 0;
            while(std::getline(file, data_line, '\n')) {
                std::stringstream string_stream(data_line);

                
                while(std::getline(string_stream, data_item, ',')) {

                    // skips lines that are not associated with specified country
                    if(data_item != country_name && loop_counter1 == 0){
                        break;
                    }

                    if(loop_counter1 == 1) {
                        country_code_ = data_item;
                        country_name_ = country_name;
                        break;
                    }
                    
                    loop_counter1 = loop_counter1 + 1;


                }

                if(loop_counter1 == 1) {
                    break;
                }
            }

            file.seekg(0);

            int i = 0;
            while(std::getline(file, data_line, '\n')) {
                std::stringstream string_stream(data_line);

                while(std::getline(string_stream, data_item, ',')) {

                    if(data_item != country_name) {
                        break;
                    }
                    
                    if(size_ == cap_) {
                        extend_collection();
                    }

                    array_ptr[i].read_data(data_line, true);
                    i = i + 1;
                    size_ = size_ + 1;
                    break;
                    
                }
            }

            file.close();
            



        }

        void list() {

            if (country_name_ == "") {return;}
            std::cout << country_name_;
            std::cout << " " << country_code_ << " ";
            for(int i = 0; i < size_; i = i + 1) {
                 std::cout << array_ptr[i].get_series_name() << " ";
            }
            std::cout << std::endl;
        }

        bool add(std::string Series_Code, std::string Y, std::string D) {
            for(int i = 0; i < size_; i = i + 1) {
                if (array_ptr[i].get_series_code() == Series_Code){

                    if(array_ptr[i].add(Y, D)) {
                        return true;
                    }
                    return false;
                }
                else{
                    continue;
                }
            }
            return false;
        }

        bool update(std::string Series_Code, std::string Y, std::string D) {
            for(int i = 0; i < size_; i = i + 1) {
                    if (array_ptr[i].get_series_code() == Series_Code){

                        if(array_ptr[i].update(Y, D)) {
                            return true;
                        }
                        return false;
                    }
                    else{
                        continue;
                    }
                }
            return false;
            
                
        }  
        
        bool print(std::string Series_Code) {

            for(int i = 0; i < size_; i = i + 1) {
                if (array_ptr[i].get_series_code() == Series_Code){

                    if(!(array_ptr[i].print())) {
                        return false;
                    }
                    return true;
                }
                else{
                    continue;
                }
            }
            return false;
        }

        bool delete_timeseries(std::string Series_Code) {
            for(int i = 0; i < size_; i = i + 1) {

                    // if series with provided series code found
                    if (array_ptr[i].get_series_code() == Series_Code){
                        // std::cout << "success" << std::endl;

                        // case if last series to be deleted
                        if(i == (size_ - 1)) {

                        }
                        // shift all series following after's index position back by 1 to effectively delete the specified series
                        else{
                            for(int j = i + 1; j < size_; j = j + 1) {
                                array_ptr[j - 1] = array_ptr[j];
                            } 
                        }

                        size_ = size_ - 1;
                        return true;
                    }
                    else{
                        continue;
                    }
                }

            // std::cout << "failure" << std::endl;
            return false;
        }
        
        std::string biggest() {
            int no_validcount = 0;
            double biggest_mean = 0;
            int biggest_mean_index = 0;

            for(int i = 0; i < size_; i = i + 1) {
                double current_mean = array_ptr[i].data_mean();

                if(current_mean == 0) {
                    no_validcount = no_validcount + 1;
                }
                else if(current_mean > biggest_mean) {
                    biggest_mean = current_mean;
                    biggest_mean_index = i;
                }

            }

            if(no_validcount == size_) {
                return "failure";
            }

            return array_ptr[biggest_mean_index].get_series_code();

        }
        
        bool TS(std::string Series_Code) {

            for(int i = 0; i < size_; i = i + 1) {
                if (array_ptr[i].get_series_code() == Series_Code){
                    std::cout << "size is " << array_ptr[i].get_array_size() << " capacity is " << array_ptr[i].get_array_cap() << std::endl;
                    return true;
                    
                }
                else{
                    continue;
                }
            }
            return false;
        }
    };

        

int main() {

    TSContainer timerseries_collection = TSContainer();

    while(true) {
        

        std::string input_command;
        std::getline(std::cin, input_command);
        
        
        // stringstream allows accessing space seperated arguments of input
        std::stringstream ss(input_command);
        std::string command_name;

        ss >> command_name;

        if (command_name == "LOAD_P2") {
            std::cout << "success" << std::endl;

            std::string country_name;
            std::getline(ss >> std::ws, country_name, '\0');
            

            // std::cout << "filepath: " << filepath << std::endl;
            timerseries_collection.load_country_series(country_name);
        }

        if (command_name == "LIST_P2") {
        
            timerseries_collection.list();
        }

        if (command_name == "ADD_P2") {
            std::string Series_Code;
            std::string Y;
            std::string D;

            ss >> Series_Code >> Y >> D;

            if(timerseries_collection.add(Series_Code, Y, D)) {
                std::cout << "success" << std::endl;
            }
            else {
                std::cout << "failure" << std::endl;
            }
        }

        if (command_name == "UPDATE_P2") {
            std::string Series_Code;
            std::string Y;
            std::string D;

            ss >> Series_Code >> Y >> D;

            if(timerseries_collection.update(Series_Code, Y, D)) {
                std::cout << "success" << std::endl;
            }
            else {
                std::cout << "failure" << std::endl;
            }
        }

        if (command_name == "PRINT_P2") {
            std::string Series_Code;

            ss >> Series_Code;

            if(!timerseries_collection.print(Series_Code)) {
                std::cout << "failure" << std::endl;
            }
        }

        if (command_name == "DELETE_P2") {
            std::string Series_Code;
            ss >> Series_Code;

            if(timerseries_collection.delete_timeseries(Series_Code)) {
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }

        }

        if (command_name == "BIGGEST_P2") {
            std::cout << timerseries_collection.biggest() << std::endl;
        }

        if (command_name == "TS_P2") {

            std::string Series_Code;
            ss >> Series_Code;

            if(!timerseries_collection.TS(Series_Code)) {
                std::cout << "failure" << std::endl;
            }

            
        }

        if (command_name == "EXIT") {
        
            break;
        }

    }
}