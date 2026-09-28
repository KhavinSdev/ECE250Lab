#include "dataP1.hpp"
#include <filesystem>


// constructor and destructor for Array class
Array::Array() {
    array_ptr = new double[2];
    size_ = 0;
    cap_ = 2;
}

Array::~Array() {
    delete[] array_ptr;
    array_ptr = nullptr;
}

Array::Array(const Array& other) : size_(other.size_), cap_(other.cap_) {
        array_ptr = new double[cap_];
        std::copy(other.array_ptr, other.array_ptr + size_, array_ptr);
    }

// Copy assignment
Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] array_ptr;
        size_ = other.size_;
        cap_ = other.cap_;
        array_ptr = new double[cap_];
        std::copy(other.array_ptr, other.array_ptr + size_, array_ptr);
    }
    return *this;
}

// resets array parameters to default
void Array::reset_array() {
    

    delete[] array_ptr;
    array_ptr = nullptr;
    array_ptr = new double[2];
    size_ = 0;
    cap_ = 2;
}

// returns value stored at index
double Array::index_array(int index) {

    if (index < 0 || index >= size_) {
        
        throw std::out_of_range("index out of range");
    }

    return array_ptr[index];
}

// increases capacity of array
void Array::extend_array() {
    
    
    // verifies current size = capacity
    if(size_ != cap_) {
        return;
    }
    
    // allocates new array and copies elements from previous array
    double* new_array = new double[cap_ * 2];
    
    for(int i = 0; i < size_; i = i + 1){
        new_array[i] = array_ptr[i];
    }

    // de-allocates previous array and re-assigns address for pointer
    delete[] array_ptr;
    array_ptr = nullptr;
    array_ptr = new_array;
    cap_ = cap_ * 2;
 

    
    
}

// decreases capacity of array
void Array::shrink_array() {

    // verifies size > 2 and size = 1/4 capacity
    if(cap_ == 2 || size_ != cap_/4) {
        return;
    }

    // allocates new array and copies elements from previous array
    double* new_array = new double[cap_ / 2];
    
    for(int i = 0; i < size_; i = i + 1){
        new_array[i] = array_ptr[i];
    }

    std::cout << "size of array: " << size_ << " capacity of array: " << cap_ << std::endl;
    // de-allocates previous array and re-assigns address for pointer

    delete[] array_ptr;
    array_ptr = nullptr;
    array_ptr = new_array;
    cap_ = cap_ / 2;

}


void Array::append_array(std::string val) {


    array_ptr[size_] = std::stod(val);
    size_ = size_ + 1;

   
    extend_array();
  
    

}

void Array::prepend_array(std::string val) {


    for(int i = size_ - 1; i >= 0; i = i - 1){
        array_ptr[i + 1] = array_ptr[i];
    }
    array_ptr[0] = std::stod(val);
    
    size_ = size_ + 1;

    extend_array();
    
    

}

// updates value stored at an index
void Array::update_array(int index, std::string val) {

    if((index < cap_) && (index >= 0)) {
        array_ptr[index] = std::stod(val);
    }
    else {
        throw std::out_of_range("index out of range");
    }
    
}

// inserts a new value at index position
void Array::insert(std::string field, int position) {

    // shifts elements to the right from the desired index by one to make room for inserted value:
    for(int i = size_ - 1; i >= position; i = i - 1){
        array_ptr[i + 1] = array_ptr[i];
    }
    array_ptr[position] = std::stod(field);

    size_ = size_ + 1;

    extend_array();
    
    

}

// physically deletes element stored at an index position
void Array::delete_item(int position) {
    if (position < 0 || position >= size_) {
        
        throw std::out_of_range("Index out of range");
    }

    //P2 - checks for edge case where only one item is present in array
    if(size_ > 1) {
        // shifts elements after index position to the left by one to overwrite the item to be deleted
        for (int i = position; i < size_ - 1; ++i) {
            array_ptr[i] = array_ptr[i + 1];
        }
    }
    

    size_ = size_ - 1;

    shrink_array();
}

// getter functions for useful properties
int Array::get_size() {return size_;}

int Array::get_cap() {return cap_;}

// binary search used to search years array to get index position of a specific year in the timeseries - used by add and update functions
// based on value of boolean argument get_ifmissing - returns index position of where item should be even if item not found (or) returns -1 if item not found  
int Array::binary_search(std::string value, bool get_ifmissing) {
    
    // Handle empty array case - not expected
    if (size_ == 0) return get_ifmissing ? 0 : -1;

    // Convert the string input to double once
    double value_double = std::stod(value);

    int min_index = 0;           // Start of search range
    int max_index = size_ - 1;   // End of search range

    // Standard binary search loop
    while (min_index <= max_index) {
        int half_point = (min_index + max_index) / 2;

        // Check if the middle element is the value we want
        if (array_ptr[half_point] == value_double) {
            return half_point;  // Value found
        } 
        // If value is smaller, search lower half
        else if (value_double < array_ptr[half_point]) {
            max_index = half_point - 1;
        } 
        // If value is larger, search upper half
        else {
            min_index = half_point + 1;
        }
    }

    // Value not found in array
    if (get_ifmissing) {
        // If get_ifmissing is true, return the correct insertion index
        // min_index points to where the value should be inserted
        return min_index;
    } else {
        // Otherwise, return -1 to indicate not found
        return -1;
    }
}


    





