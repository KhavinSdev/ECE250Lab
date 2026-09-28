#pragma once
#include "dataP1.hpp"



class TSContainer {

    private:

        TimeSeries *array_ptr = nullptr;
        std::string country_name_{""};
        std::string country_code_{""};
        int size_{0};
        int cap_{0};

    public:

        TSContainer();
        ~TSContainer();
        TSContainer& operator=(const TSContainer& other);

        
        std::string get_country_name() const{return country_name_;}
        std::string get_country_code() const{return country_code_;}
        void set_country_name(std::string CountryName) {country_name_ = CountryName; }
        void set_country_code(std::string CountryCode) {country_code_ = CountryCode; }

        void reset();
        void extend_collection();
        void load_country_series(std::string country_name);

        // P3 
        void load_country_series(std::stringstream& string_stream);

        void list();
        bool add(std::string Series_Code, std::string Y, std::string D);
        bool update(std::string Series_Code, std::string Y, std::string D);
        bool print(std::string Series_Code);
        bool delete_timeseries(std::string Series_Code);
        std::string biggest();
        std::string smallest();
        double mean_of_series(std::string Series_Code) const;
        bool TS(std::string Series_Code);

};