// This program is designed to read a text file and record the
// frequency of all (ascii) characters that it contains.
// It then builds a Huffman Tree of these characters based on
// these frequency values.
//
// This code written by Michael Asnes
// (Partnered with Chris Wittenberg for class problemset #5)
// During: Feb, 2014
// For: CSCI3104, Algorithms
#ifndef __huffman__
#define __huffman__

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <climits>
#include <map>
#include <sstream>
#include <sys/time.h>

using namespace std;
// huff_node structure for the huffman tree
struct huff_node {
    string text;
    string key;
    int priority;
    huff_node* parent;
    huff_node* leftChild;
    huff_node* rightChild;
};

// pq structure for the priorityqueue
struct pq {
    int priority;
    huff_node* huffnode;
    pq* next;
};

// the main function
int main();

// create a new huff_node for the huffman tree
// this huff_node's children are initially set to NULL
huff_node* init_huff_node(string text, int priority);

// links an existing huff_node (node) to other nodes
huff_node* link_huff_node(huff_node* &node, huff_node* parent, huff_node* leftChild, huff_node* rightChild);

// encode the character array into a string of huffman values;
string huff_encoding_string(char* input_array);

// remove 2 nodes from the  priority queue, make a
// parent node with the  combined string and value of
// these noes  link the nodes together in a parent->
// children,  children->parent fashion. Insert parent
// back into the  priority queue, then return the parent
huff_node* huff_build(pq* &queue);

// add the key to all members of the huffman tree
// recursion_string should start blank!
huff_node* huff_number(huff_node* &node, string recursion_string);

// add the key to all members of the huffman tree
huff_node* huff_add_keys(huff_node* &node, string recursion_string);

// print out the huffman tree (feed this the
// root node, and the number of levels down you
// are willing to go)
void huff_print(huff_node* &huffnode, int maxdepth, bool leafs_only);

// recursion function to enable huff_print()
void huff_print_recurse(huff_node* &huffnode, int maxdepth, bool leafs_only);


// This function reads the input file specified, currently set to:
// csci3104_spring2014_ps5_data
// into a char array, then sends each character in the array to the
// mapper function
char * input_reader();

// function to iterate through the file_char_array and print it to stdout
void read_char_array(char* file_char_array);

// This function reads characters (given by the reader() function)
// and places them into a global map. The map contains all
// characters entered, and the frequency of the different
// characters in the given file
int mapper(char c);

// read map contents to stdout
void char_int_map_reader(map<char,int> readme, string map_name);
void string_char_map_reader(map<string,char> readme, string map_name);
void char_string_map_reader(map<char,string> readme, string map_name);

// initialize the priority queue
pq* init_priority_queue();

// populate a priority queue with the values in our map
// this priority queue can then be used for the huffman pogram
pq* pq_populate();

// duplicate a given priority queue
pq* pq_duplicate(pq* node);

// insert an element to the priority queue
pq* pq_insert(pq* &queue, int priority, huff_node* huffnode);

// remove first element from the priority queue
pq* pq_remove(pq* &queue);

// view first element in the priority queue
pq* pq_peek(pq* &queue);

void populate_huffman_maps(pq* &queue);

// print the priority queue for debugging purposes
void pq_print(pq* &queue);

#endif
