#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>



class AllTimeSeries;


class DataTree {

    private:
    
        // transient object type to make tree building process easier
        class countrymean_pair {
            
            public:

                std::string country_name;
                double mean;

                countrymean_pair(std::string countryName, double Mean) {
                    country_name = countryName;
                    mean = Mean;
                }

                countrymean_pair() {
                }
        };


        class Node {

            public:

                countrymean_pair data[512];
                int size = 0;
                const int cap = 512;

                double local_min = 0;
                double local_max = 0;
                double midpoint = 0;

                Node* left_child = nullptr;
                Node* right_child = nullptr;

                bool is_leaf = false;

                int h_findcountry(std::string country_name);
                void h_delete_data(int index);
                void h_print_countries();

                Node(countrymean_pair* pair_array, int starting_index, int ending_index, int size_of_dataarray) {
                    local_min = pair_array[starting_index].mean;
                    local_max = pair_array[ending_index].mean;
                    midpoint = (local_min + local_max) / 2;
                    
                    
                    for (int i = 0; i < size_of_dataarray; i = i + 1) {
                        data[i] = pair_array[i + starting_index];
                        size = size + 1;

                    
                    }

                    if( std::abs(local_max - local_min) <= 0.001) {
                        is_leaf = true;
                    }

                }

        };

        

        Node* root_node = nullptr;
        double Min{0};
        double Max{0};
        std::string series_code;
        bool built_tree = false;

        countrymean_pair valid_mean_pairs[512];
        int valid_means_size = 0;
        

        Node* build_recursive(Node* current, int start_index, int end_index);
        void recursive_postorder_deallocation(Node* root);

        public:

            ~DataTree();
            void set_series_code(std::string Series_code) { series_code = Series_code;}

            void populate_valid_means_array(const AllTimeSeries& timeseries_allcountries);
            // implemented using insertion sort algorithm
            void sort_valid_means_array();
            void reset();
            void build();
            void traverse_less(Node* current, double mean);

            void traverse_greater(Node* current, double mean);
            void traverse_equal(Node* current, double mean);
            void recursive_find_less(Node* current, double mean);
            void recursive_find_greater(Node* current, double mean);
            void recursive_find_equal(Node* current, double mean);

            void find(double mean, std::string operation);

            void delete_country(std::string country_name);
            //recursive helper overload
            void delete_country(Node* current, std::string country_name, Node** pointer_from_parent);
            void limits(std::string condition);
            void trace(std::string country_name);
};