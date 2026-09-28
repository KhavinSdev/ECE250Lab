 #include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include "tscontainer.hpp"
#include "AllTimeSeries.hpp"
#include "graph.hpp"
 
 
void Graph::initialize_graph() {

    

    adjacency_map.clear();
    CountryCodeToName_map.clear();

    int size = 512;

    for(int i = 0; i < 512; i = i + 1) {
        
        if(AllTimeSeriesHashTable->index_array(i).is_empty) {
            continue;
        }
        else {
            // adds countries to the adjacency map for the graph
            adjacency_map.insert({(AllTimeSeriesHashTable->index_array(i)).get_country_name(), {}});

            // associates country code with country name
            CountryCodeToName_map.insert({(AllTimeSeriesHashTable->index_array(i)).get_country_code(), ((AllTimeSeriesHashTable->index_array(i)).get_country_name())});
        }
    }

    std::cout << "success" << std::endl;
}

bool Graph::update_edges(std::string Series_Code, double Threshold, std::string Relation) {
    AllTimeSeriesHashTable->build_tree(Series_Code);
    std::vector<std::string> country_list;

    AllTimeSeriesHashTable->find_tree(Threshold, Relation, country_list);

    // if no countries satisfy the relation, failure
    if(country_list.empty()) {
        return false;
    }

    int update_count = 0;
    bool relation_already_present = false;

    // iterates over countries that were found to satisfy the relationship and attempts to add the relation to the edges

    for(int i = 0; i < country_list.size(); i = i + 1) {

        for(int j = 0; j < country_list.size(); j = j + 1) {

            relation_already_present = false;
            
            // skips itself
            if(j == i) {
                continue;
            }

            // attempts to update the edge if the edge is already present
            if(!adjacency_map[country_list[i]].empty()) {

                for(int k = 0; k < adjacency_map[country_list[i]][country_list[j]].Relationships.size(); k = k + 1) {

                    if(std::get<0>(adjacency_map[country_list[i]][country_list[j]].Relationships[k]) == Series_Code &&
                    std::get<1>(adjacency_map[country_list[i]][country_list[j]].Relationships[k]) == Threshold &&
                    std::get<2>(adjacency_map[country_list[i]][country_list[j]].Relationships[k]) == Relation) {

                        relation_already_present = true;
                        break;
                    }
                }

                // adds relation to edge
                if(!relation_already_present) {
                    adjacency_map[country_list[i]][country_list[j]].Relationships.push_back(std::make_tuple(Series_Code, Threshold, Relation));
                    update_count = update_count + 1;
                }

            }

            // adds new key(country) associated with a new edge to the adjacency map's value(another map for edge between country nodes)

            else {
                adjacency_map[country_list[i]].insert({country_list[j], {}});
                adjacency_map[country_list[i]][country_list[j]].Relationships.push_back(std::make_tuple(Series_Code, Threshold, Relation));
                
                adjacency_map[country_list[i]][country_list[j]].ingraph = true;
                update_count = update_count + 1;
            }
            

        }
    }

    // if no changes were made, failure
    if(update_count == 0) {
        return false;
    }
    else {
        return true;
    }
    
}


void Graph::adjacent(std::string Country_Code) {

    if(CountryCodeToName_map.find(Country_Code) == CountryCodeToName_map.end()) {
        std::cout << "failure" << std::endl;
        return;
    }

    std::string Country_Name = CountryCodeToName_map[Country_Code];

    // // fails if country not in graph
    // if(!(adjacency_map.find(Country_Name) != adjacency_map.end())) {
    //     std::cout << "failure" << std::endl;
    //     return;
    // }

    if(adjacency_map[Country_Name].empty()) {
        std::cout << "none" << std::endl;
        return;
    }

    for (auto it = adjacency_map[Country_Name].begin(); it != adjacency_map[Country_Name].end(); ++it) {

        // skips the empty key since we don't want to print it
        if(it->first == "empty") {
            continue;
        }

        std::cout << it->first << " ";   // prints the adjacent country name

    }
    std::cout << std::endl;

    return;

}

bool Graph::recursive_dfs(std::string country1, std::string country2, std::unordered_set<std::string> &visited_countries) {
    
    visited_countries.insert(country1);

    for (auto it = adjacency_map[country1].begin(); it != adjacency_map[country1].end(); ++it) {
        if(it->first == country2) {
            return true;
        }

        // skips nodes we have already seen
        if(visited_countries.find(it->first) != visited_countries.end()) {
            continue;
        }

        if(recursive_dfs(it->first, country2, visited_countries)) {
            return true;
        }
    }

    return false;

}

void Graph::path(std::string Country_Code1, std::string Country_Code2) {

    std::string country1 = CountryCodeToName_map[Country_Code1];
    std::string country2 = CountryCodeToName_map[Country_Code2];

    // first check if the countries are directly connected
    if(adjacency_map[country1].find(country2) != adjacency_map[country1].end()) {
        std::cout << "true" << std::endl;

        return;
    }

    // will use DFS to find if a path exists

    std::unordered_set<std::string> visited_countries;
    if(recursive_dfs(country1, country2, visited_countries)) {
        std::cout << "true" << std::endl;

        return;
    }

    std::cout << "false" << std::endl;

}


void Graph::relationships(std::string Country_Code1, std::string Country_Code2) {

    std::string country1 = CountryCodeToName_map[Country_Code1];
    std::string country2 = CountryCodeToName_map[Country_Code2];

    std::string Series_Code;
    double Threshold;
    std::string Relation;

    // checks whether any edge is present between country 1 and country2, outputs none if no edge is present
    if(adjacency_map[country1].find(country2) == adjacency_map[country1].end()) {
        std::cout << "none" << std::endl;

        return;
    }

    // prints the relationship tuples for the edge
    for(int i = 0; i < adjacency_map[country1][country2].Relationships.size(); i = i + 1) {

        Series_Code = std::get<0>(adjacency_map[country1][country2].Relationships[i]);
        Threshold = std::get<1>(adjacency_map[country1][country2].Relationships[i]);
        Relation = std::get<2>(adjacency_map[country1][country2].Relationships[i]);

        // yes aware of unrequired space character when can be appended to bracket
        std::cout << "(" << Series_Code << " " << Threshold << " " << Relation << ")" << " ";
    }
    std::cout << std::endl;

}