// Timeseries class function definitions:


TimeSeries::TimeSeries() {

}

// Copy constructor
TimeSeries::TimeSeries(const TimeSeries& other) {
        series_name_ = other.series_name_;
        series_code_ = other.series_code_;
        data_src_ = other.data_src_;

        valid_data_count_ = other.valid_data_count_;
        earliest_year_ = other.earliest_year_;
        latest_year_ = other.latest_year_;


        years_array = other.years_array;
        data_array = other.data_array;

}



// resets timeseries arrays and parameters to default values
void TimeSeries::reset_timeseries() {
    years_array.reset_array();
    data_array.reset_array();
    valid_data_count_ = 0;
    earliest_year_ = 0;
    latest_year_ = 0;
}

// loads data from a csv source into the timeseries, also calls reset functions even if datasource path string is empty
void TimeSeries::read_data(std::string data_source, bool direct_line) {

    reset_timeseries();

    int yr_read = 1960;
    std::string data_string{data_source};
    std::string data_item;


    if(!direct_line) {
        if(data_source == "") {
            return;
        }

        std::ifstream file{};

        file.open(data_source);

        std::getline(file, data_string, '\n');
        
        // verifies that the file was opened
        if (!file.is_open()) {
            return;
        }
    }
    
    std::stringstream string_stream(data_string);
    // assumes that data source contains data from 1960 to 2023
    earliest_year_ = 1960;
    latest_year_ = 2023;


    // loads the comma seperated data into respective member variables
    int index{0};
    while(std::getline(string_stream, data_item, ',')){

        if(index == 2) {series_name_ = data_item;}
        if(index == 3) {series_code_ = data_item;}

        if(index < 4) {
            index = index + 1;
            continue;
        }

        // increments valid_data_count_ when data item read for a year is >= 0
        if(std::stod(data_item) >= 0) {
            valid_data_count_ = valid_data_count_ + 1;
        }

        // adds data to the array s - in this case we just append to the end
        years_array.append_array(std::to_string(yr_read));
        data_array.append_array(data_item);

        yr_read = yr_read + 1;

    }
    

}

// P3 modification - overloaded load to use stringstream object passed by reference
void TimeSeries::read_data(std::stringstream& string_stream) {

    int yr_read = 1960;
    std::string data_item;

    // assumes that data source contains data from 1960 to 2023
    earliest_year_ = 1960;
    latest_year_ = 2023;


    // loads the comma seperated data into respective member variables
    int index{0};
    while(std::getline(string_stream, data_item, ',')){

        if(index == 0) {series_name_ = data_item;}
        if(index == 1) {series_code_ = data_item;}

        if(index < 2) {
            index = index + 1;
            continue;
        }

        // increments valid_data_count_ when data item read for a year is >= 0
        if(std::stod(data_item) >= 0) {
            valid_data_count_ = valid_data_count_ + 1;
        }

        // adds data to the array s - in this case we just append to the end
        years_array.append_array(std::to_string(yr_read));
        data_array.append_array(data_item);

        yr_read = yr_read + 1;

    }
}

// prints the valid (year,data) pairs of the timeseries:
bool TimeSeries::print() {

    
    // fails if no valid data
    if(valid_data_count_ < 1) { 

        return false;
    }
    
    std::stringstream ss;

    int size = years_array.get_size();
    
    // iterates to construct data pairs
    for(int i = 0; i < size; i = i + 1) {

        if(data_array.index_array(i) >= 0) {
            ss << "(" << years_array.index_array(i) << "," << data_array.index_array(i) << ") ";
        }

        
    }

    std::string output = ss.str();


    std::cout << output << std::endl;

    return true;

}

// returns value of valid_data_count_ private member variable
int TimeSeries::get_valid_data_count() {
    return valid_data_count_;
}



