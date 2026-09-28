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

        void read_data(std::string data_source);

        void print();

        int get_valid_data_count();

        void add(std::string Y, std::string D);
        void update(std::string Y, std::string D);
        double data_mean();
        bool is_monotonic();
        bool best_fit(double &m, double &b);


};