#pragma once
#include "tscontainer.hpp"
#include "DataTree.hpp"



class AllTimeSeries {

    TSContainer country_series_array[512];
    DataTree BST;
    int size = 0;

    
    public:
        // read only access to TSContainer object
        const TSContainer& index_array(int index) const;
        TSContainer& index_array(int index);
        int get_size() const;
        void load();
        void list(std::string country_name);
        bool country_min(std::string country_code);
        void range(std::string series_code);
        void build_tree(std::string series_code);
        void find_tree(double mean, std::string operation, std::vector<std::string> &country_list);
        void delete_tree(std::string country_name);
        void limits_tree(std::string condition);
        void trace_tree(std::string country_name);

        // P4 - additional functions

        int compute_W(std::string country_code);
        int primary_hash_function(int W);
        int secondary_hash_function(int primary_hash_code);
        int overall_hash_function(int secondary_hash_result, int i);
        std::pair<int, int> search_hash_table(std::string country_code);
        bool remove_from_hashtable(std::string country_code);

        int find_insert_location(std::string country_code);
        void insert_TSContainer(std::string country_code, int index);
        bool insert_into_hashtable(std::string country_code);

        bool compute_order(std::string country_code1, std::string country_code2);
        void merge(TSContainer base_array[], int start_index, int middle_index ,int end_index);
        void merge_sort(TSContainer base_array[], int start_index, int end_index);

        void merge(TSContainer* base_array[], int start_index, int middle_index ,int end_index);
        void merge_sort(TSContainer* base_array[], int start_index, int end_index);

        void clean_hashtable();
        
    };