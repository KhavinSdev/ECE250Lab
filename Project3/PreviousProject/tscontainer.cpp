#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "tscontainer.hpp"


// constructor and destructor
TSContainer::TSContainer() {
    array_ptr = new TimeSeries[10];
    cap_ = 10;
}

TSContainer::~TSContainer() {


    delete[] array_ptr;
    array_ptr = nullptr;

    
}

TSContainer& TSContainer::operator=(const TSContainer& other) {
    if (this == &other) return *this;
    
    delete[] array_ptr;  // free existing allocation
    
    cap_ = other.cap_;
    size_ = other.size_;
    country_name_ = other.country_name_;
    country_code_ = other.country_code_;
    
    array_ptr = new TimeSeries[cap_];
    
    for(int i = 0; i < size_; i = i + 1) {
        array_ptr[i] = other.array_ptr[i];
    }
    
    return *this;
}

// resets TSContainer object to default configuration of member variables
void TSContainer::reset() {
    delete[] array_ptr;
    array_ptr = nullptr;

    array_ptr = new TimeSeries[10];
    cap_ = 10;
    size_ = 0;
    country_name_ = "";
    country_code_ = "";
}

// expands the collection capacity by allocating and copying series from the old to new container before deleting the old container
void TSContainer::extend_collection() {
    int new_cap = cap_ * 2;

    TimeSeries* new_array_ptr = new TimeSeries[new_cap];

    for(int i = 0; i < size_; i = i + 1) {
        //P3 - edit removed TimeSeries()
        new_array_ptr[i] = array_ptr[i];
    }

    delete[] array_ptr;
    array_ptr = nullptr;
    array_ptr = new_array_ptr;
    cap_ = new_cap;


}

// loads data from the dataset into the TSContainer TimeSeries array
void TSContainer::load_country_series(std::string country_name) {
    std::fstream file;
    std::string data_line;
    std::string data_item;

    // resets timeseries collection everytime load_country_series is called as per requirement
    reset();

    // opens dataset file
    
    file.open("lab2_multidata.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file: lab2_multidata.csv" << "\n";
        std::cerr << std::filesystem::current_path() << std::endl;
        return;
    }

    // exits if no country name is provided in LOAD_P2 command
    if(country_name == "") {
        file.close();
        return;
    }

    // Loop that stores the metadata of the TimeSeries collection (country name and country code)
    int loop_counter1 = 0;
    while(std::getline(file, data_line, '\n')) {
        std::stringstream string_stream(data_line);

        //parses comma seperated line from the data source
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
        // stops parsing data set once confirmed that required metadata has been stored for use 
        if(loop_counter1 == 1) {
            break;
        }
    }

    // moves the file reader to the start so that we don't skip the first line when reading timeseries data
    file.seekg(0);

    // parses data set line by line to read TimeSeries' data for a given country only
    int i = 0;
    while(std::getline(file, data_line, '\n')) {
        std::stringstream string_stream(data_line);

        while(std::getline(string_stream, data_item, ',')) {

            // moves to next line if current line data not for the given country
            if(data_item != country_name) {
                break;
            }
            
            // extends the capacity for the TimeSeries array as required to store more data
            if(size_ == cap_) {
                extend_collection();
            }

            // calls read_data of project 1 to load the TimeSeries data into the TimeSeries object, directly loads from the string data_line 
            array_ptr[i].read_data(data_line, true);
            i = i + 1;
            size_ = size_ + 1;
            break;
            
        }
    }

    // closes file
    file.close();
    



}

// P3 modification - loads data from a stringstream object passed by reference
void TSContainer::load_country_series(std::stringstream& string_stream) {

    // extends the capacity for the TimeSeries array as required to store more data
    if(size_ == cap_) {
        extend_collection();
    }
    // calls read_data of project 1 to load the TimeSeries data into the TimeSeries object, directly loads from the stringstream 
    array_ptr[size_].read_data(string_stream);
    size_ = size_ + 1;


}


// lists the series names of all timeseries for a given country

void TSContainer::list() {
    
    // early exit if country name is empty
    if (country_name_ == "") {return;}
    std::cout << country_name_;
    std::cout << " " << country_code_ << " ";

    // prints all the series names of the TimeSeries collection
    for(int i = 0; i < size_; i = i + 1) {
            std::cout << array_ptr[i].get_series_name() << " ";
    }
    std::cout << std::endl;
}

