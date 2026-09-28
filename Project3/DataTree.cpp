#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "DataTree.hpp"
#include "AllTimeSeries.hpp"




// Node functions

// returns the index of the countrymean_pair from the node's data array with the given country_name, returns -1 if not found
int DataTree::Node::h_findcountry(std::string country_name) {

    int index = 0;
    for(int i = 0; i < size; i = i + 1) {
        if(data[i].country_name == country_name) {
            index = i;
            break;
        }
    }

    if(data[index].country_name == country_name) {
        return index;
    } 
    else {
        return -1;
    }

}

// deletes entry in the data array at the given index
void DataTree::Node::h_delete_data(int index) {

    for(int i = index; i < size - 1; i = i + 1) {
        data[i] = data[i+1];
    }
}

// prints the names of the countries stored by the node
void DataTree::Node::h_print_countries() {
    for(int i = 0; i < size; i = i + 1) {
        std::cout << data[i].country_name << " ";
    }
    std::cout << std::endl;
}


// DataTree definitions


// recursively builds the BST given a root node, starting index and ending index relative to the valid_mean_pairs array
DataTree::Node* DataTree::build_recursive(Node* current, int start_index, int end_index) {

    // base case when sub-tree root is a leaf node of the main tree
    if(current != nullptr && current->is_leaf) {
        return nullptr;
    }

    // creates a new node and initialises its data
    Node* new_node = new Node(valid_mean_pairs, start_index, end_index, (end_index - start_index) + 1);

    // determines where to split the range of indices for building child nodes recursively (relative to the same valid_mean_pairs array)
    int partition_index = start_index;
    for( int i = start_index; i < end_index + 1; i = i + 1) {

        // exits if mean at next index is greater than or equal to the midpoint of the current node
        if(valid_mean_pairs[i].mean >= new_node->midpoint) {
            break;
        }
        else {
            partition_index = i;
        }

    }

    // adds 1 to get partion index to be the starting index of the right child
    partition_index = partition_index + 1;

    // builds the left and right sub trees
    new_node->left_child = build_recursive(new_node, start_index, partition_index - 1);
    new_node->right_child = build_recursive(new_node, partition_index, end_index);

    //returns the new node which becomes the child of the higher tree node or root in case this is the first recursive call
    return new_node;


}

// Helper function used by destructor and reset to free dynamically allocated memory of the tree being reset/removed
void DataTree::recursive_postorder_deallocation(Node* root) {

    if(root == nullptr) {
        return;
    }

    recursive_postorder_deallocation(root->left_child);
    recursive_postorder_deallocation(root->right_child);

    delete root;
    root = nullptr;

}

// destructor for the DataTree object
DataTree::~DataTree() {
    if(root_node == nullptr) {
        return;
    }

    recursive_postorder_deallocation(root_node);
    root_node = nullptr;
    }


// iterates over the const reference to AllTimeSeries argument's array and adds the valid (country_name, mean) pairs to the valid_mean_pairs array of this DataTree
void DataTree::populate_valid_means_array(const AllTimeSeries& timeseries_allcountries) {


    int size = timeseries_allcountries.get_size();
    double current_mean;
    std::string country_name;

    int current_index = 0;

    for (int i = 0; i < size; i = i + 1) {

        current_mean = timeseries_allcountries.index_array(i).mean_of_series(series_code);
        country_name = timeseries_allcountries.index_array(i).get_country_name();

        // only adds valid data to the array
        if(current_mean != 0) {

            valid_mean_pairs[current_index] = countrymean_pair(country_name, current_mean);

            valid_means_size = valid_means_size + 1;
            current_index = current_index + 1;
        }
        
        
        
    }

}


// uses insertion sort algorithm to sort the valid_mean_pairs array in ascending order
void DataTree::sort_valid_means_array() {

    countrymean_pair temp;

    if(valid_means_size == 1) {
        Min = valid_mean_pairs[0].mean;
        Max = Min;
        return;
    }

    for(int i = 1; i < valid_means_size; i = i + 1) {

        for (int j = i - 1; j > -1; j = j - 1) {

            if(valid_mean_pairs[j + 1].mean < valid_mean_pairs[j].mean) {

                temp = valid_mean_pairs[j];
                valid_mean_pairs[j] = valid_mean_pairs[j + 1];
                valid_mean_pairs[j + 1] = temp;

            }
            else {
                break;
            }

        }
    }

    // sets the global Min and Max for the tree
    Min = valid_mean_pairs[0].mean;
    Max = valid_mean_pairs[valid_means_size - 1].mean;

}

