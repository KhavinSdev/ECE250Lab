#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cmath>

#include "AllTimeSeries.hpp"
#include "tscontainer.hpp"



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

        if(current_country_name == "Canada") {
            int test = 0;
        }

        // we increment size whenever a new TSContainer object is created, this only serves as a valid data count now
        size = size + 1;

        // sets metadata for the new TSContainer and provides the line through a reference to the stringstream to load the data in the TSContainer for that specific country
        std::getline(string_stream, current_country_code, ',');

        // current index is the index of the country_series_array at which the currently being processed TSContainer object is stored, update when new TSContainer is created

        int i = 0;
        int W = compute_W(current_country_code);
        current_index = overall_hash_function(W, i);

        while(!(country_series_array[current_index].is_empty)) {
            i = i + 1;
            current_index = overall_hash_function(W, i);
        }


        country_series_array[current_index] = TSContainer();
        country_series_array[current_index].set_country_name(current_country_name);
        country_series_array[current_index].set_country_code(current_country_code);
        country_series_array[current_index].load_country_series(string_stream);
        country_series_array[current_index].is_empty = false;

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

    for (int i = 0; i < 512; i = i + 1) {

        if(country_series_array[i].is_empty) {
            continue;
        }

        if(country_series_array[i].get_country_name() == country_name) {
            country_series_array[i].list();
            return;
        }
    }

}

// prints the series code of the series with the smallest mean for the given country_code
bool AllTimeSeries::country_min(std::string country_code) {

    for (int i = 0; i < 512; i = i + 1) {

        if(country_series_array[i].is_empty) {
            continue;
        }

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
    for (int i = 0; i < 512; i = i + 1) {

        if(country_series_array[i].is_empty) {
            continue;
        }

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
    if(BST.delete_country(country_name)) {
        std::cout << "success" << std::endl;
    }
    else {
        std::cout << "failure" << std::endl;
    }
}

void AllTimeSeries::limits_tree(std::string condition) {
    BST.limits(condition);
}

void AllTimeSeries::trace_tree(std::string country_name) {
    BST.trace(country_name);
}

int AllTimeSeries::compute_W(std::string country_code) {

    int W = 0;

    for(int i = 0; i < country_code.length(); i = i + 1) {
        W = W + ( ((int)country_code[i] - 65) * (std::pow(26, country_code.length() - (1 + i))) );
    }

    return W;

}

int AllTimeSeries::primary_hash_function(int W) {
    return W % 512;
}

int AllTimeSeries::secondary_hash_function(int W) {

    // Since W >= 0, integer division behaves like floor()
    int hash_code = (W/512) % 512;

    if(hash_code % 2 != 0) {
        return hash_code;
    }
    else {
        return hash_code + 1;
    }
}

int AllTimeSeries::overall_hash_function(int W, int i) {
    
    return (primary_hash_function(W) + (i * secondary_hash_function(W))) % 512;
}

std::pair<int, int> AllTimeSeries::search_hash_table(std::string country_code) {

    int W = compute_W(country_code);
    int starting_index = overall_hash_function(W, 0);
    int current_index = starting_index;
    int i = 1;

    if(country_series_array[current_index].get_country_code() == country_code) {
        return {starting_index, i};
    }

    while(!(country_series_array[current_index].is_empty) || country_series_array[current_index].previously_occupied) {

        current_index = overall_hash_function(W, i);
        i = i + 1;

        // we looped around to the starting index which was already searched so element not found
        if(current_index == starting_index) {
            return {-1, 0};
        }

        if(country_series_array[current_index].get_country_code() == country_code) {
            return {current_index, i};
        }

        
        
    }

    // we can also get the i value for insert
    return {-1, i};
}

bool AllTimeSeries::remove_from_hashtable(std::string country_code) {

    auto [index, searches] = search_hash_table(country_code);

    if(index != -1) {

        std::string country_name = country_series_array[index].get_country_name();
        country_series_array[index].reset();
        country_series_array[index].previously_occupied = true;

        // reduce valid data count
        size = size - 1;

        if(BST.is_treebuilt()) {
            BST.delete_country(country_name);
        }

        return true;

    }


    return false;
   
}

// helper function for insert_into_hashtable
int AllTimeSeries::find_insert_location(std::string country_code) {

    int W = compute_W(country_code);
    int starting_index = overall_hash_function(W, 0);
    int current_index = starting_index;
    int i = 0;

    while(!(country_series_array[current_index].is_empty)) {

        i = i + 1;
        current_index = overall_hash_function(W, i);
        

        // we looped around to the starting index which was already searched so element not found
        if(current_index == starting_index) {
            return -1;
        }       
        
    }

    // we can also get the i value for insert
    return current_index;
}

void AllTimeSeries::insert_TSContainer(std::string country_code, int index) {


std::fstream file;
std::string data_line;
std::string data_item;

std::string current_country_code{""};
std::string previous_country_code{""};
std::string current_country_name{""};
std::string previous_country_name{""};
bool country_processed = false;



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

        if(country_processed) {
            return;
        }

        // sets metadata for the new TSContainer and provides the line through a reference to the stringstream to load the data in the TSContainer for that specific country
        std::getline(string_stream, current_country_code, ',');

        if(current_country_code != country_code) {
            continue;
        }



        country_series_array[index] = TSContainer();
        country_series_array[index].set_country_name(current_country_name);
        country_series_array[index].set_country_code(current_country_code);
        country_series_array[index].load_country_series(string_stream);
        country_series_array[index].is_empty = false;
        country_series_array[index].previously_occupied = false;

        // updates previous_country_name
        previous_country_name = current_country_name;
        
        country_processed = true;
        
    }

    // if the current line's country matches, we update the existing country's data
    else {

        if(current_country_code != country_code) {
            continue;
        }

        string_stream.ignore(std::numeric_limits<std::streamsize>::max(), ',');
        country_series_array[index].load_country_series(string_stream);
    } 
}

size = size + 1;

// closes file
file.close();
}