// adds year and data pair to the timeseries with given series code
bool TSContainer::add(std::string Series_Code, std::string Y, std::string D) {

    // finds the timeseries for the code
    for(int i = 0; i < size_; i = i + 1) {
        if (array_ptr[i].get_series_code() == Series_Code){

            // calls add from project 1 to add the data to the timeseries
            if(array_ptr[i].add(Y, D)) {
                return true;
            }
            // returns false if add fail
            return false;
        }
        else{
            continue;
        }
    }

    // returns false if series not present in the collection
    return false;
}

// updates year and data pair of a timeseries with the given series code 
bool TSContainer::update(std::string Series_Code, std::string Y, std::string D) {

    // finds the timeseries for the code
    for(int i = 0; i < size_; i = i + 1) {
            if (array_ptr[i].get_series_code() == Series_Code){
                
                // calls update from project 1 to update the series with the given code
                if(array_ptr[i].update(Y, D)) {
                    return true;
                }
                // if update fails
                return false;
            }
            else{
                continue;
            }
        }
    // if series not found in the collection
    return false;
    
        
}  

// prints the year and data pairs for a timeseries with given code
bool TSContainer::print(std::string Series_Code) {

    // finds the timeseries for the code
    for(int i = 0; i < size_; i = i + 1) {
        if (array_ptr[i].get_series_code() == Series_Code){

            // calls print() from project 1 to print the year and data pairs for the timeseries
            if(!(array_ptr[i].print())) {
                return false;
            }
            // passes if print is succesful
            return true;
        }
        else{
            continue;
        }
    }
    // fails if timeseries not found in collection of series
    return false;
}

// deletes a timeseries with given code from the collection
bool TSContainer::delete_timeseries(std::string Series_Code) {
    for(int i = 0; i < size_; i = i + 1) {

            // if series with provided series code found
            if (array_ptr[i].get_series_code() == Series_Code){


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

    // fails if series not found in the collection
    return false;
}

// finds the series with the biggest mean and returns its series code
std::string TSContainer::biggest() {
    int no_validcount = 0;
    double biggest_mean = 0;
    int biggest_mean_index = 0;

    // calculates means for all the series in the collection and stores the biggest
    for(int i = 0; i < size_; i = i + 1) {
        double current_mean = array_ptr[i].data_mean();

        // notes invalid timeseries (those that have no valid data at all)
        if(current_mean == 0) {
            no_validcount = no_validcount + 1;
        }

        // updates the biggest mean
        else if(current_mean > biggest_mean) {
            biggest_mean = current_mean;
            biggest_mean_index = i;
        }

    }

    // fails if none of the series have valid data
    if(no_validcount == size_) {
        return "failure";
    }

    // returns series code of the series with the biggest mean
    return array_ptr[biggest_mean_index].get_series_code();

}


// finds the series with the smallest mean and returns its series code
std::string TSContainer::smallest() {
    int no_validcount = 0;
    double smallest_mean = std::numeric_limits<double>::max();
    int smallest_mean_index = 0;

    // calculates means for all the series in the collection and stores the smallest
    for(int i = 0; i < size_; i = i + 1) {
        double current_mean = array_ptr[i].data_mean();

        // notes invalid timeseries (those that have no valid data at all)
        if(current_mean == 0) {
            no_validcount = no_validcount + 1;
        }

        // updates the biggest mean
        else if(current_mean < smallest_mean) {
            smallest_mean = current_mean;
            smallest_mean_index = i;
        }

    }

    // fails if none of the series have valid data
    if(no_validcount == size_) {
        return "failure";
    }

    // returns series code of the series with the biggest mean
    return array_ptr[smallest_mean_index].get_series_code();

}

double TSContainer::mean_of_series(std::string Series_Code) const{

    for(int i = 0; i < size_; i = i + 1) {
        if (array_ptr[i].get_series_code() == Series_Code){
            
            return array_ptr[i].data_mean();
            
            
        }
        else{
            continue;
        }
    }

    return 0;
}

// prints the size and capacity of the data array of the specified timeseries
bool TSContainer::TS(std::string Series_Code) {

    for(int i = 0; i < size_; i = i + 1) {
        if (array_ptr[i].get_series_code() == Series_Code){
            std::cout << "size is " << array_ptr[i].get_array_size() << " capacity is " << array_ptr[i].get_array_cap() << std::endl;
            return true;
            
        }
        else{
            continue;
        }
    }
    // fails if specified series not in the collection
    return false;
}