// resets the DataTree object and removes the existing BST if already built
void DataTree::reset() {
    recursive_postorder_deallocation(root_node);

    root_node = nullptr;
    Min = 0;
    Max = 0;
    series_code = "";
    built_tree = false;
    valid_means_size = 0;
}

// builds the BST
void DataTree::build() {
    
    
    root_node = build_recursive(root_node, 0, valid_means_size - 1);

    // member variable specifies that a tree has been built - useful in other functions
    built_tree = true;
    
}


// recursive helper function which prints the countries of the leaf nodes where means are less than the argument mean
void DataTree::traverse_less(Node* current, double mean) {

    if(current == nullptr) {
        return;
    }

    if(current->is_leaf) {

        for(int i = 0; i < current->size; i = i + 1) {
            if(current->data[i].mean < mean) {
                std::cout << current->data[i].country_name << " ";
            }
            
        }
        return;
    }

    traverse_less(current->left_child, mean);
    traverse_less(current->right_child, mean);


}

// recursive helper function which prints the countries of the leaf nodes where means are greater than the argument mean
void DataTree::traverse_greater(Node* current, double mean) {

    if(current == nullptr) {
        return;
    }

    if(current->is_leaf) {

        for(int i = 0; i < current->size; i = i + 1) {
            if(current->data[i].mean > mean) {
                std::cout << current->data[i].country_name << " ";
            }
            
        }
        return;
    }

    traverse_greater(current->left_child, mean);
    traverse_greater(current->right_child, mean);


}

// recursive helper function which prints the countries of the leaf nodes where means are equal to the argument mean within tolerance(0.001)
void DataTree::traverse_equal(Node* current, double mean) {

    

    for(int i = 0; i < current->size; i = i + 1) {
        if(std::abs(current->data[i].mean - mean) <= 0.001) {
            std::cout << current->data[i].country_name << " ";
        }
        
    }
    return; 

}


// finds the nodes with means less than the argument mean and calls the relevant traverse helper to print the names of the countries in these nodes
void DataTree::recursive_find_less(Node* current, double mean) {

    if(current->is_leaf) {                  
        traverse_less(current, mean);
        return;
    }

    if( mean == current->midpoint) {
        traverse_less(current->left_child, mean);
        return;
    }
    
    else if( mean > current->midpoint) {
        traverse_less(current->left_child, mean);
        recursive_find_less(current->right_child, mean);
    }
    else {
        recursive_find_less(current->left_child, mean);
    }
}

// finds the nodes with means greater than the argument mean and calls the relevant traverse helper to print the names of the countries in these nodes
void DataTree::recursive_find_greater(Node* current, double mean) {

    if(current->is_leaf) {
        traverse_greater(current, mean);
        return;
    }

    if( mean == current->midpoint) {
        recursive_find_greater(current->right_child, mean);
        
    }
    
    else if( mean < current->midpoint) {
        traverse_greater(current->right_child, mean);
        recursive_find_greater(current->left_child, mean);
    }
    else {
        recursive_find_greater(current->right_child, mean);
    }

}

// finds the nodes with means that could be equal to the argument mean and calls the relevant traverse helper to verify and print the names of the countries in these nodes
void DataTree::recursive_find_equal(Node* current, double mean) {

    if(current == nullptr) {
        return;
    }

    if(current->is_leaf) {
        traverse_equal(current, mean);
        return;
    }

    else if (current->midpoint > mean) {
        recursive_find_equal(current->left_child, mean);
    }
    else {
        recursive_find_equal(current->right_child, mean);
    }
}