bool AllTimeSeries::insert_into_hashtable(std::string country_code) {


    auto [index, searches] = search_hash_table(country_code);

    if(index != -1) {
        return false;
    }

    index = find_insert_location(country_code);

    insert_TSContainer(country_code, index);

    // increase valid data quantifier
    size = size + 1;

    return true;


}

// helper function for computing the order based on base 26 code
bool AllTimeSeries::compute_order(std::string country_code1, std::string country_code2) {

    int item0 = compute_W(country_code1);
    int item1 = compute_W(country_code2);

    if(item0 < item1) {
        return true;
    }
    else {
        return false;
    }
}

// void AllTimeSeries::merge(TSContainer base_array[], int start_index, int middle_index ,int end_index) {

//     int temp_size = (end_index-start_index) + 1;
//     TSContainer temp[temp_size];

//     int i = start_index;
//     int j = middle_index + 1;
//     int temp_index = 0;

//     // copies elements to temp array in ascending order
//     while(i < middle_index + 1 && j < end_index + 1) {

//         if(compute_order(base_array[i].get_country_code(), base_array[j].get_country_code())) {
//             temp[temp_index] = base_array[i];
//             i = i + 1;

//         }
//         else {
//             temp[temp_index] = base_array[j];
//             j = j + 1;
//         }

//         temp_index = temp_index + 1;
//     }

//     if(i < middle_index + 1) {
//         for(i; i < middle_index + 1; i = i + 1) {
//             temp[temp_index] = base_array[i];
//             temp_index = temp_index + 1;
//         }
//     }

//     if(j < end_index + 1) {
//         for(j; j < end_index + 1; j = j + 1) {
//             temp[temp_index] = base_array[j];
//             temp_index = temp_index + 1;
//         }
//     }

//     // copies sorted elements in correct order to the original array
//     temp_index = 0;
//     for (int i = start_index; i < end_index + 1; i = i + 1) {
//         base_array[i] = temp[temp_index];
//         temp_index = temp_index + 1;
//     }

