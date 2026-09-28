#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "AllTimeSeries.hpp"
#include "PreviousProject/tscontainer.hpp"



// returns a const reference to the TSContainer object stored at the given index in the country series (array)
const TSContainer& AllTimeSeries::index_array(int index) const{
    return country_series_array[index];
}


// getter function for size of the array
int AllTimeSeries::get_size() const{
    return size;
}


// handles loading data from the csv
void AllTimeSeries::load() {


std::fstream file;
std::string data_line;
std::string data_item;

int current_index = -1;
size = 0;
std::string current_country_name{""};
std::string previous_country_name{""};
std::string current_country_code{""};


// opens dataset file

file.open("lab2_multidata.csv");

if (!file.is_open()) {
    std::cerr << "Failed to open file: lab2_multidata.csv" << "\n";
    std::cerr << std::filesystem::current_path() << std::endl;
    return;
}

// iterates line by line over the data file
while(std::getline(file, data_line, '\n')) {
    std::stringstream string_stream(data_line);

    // stores the name of the country specified for the specific data_line
    std::getline(string_stream, current_country_name, ',');

    // compares current country name with the previous line's country name to decide whether to instantiate a new TSContainer object
    if(previous_country_name != current_country_name) {

        // current index is the index of the country_series_array at which the currently being processed TSContainer object is stored, increment when new TSContainer is created
        current_index = current_index + 1;

        // we increment size whenever a new TSContainer object is created
        size = size + 1;

        // sets metadata for the new TSContainer and provides the line through a reference to the stringstream to load the data in the TSContainer for that specific country
        std::getline(string_stream, current_country_code, ',');
        country_series_array[current_index] = TSContainer();
        country_series_array[current_index].set_country_name(current_country_name);
        country_series_array[current_index].set_country_code(current_country_code);
        country_series_array[current_index].load_country_series(string_stream);

        // updates previous_country_name
        previous_country_name = current_country_name;
        
        
    }

    // if the current line's country matches, we update the existing country's data
    else {
        string_stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
        country_series_array[current_index].load_country_series(string_stream);
    } 
}


// closes file
file.close();
}


// calls list from TSContainer to list the series for a give country_name
void AllTimeSeries::list(std::string country_name) {

    if(country_name == "") {
        return;
    }

    for (int i = 0; i < size; i = i + 1) {

        if(country_series_array[i].get_country_name() == country_name) {
            country_series_array[i].list();
            return;
        }
    }

}

// prints the series code of the series with the smallest mean for the given country_code
bool AllTimeSeries::country_min(std::string country_code) {

    for (int i = 0; i < size; i = i + 1) {

        if(country_series_array[i].get_country_code() == country_code) {
            
            
            std::cout << country_series_array[i].smallest() << std::endl;
            return true;
        }
    }

    return false;
}


// outputs the range for the given series (min max)
void AllTimeSeries::range(std::string series_code) {

    double min = std::numeric_limits<double>::max();
    double max = 0;
    double mean_current = 0;
    int country_count = 0;

    // iterates over the country_series_array and gets the mean of the series associated with the given series_code for each country
    for (int i = 0; i < size; i = i + 1) {

        mean_current = country_series_array[i].mean_of_series(series_code);

        
        // updates min accordingly
        if(mean_current <= min && mean_current != 0) {
            min = mean_current;
            
        }

        // updates max accordingly
        if(mean_current >= max) {
            max = mean_current;
            
        }

    
        country_count = country_count + 1;
        
        
        
        
    }

    // if only one country with valid mean data, if its mean is 0 set min and max as 0 else min = max
    if(country_count == 1) {

        if(mean_current == 0) {
            min = 0;
            max = 0;
        }
        else{
            min = max;
        }
        
    }

    std::cout << min << " " << max << std::endl;

}

// builds the BST by first reseting to clear existing data, defining the series_code it stores, getting and sorting an array of all the valid means for the given series_code and building the tree structure itself
void AllTimeSeries::build_tree(std::string series_code) {
    BST.reset();
    BST.set_series_code(series_code);
    BST.populate_valid_means_array(*this);
    BST.sort_valid_means_array();
    BST.build();
}

// These functions call the corresponding functionalities in the DataTree class

void AllTimeSeries::find_tree(double mean, std::string operation) {
    BST.find(mean, operation);
}

void AllTimeSeries::delete_tree(std::string country_name) {
    BST.delete_country(country_name);
}

void AllTimeSeries::limits_tree(std::string condition) {
    BST.limits(condition);
}

void AllTimeSeries::trace_tree(std::string country_name) {
    BST.trace(country_name);
}