// adds new (year,data) pairs to the timeseries
bool TimeSeries::add(std::string Y, std::string D) {


    // case when no data is present
    if(data_array.get_size() == 0) {

        years_array.prepend_array(Y);
        data_array.prepend_array(D);

        earliest_year_ = std::stoi(Y);

        // updates the valid data count
        if(std::stod(D) >= 0) {
            valid_data_count_ = valid_data_count_ + 1;
        }

        return true;
    }

    // case where year specified is the latest year in the current series
    if(std::stoi(Y) == latest_year_) {
        if(data_array.index_array(years_array.get_size() - 1) >= 0) {
            
            return false;
        }
        else{
            
            data_array.update_array(data_array.get_size() - 1, D);

            // P2- updates the valid data count
            if(std::stod(D) >= 0) {
                valid_data_count_ = valid_data_count_ + 1;
            }

            return true;

        }
    }

    // cases where year specified is outside of current range
    else if(std::stoi(Y) > latest_year_) {

        years_array.append_array(Y);
        data_array.append_array(D);

        // updates the valid data count
        if(std::stod(D) >= 0) {
            valid_data_count_ = valid_data_count_ + 1;
        }

        latest_year_ = std::stoi(Y);

       
        return true;
    }

    else if(std::stoi(Y) < earliest_year_) {

        years_array.prepend_array(Y);
        data_array.prepend_array(D);

        // updates the valid data count
        if(std::stod(D) >= 0) {
            valid_data_count_ = valid_data_count_ + 1;
        }

        earliest_year_ = std::stoi(Y);

       
        return true;   
    }
    
    // cases where year is within the range
    else {

        // (value, return appropriate index even if val is not present)

        int position = years_array.binary_search(Y, true);

        // if the year is present in the series
        if(years_array.index_array(position) == std::stod(Y)) {

            // fails if valid data exists for that year
            if(data_array.index_array(position) >= 0) {
               
                return false;
            }
            // updates the value for that year (this is considering that invalid data is treated as not contributing to the series)
            else {
                data_array.update_array(position, D);
                
                // P2- updates the valid data count
                if(std::stod(D) >= 0) {
                    valid_data_count_ = valid_data_count_ + 1;
                }
                
                return true;

            }
            
        }  
        // inserts the (year,data) pair into the series 
        else{
           

            years_array.insert(Y, position);
            data_array.insert(D, position);

            // updates the valid data count
            if(std::stod(D) >= 0) {
                valid_data_count_ = valid_data_count_ + 1;
            } 
            
            return true;
 
        }

    }

}

// updates the data stored for a year in the timeseries
bool TimeSeries::update(std::string Y, std::string D) {

    // we want the position only if the year exists in the series
    // binary_search(value, returns -1 if val is not present)
    int position = years_array.binary_search(Y, false);

    

    // fails if year not currently in the series
    if(position == -1) {

        return false;
    }


    // fails if data stored for the year is invalid
    if(data_array.index_array(position) < 0) {

        return false;
    }
    
    

    // if D is negative, delete the pair from the series
    if(std::stod(D) < 0){
        data_array.delete_item(position);
        years_array.delete_item(position);

        if(years_array.get_size() > 0) {
            // updates latest year to the formerly second last year in the timeseries
            latest_year_ = years_array.index_array(years_array.get_size() - 1);
        }
       

        valid_data_count_ = valid_data_count_ - 1;
    }

    // updates the data present for the year Y in timeseries :
    else{
        data_array.update_array(position, D);
    }
    

    return true;
}
 

// 
double TimeSeries::data_mean() {
    double sum = 0;

    int data_array_size = data_array.get_size();

    double count = 0;
    int count_missing = 0;

    for(int i = 0; i < data_array_size; i = i + 1) {

        
        if(data_array.index_array(i) == -1) {
            count_missing = count_missing + 1;
            continue;
        }
        else {
            sum = sum + data_array.index_array(i);
            count = count + 1;
        }
        
    }

    if(count_missing == data_array_size) {
        return 0;
    }
    else {
        return sum / count;
    }

    
}

// determines if the series is monotonic, returns true if it is
bool TimeSeries::is_monotonic() {

    if(valid_data_count_ == 0) {return false;}
    if(valid_data_count_ == 1) {return true;} // special case considered monotonic as per requirement

    int decreasing = 0;
    int increasing = -1; // since the first value the loop encounter will be > 0
    double previous_value{0};

    // checks valid data values and updates variables above based on conditions below
    for(int i = 0; i < data_array.get_size(); i = i + 1) {

        double value = data_array.index_array(i);

        if((increasing != 0) && (decreasing != 0)) {
            return false;
        }

        if(value < 0) {
            continue;
        }

        if(value > previous_value) {
            increasing = increasing + 1;
        }
        else if(value < previous_value) {
            decreasing = decreasing + 1;
        }

        previous_value = value;
    }

    // if the series show both increasing and decreasing segments, return false
    if((increasing != 0) && (decreasing != 0)) {
            return false;
    }

    return true;
}

bool TimeSeries::best_fit(double &m, double &b) {

    

    if (valid_data_count_ < 2) {
        m = 0;
        b = 0;
        return false;
    }

    double sum_xy{0};
    double sum_x{0};
    double sum_y{0};
    double sum_x2{0};
    int N{0};

    // iterates over valid (year,data) pairs and calculates parameters used in slope-intercept formulae
    for(int i = 0; i < data_array.get_size(); i = i + 1) {

        double xi = years_array.index_array(i);
        double yi = data_array.index_array(i);

        if(yi < 0) {
            continue;
        }

        sum_xy = sum_xy + (xi * yi);
        sum_x = sum_x + xi;
        sum_y = sum_y + yi;
        sum_x2 = sum_x2 + (xi * xi);
        N = N + 1;
    }

    
    // calculates slope and intercept
    m = ((N * sum_xy) - (sum_x * sum_y)) / ((N * sum_x2) - (sum_x * sum_x));
    b = (sum_y - (m * sum_x)) / N;

    return true;

}