// }

// void AllTimeSeries::merge_sort(TSContainer base_array[], int start_index, int end_index) {

//     if(start_index >= end_index) {
//         return;
//     }

//     int middle_index = (end_index + start_index) / 2;

//     merge_sort(base_array, start_index, middle_index);
//     merge_sort(base_array, middle_index + 1, end_index);

//     merge(base_array, start_index, middle_index, end_index);


// }

void AllTimeSeries::merge(TSContainer* base_array[], int start_index, int middle_index ,int end_index) {

    int temp_size = (end_index-start_index) + 1;
    TSContainer* temp[temp_size];

    int i = start_index;
    int j = middle_index + 1;
    int temp_index = 0;

    // copies elements to temp array in ascending order
    while(i < middle_index + 1 && j < end_index + 1) {

        if(compute_order(base_array[i]->get_country_code(), base_array[j]->get_country_code())) {
            temp[temp_index] = base_array[i];
            i = i + 1;

        }
        else {
            temp[temp_index] = base_array[j];
            j = j + 1;
        }

        temp_index = temp_index + 1;
    }

    if(i < middle_index + 1) {
        for(i; i < middle_index + 1; i = i + 1) {
            temp[temp_index] = base_array[i];
            temp_index = temp_index + 1;
        }
    }

    if(j < end_index + 1) {
        for(j; j < end_index + 1; j = j + 1) {
            temp[temp_index] = base_array[j];
            temp_index = temp_index + 1;
        }
    }

    // copies sorted elements in correct order to the original array
    temp_index = 0;
    for (int i = start_index; i < end_index + 1; i = i + 1) {
        base_array[i] = temp[temp_index];
        temp_index = temp_index + 1;
    }

}

void AllTimeSeries::merge_sort(TSContainer* base_array[], int start_index, int end_index) {

    if(start_index >= end_index) {
        return;
    }

    int middle_index = (end_index + start_index) / 2;

    merge_sort(base_array, start_index, middle_index);
    merge_sort(base_array, middle_index + 1, end_index);

    merge(base_array, start_index, middle_index, end_index);


}


void AllTimeSeries::clean_hashtable() {
    
    // create a new array to contain all valid data:

    TSContainer* sort_array[size];
    TSContainer new_array[512];

    // iterate through hash table and copy valid data to the sort_array:

    int new_index = 0;
    for(int i = 0; i < 512; i = i + 1) {

        if(country_series_array[i].is_empty || country_series_array[i].previously_occupied) {
            country_series_array[i].reset();
            continue;
        }

        sort_array[new_index] = &country_series_array[i];
        new_index = new_index + 1;

    }

    // sort the array:

    merge_sort(sort_array, 0, size - 1);


    // insert elements back into the hashtable
    int destination = 0;
    for(int i = 0; i < size; i = i + 1) {

        destination = find_insert_location(sort_array[i]->get_country_code());
        new_array[destination] = *sort_array[i];
        
    }

    std::copy(new_array, new_array + 512, country_series_array);

    
    
}

// void AllTimeSeries::clean_hashtable() {
    
//     // create a new array to contain all valid data:

//     TSContainer sort_array[size];

//     // iterate through hash table and copy valid data to the sort_array:

//     int new_index = 0;
//     for(int i = 0; i < 512; i = i + 1) {

//         if(country_series_array[i].is_empty || country_series_array[i].previously_occupied) {
//             country_series_array[i].reset();
//             continue;
//         }

//         sort_array[new_index] = country_series_array[i];
//         country_series_array[i].reset();
//         new_index = new_index + 1;


//     }

//     // sort the array:

//     merge_sort(sort_array, 0, size - 1);


//     // insert elements back into the hashtable
//     int destination = 0;
//     for(int i = 0; i < size; i = i + 1) {

//         destination = find_insert_location(sort_array[i].get_country_code());
//         country_series_array[destination] = sort_array[i];
        
//     }


    
// }
