#pragma once
#include <iostream>
#include <fstream>
#include <sstream>



class Array {
    private:

        double *array_ptr{};
        int size_;
        int cap_;

    public:

        Array();
        ~Array();

        Array(const Array& other);
        // Assignment
        Array& operator=(const Array& other);
       

        void reset_array();
        double index_array(int index);

        void extend_array();
        void shrink_array();

        void append_array(std::string val);

        void prepend_array(std::string val);

        void update_array(int index, std::string val);

        void insert(std::string field, int position);

        void delete_item(int position);

        int get_size();

        int get_cap();

        int binary_search(std::string value, bool get_ifmissing);


};

class TimeSeries{

    private:

        std::string series_name_;
        std::string series_code_;
        std::string data_src_;

        int valid_data_count_{0};
        
        Array years_array;
        Array data_array;

        int earliest_year_{0};
        int latest_year_{0};
        
        void reset_timeseries();
        

    public:

        TimeSeries();
            // Copy constructor declaration
        TimeSeries(const TimeSeries& other);

        void read_data(std::string data_source, bool direct_line);
        void read_data(std::stringstream& string_stream);

        bool print();

        int get_valid_data_count();
        std::string get_series_name() {return series_name_;}
        std::string get_series_code() {return series_code_;}
        int get_array_size() {return years_array.get_size();}
        int get_array_cap() {return years_array.get_cap();}

        bool add(std::string Y, std::string D);
        bool update(std::string Y, std::string D);
        double data_mean();
        bool is_monotonic();
        bool best_fit(double &m, double &b);


};