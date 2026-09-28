#pragma once
#include "PreviousProject/tscontainer.hpp"
#include "DataTree.hpp"



class AllTimeSeries {

    TSContainer country_series_array[512];
    DataTree BST;
    int size = 0;

    
    public:
        // read only access to TSContainer object
        const TSContainer& index_array(int index) const;
        int get_size() const;
        void load();
        void list(std::string country_name);
        bool country_min(std::string country_code);
        void range(std::string series_code);
        void build_tree(std::string series_code);
        void find_tree(double mean, std::string operation);
        void delete_tree(std::string country_name);
        void limits_tree(std::string condition);
        void trace_tree(std::string country_name);
    };