// implements the find operations based on the string argument operation
void DataTree::find(double mean, std::string operation) {

    // fails if tree does not exist at this moment
    if(!built_tree) {
        std::cout << "failure" << std::endl;
        return;
    }


    if(operation == "less") {
        // outputs blank line if mean provided is less than min, meaning no country satisfies the condition
        if(mean < Min) {
            std::cout << std::endl;
            return;
        }
        recursive_find_less(root_node, mean);
    }

    else if(operation == "equal") {
        // outputs blank line if mean provided is less than minimum or greater than maximum within tolerance 0.001, meaning no country satisfies the condition
        if(!(mean < (Min - 0.001) || mean > (Max + 0.001))) {
            recursive_find_equal(root_node, mean);
        }

    }

    else if(operation == "greater") {
        // outputs blank line if mean provided is greater than max, meaning no country satisfies the condition
        if(mean > Max) {
            std::cout << std::endl;
            return;
        }
        recursive_find_greater(root_node, mean);
    }

    std::cout << std::endl;


}

// removes the country_mean pairs from the nodes of the tree where country_name matches the arguments, deletes leaf if all countries in it are removed
void DataTree::delete_country(std::string country_name) {

    // failure if tree is empty/is not built
    if(root_node == nullptr) {

        std::cout << "failure" << std::endl;
        return;
    }

    // deletes country if in current node, else no need to search the remaining subtrees since country won't be found in them
    int index = root_node->h_findcountry(country_name);
    if(index != -1) {
        root_node->h_delete_data(index);
        root_node->size = root_node->size - 1;


        // Edit: monday - if root becomes an empty node, we delete the tree
        if(root_node->size == 0) {
            reset();
        }

        std::cout << "success" << std::endl;
    }
    else {

        std::cout << "failure" << std::endl;
        return;
    }

    // recursively deletes from the remaining child nodes calling the recursive helper function
    delete_country(root_node->left_child, country_name, &(root_node->left_child));
    delete_country(root_node->right_child, country_name, &(root_node->right_child));

    
}

//recursive helper for delete_country - we also include a pointer to the parents left/right child pointer so that we can delete leaf nodes
void DataTree::delete_country(Node* current, std::string country_name, Node** pointer_from_parent) {

    if(current == nullptr) {
        return;
    }

    // deletes country if in current node, else no need to search the remaining subtrees since country won't be found in them
    int index = current->h_findcountry(country_name);
    if(index != -1) {
        current->h_delete_data(index);
        current->size = current->size - 1;

        // edit: monday - current is a leaf node and now is empty, we delete it:
        if(current->is_leaf && current->size == 0) {
            *pointer_from_parent = nullptr;
            delete current;
            return;
        }

    }
    else {
        return;
    }

    delete_country(current->left_child, country_name, &(current->left_child));
    delete_country(current->right_child, country_name, &(current->right_child));
    

}

// finds either the leftmost or rightmost leaf node based on the condition and prints its country names
void DataTree::limits(std::string condition) {

    if(!built_tree) {
        std::cout << "failure" << std::endl;
        return;
    }


    Node * current = root_node;

    // prints all countries if root has no child nodes
    if(current->is_leaf) {
        current->h_print_countries();
        return;
    }

    // finds the leftmost leafnode
    if(condition == "lowest") {
        while(!(current->is_leaf)) {

            if(current->left_child != nullptr) {
                current = current->left_child;
            }
            else{
                current = current->right_child;
            }
        }  
    }
    // finds the rightmost leaf node
    else if (condition == "highest") {
        while(!(current->is_leaf)) {

            if(current->right_child != nullptr) {
                current = current->right_child;
            }
            else{
                current = current->left_child;
            }
        } 
    }
    
    // prints country names for the leaf node found
    current->h_print_countries();
    return;


}

// implements trace command as specified in the project requirements: Search the tree for the given country. If found, output the interval [L R] of every node that contains this country, starting from the root node and ending at the leaf.

void DataTree::trace(std::string country_name) {

    if(!built_tree) {
        std::cout << "failure" << std::endl;
        return;
    }

    Node* current = root_node;
    int index = current->h_findcountry(country_name);

    // root node has all the countries in the tree so if not found in the root, it is not in the tree,
    if(index == -1) {
        std::cout << "failure" << std::endl;
        return;
    }

    // traverses the tree and prints each range if country found in the node
    while(current != nullptr) {

        index = current->h_findcountry(country_name);

        if(index == -1) {
            break;
        }

        std::cout << current->local_min << " " << current->local_max << " ";


        if (current->data[index].mean < current->midpoint) {
            current = current->left_child;
        }
        else {
            current = current->right_child;
        }

    }

    std::cout << std::endl;
}
