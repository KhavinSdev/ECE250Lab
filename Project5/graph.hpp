#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include "tscontainer.hpp"
#include "AllTimeSeries.hpp"



class Graph {

    class Edge {
        public:

        // Defines an alias for readability, this tuple represents a relationship between two countries
        using RelationTuple = std::tuple<std::string, double, std::string>;
        std::vector<RelationTuple> Relationships;
        bool ingraph = false;

    };


    private:

    AllTimeSeries* AllTimeSeriesHashTable;

    // Adjacency map for graph implementation, uses a pointer to a TSContainer as a Node representing country
    std::map<
        std::string, 
        std::map<
            std::string, 
            Edge
        >
    > adjacency_map;

    std::map<std::string, std::string> CountryCodeToName_map;

    bool first_load = true;

    public:

    // provides pointer to the hashtable object for graph
    Graph(AllTimeSeries &AllTimeSeriesObject) {
        AllTimeSeriesHashTable = &AllTimeSeriesObject;
    }


    void initialize_graph();

    bool update_edges(std::string Series_Code, double Threshold, std::string Relation);


    void adjacent(std::string Country_Code);

    bool recursive_dfs(std::string country1, std::string country2, std::unordered_set<std::string> &visited_countries);

    void path(std::string Country_Code1, std::string Country_Code2);


    void relationships(std::string Country_Code1, std::string Country_Code2);

};
