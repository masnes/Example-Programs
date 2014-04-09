// This program is designed to read a text file and record the
// frequency of all (ascii) characters that it contains.
// It then builds a Huffman Tree of these characters based on
// these frequency values.
//
// This code written by Michael Asnes
// (Partnered with Chris Wittenberg for class problemset #5)
// During: Feb, 2014 
// For: CSCI3104, Algorithms
#include <sys/time.h>
#include "huffman.h"

/*
 * DONE (mostly): Remove debugging strings once program is finished
 */

// global string delineating filename of file to be used
// (This variable is currently unused)
string name_of_file = "csci3104_spring2014_PS5_data.txt";

// global map to contain characters used in the file, and their frequencies
map<char,int> character_frequency;
// global map to contain huffman encoding strings and their associated characters
map<string,char> huffman_decoding;
// global map to contain huffman encoding strings and their associated characters
map<char,string> huffman_encoding;

// counter for hufmman operations
int huff_op_counter = 0;

int main () {
    // for recodring the time that the huffman encoding functions take
    timespec ts1, ts2, ts3, ts4;
    // string that the input file will be encoded into
    string encoding_string;
    // priority queue to be used for building the huffman tree
    pq* p = new pq;
    // priority queue used to preserve the first priority queue
    pq* q = new pq;
    // read the given input file into a character array
    // then place character array into a map
    char * input_array = input_reader();
    // read the created character array to stdout (optional function)
    read_char_array(input_array);
    // read the created map to stdout (optional function)
    char_int_map_reader(character_frequency, "Character frequency");
    // populate the priority queue p
    p = pq_populate();
    // duplicate p into q to preserve its order
    // (p will be emptied as the huffman tree is generated)
    q = pq_duplicate(p);
    // print the priority queue p to stdout (optional function)
    // (before it has been emptied)
    //pq_print(p);
    // build the huffman tree (and record the time)

    // clock_gettime is a system dependent function
    // I'm commenting it out so that the code compiles for all systems
    // clock_gettime(CLOCK_REALTIME, &ts1);
 
    huff_node* h = huff_build(p);
    // attach keys to the huffman tree (and record the time)
    huff_add_keys(h, "");

    // clock_gettime is a system dependent function
    // I'm commenting it out so that the code compiles for all systems
    // clock_gettime(CLOCK_REALTIME, &ts2);

    // print the initial order of the priority queue again
    // This time the keys associated with each entry will
    // also be displayed
    pq_print(q);
    // print out information on the huffman tree "h" (optional function)
    // (down to a maximum depth of 10 levels)
    //huff_print(h, 10, 0);
    //huff_print(h, 10, 1);
    // now populate maps for huffman encoding and decoding

    // clock_gettime is a system dependent function
    // I'm commenting it out so that the code compiles for all systems
    // clock_gettime(CLOCK_REALTIME, &ts3);

    populate_huffman_maps(q);
    // now read these maps (for debugging purposes)
    //char_string_map_reader(huffman_encoding, "Huffman encoding");
    //string_char_map_reader(huffman_decoding, "Huffman decoding");
    encoding_string = huff_encoding_string(input_array);

    // clock_gettime is a system dependent function
    // I'm commenting it out so that the code compiles for all systems
    // clock_gettime(CLOCK_REALTIME, &ts4);

    cout << "The encoding string is: \n" << encoding_string << endl;
    cout << "The length of the encoding string is " << encoding_string.length() << " characters." << endl;
    cout <<  (ts4.tv_nsec - ts3.tv_nsec) << endl;
    return 0;
}

// code to create a huffman tree node (huff_node)
huff_node* init_huff_node(string text, int priority) {
    huff_node* init = new huff_node;
    init->text = text;
    init->key = "";
    init->priority = priority;
    init->parent = NULL;
    init->leftChild = NULL;
    init->rightChild = NULL;
    return init;
}

// code to link a huffman tree node (huff_node)
// no links are built towards null values,
// so passing NULL to an argument for this function will preserve
// whatever value that argument corresponds to
// e.g. if NULL is passed for leftChild, leftChild will
// be preserved
huff_node* link_huff_node(huff_node* &node, huff_node* parent, huff_node* leftChild, huff_node* rightChild) {
    if (node == NULL) {
        cout << "Error: Null node passed to link_huff_node linker function!" << endl;
        return NULL;
    }
    if (parent != NULL)
        node->parent = parent;
    if (leftChild != NULL)
        node->leftChild = leftChild;
    if (rightChild != NULL)
        node->rightChild = rightChild;
    return node;
}

// encode the char array containing the input from our file
// into a huffman coded string
string huff_encoding_string(char* input_array) {
    int i = 0;
    string encoding_string = "";
    while (input_array[i]) {
        // use the huffman_encoding map to do the encoding
        encoding_string.append(huffman_encoding[input_array[i]]);
        i++;
    }
    return encoding_string;
}

// TODO: this function
string huff_decoding_string(string encoding_string) {
    return "";
}


// remove 2 nodes from the  priority queue, make a
// parent node with the  combined string and value of
// these noes  link the nodes together in a parent->
// children,  children->parent fashion. Insert parent
// back into the  priority queue, then return the parent
//
huff_node* huff_build(pq* &queue) {
    while (true) {
        // remove 2 nodes from tree
        pq* node1 = pq_remove(queue);
        pq* node2 = pq_remove(queue);
        /* problem cases */
        // both nodes null
        if (node1 == NULL && node2 == NULL) {
            cout << "Error! the pq was empty when the huffman tree was initialized! " << endl;
            return NULL;
            huff_op_counter++;
        }
        //  node 1 has info, node 2 is null
        //  only 1 node left in tree
        //  this node is the root node!
        if (node2 == NULL) {
            huff_node* root = node1->huffnode;
            return root;
        }
        // node 2 has info, node 1 is null
        // this case should never happen!
        if (node1 == NULL) {
            cout << "huff_build ERROR: null node at beginning of priority queue, non-null node at end of priority queue!" << endl;
        }
        /* end problem cases */
        /* now working with 2 good nodes pulled from pq */
        // designate children
        huff_node* leftChild = node1->huffnode;
        huff_node* rightChild = node2->huffnode;
        /* build parent */
        string string_to_insert = node1->huffnode->text + node2->huffnode->text;
        int int_to_insert = node1->huffnode->priority + node2->huffnode->priority;
        huff_node* parent = init_huff_node(string_to_insert, int_to_insert);
        /* link nodes together */
        // (passing NULL preserves any existing link)
        link_huff_node(leftChild, parent, NULL, NULL);
        link_huff_node(rightChild, parent, NULL, NULL);
        link_huff_node(parent, NULL, leftChild, rightChild);
        pq_insert(queue, parent->priority, parent);
    }
}

// add the key to all members of the huffman tree
huff_node* huff_add_keys(huff_node* &node, string recursion_string) {
    if (node == NULL)
        return NULL;
    huff_op_counter++;
    node->key = recursion_string;
    // the append method will alter the string permanently
    // so using 2 strings to avoid excess numbers being appened
    // to the recursion_string down the line
    string recursion_string_left = recursion_string;
    string recursion_string_right = recursion_string;
    if (node->leftChild != NULL)
        huff_add_keys(node->leftChild, recursion_string_left.append("0"));
    if (node->rightChild != NULL)
        huff_add_keys(node->rightChild, recursion_string_right.append("1"));
}

void huff_print_recurse(huff_node* &huffnode, int maxdepth, bool leafs_only) {
    huff_op_counter++;
    if (huffnode == NULL)
        return;
    if (maxdepth == 0)
        return;
    if (maxdepth == 1)
        //cout << "|k=\"" << huffnode->key << "\", t=\"" << huffnode->text << "\", p=\"" << huffnode->priority << "\"|   ";
        if (leafs_only == 0)
            cout << "{" << huffnode->text << "}{" << huffnode->key << "} ";
        else {
            if (huffnode->text.size() == 1)
                cout << "{" << huffnode->text << "}{" << huffnode->key << "} ";
        }
    else {
        if (leafs_only == 0)  {
            if (huffnode->leftChild != NULL)
                huff_print_recurse(huffnode->leftChild, maxdepth-1, 0);
            if (huffnode->rightChild != NULL)
                huff_print_recurse(huffnode->rightChild, maxdepth-1, 0);
        }
        else {
            if (huffnode->leftChild != NULL)
                huff_print_recurse(huffnode->leftChild, maxdepth-1, 1);
            if (huffnode->rightChild != NULL)
                huff_print_recurse(huffnode->rightChild, maxdepth-1, 1);
        }
    }
}

void huff_print(huff_node* &huffnode, int maxdepth, bool leafs_only) {
    int startdepth = 1;
    cout << "Printing huffman tree \"level|{node}{key}\"";
    if (leafs_only)
        cout << " (leaf nodes only): \n";
    else
        cout << ": \n";
    if (huffnode == NULL)
        return;
    while (startdepth <= maxdepth) {
        cout << startdepth << " |";
        huff_print_recurse(huffnode, startdepth, leafs_only);
        startdepth++;
        cout << endl;
    }
    cout << endl;
}



// This function reads the input file specified, currently set to:
// csci3104_spring2014_ps5_data
// into a char array, then sends each character in the array to the
// mapper function
char * input_reader() {
    // take in file for reading
    ifstream in("csci3104_spring2014_PS5_data.txt");
    // put file contents in string: file_contents
    string file_contents((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    //old piece of code to read datafile, keeping in case it's helpful soon
    //ifstream datafile (filename);

    // creating char array file_char_array as an array form of the string
    char * file_char_array = new char [file_contents.length()+1];
    strcpy (file_char_array, file_contents.c_str());

    // now iterating through file_char_array and reading each
    // value into mapper()
    int i = 0;
    int successval = 1;

    // printing to stdout for debugging purposes
    while (file_char_array[i]) {
        // we don't want '\n' newline character, so avoid it
        if (file_char_array[i]!='\n') {
            // printing to stdout here for debugging purposes
            successval = mapper(file_char_array[i]);
            if (successval != 0) {
                cout << "Mapper was not successful, mapper returned: " << successval << endl;
                return file_char_array;
            }
        }
        i++;
    }

    // end line and clear ostream
    cout << endl << endl;
    return file_char_array;
}

void read_char_array(char* file_char_array) {
    // now iterating through file_char_array and reading each
    // value into mapper()
    int i = 0;

    if (file_char_array == NULL) {
        cout << "ERROR: the file character array passed to read_char_array was null!" << endl;
        return;
    }
    // printing to stdout
    if (file_char_array[i])
        cout << "Characters in the File Character Array: " << endl;
    while (file_char_array[i]) {
        // we don't want '\n' newline character, so avoid it
        if (file_char_array[i]!='\n') {
            // printing to stdout here for debugging purposes
            cout << file_char_array[i];
        }
        i++;
    }
    cout << "\n" << endl;
}

// This function reads characters (given by the input_reader() function)
// and places them into a global map. The map contains all
// characters entered (besides '\n'), and the frequency of
// the different string characters from the given filename
int mapper(char c) {

    if (character_frequency[c]){
        character_frequency[c]++;
        return 0;
    }
    else {
        character_frequency[c] = 1;
        return 0;
    }
    return -1;
}

// TODO: uncomment or remove this
//// read char_int_map's contents
//void char_int_map_reader() {
    //    cout << "Mapped Values: " << endl;
    //    map<char,int>::iterator it;
    //    for (map<char,int>::iterator it=character_frequency.begin(); it!=character_frequency.end(); ++it)
    //    {
    //        if (it->first == '\n')
    //            cout << "|\\n| => " << it->second << '\n';
    //        else
    //            cout << "|" << it->first << "|  => " << it->second << '\n';
    //    }
    //
    //    cout << endl;
    //}

    // read the char_int_map's contents (for debugging purposes)
void char_int_map_reader(map<char,int> readme, string map_name) {
    cout << "Mapped Values for " << map_name << ":" << endl;
    map<char,int>::iterator it;
    for (map<char,int>::iterator it=readme.begin(); it!=readme.end(); ++it) {
        if (it->first == '\n')
            cout << "|\\n| => " << it->second << '\n';
        else
            cout << "|" << it->first << "|  => " << it->second << '\n';
    }
    cout << endl;
}

void string_char_map_reader(map<string,char> readme, string map_name) {
    cout << "Mapped Values for " << map_name << ":" << endl;
    map<string,char>::iterator it;
    for (map<string,char>::iterator it=readme.begin(); it!=readme.end(); ++it) {
        cout << "|" << it->first << "|  => " << it->second << '\n';
    }
    cout << endl;
}

void char_string_map_reader(map<char,string> readme, string map_name) {
    cout << "Mapped Values for " << map_name << ":" << endl;
    map<char,string>::iterator it;
    for (map<char,string>::iterator it=readme.begin(); it!=readme.end(); ++it) {
        cout << "|" << it->first << "|  => " << it->second << '\n';
    }
    cout << endl;
}
//// This creates a priority queue from the mapped values
//int priorityQueue(map* strings) {
    //return 0
    //}


// code to initialize the priority queue
// first node of the priority queue will be a
// special initizizer that is never changed/removed
// (this makings passing it's pointer around both easy and safe)
pq* init_priority_queue() {
    pq* p = new pq;
    p->priority = INT_MIN;
    p->next = NULL;
    p->huffnode = NULL;
    return p;
}

    // populate a priority queue with the values in our map
    // this priority queue can then be used for the huffman pogram
pq* pq_populate() {

    // p is the initiall node of the priority queue I will be using
    pq* p = new pq;
    p = init_priority_queue();

    map<char,int>::iterator it;
    for (map<char,int>::iterator it=character_frequency.begin(); it!=character_frequency.end(); ++it) {
        huff_op_counter++;
        // convert map char to string
        string map_string(1, it->first);
        // instert nour
        huff_node* h = init_huff_node(map_string, it->second);
        pq_insert(p, it->second, h);
    }
    return p;
}

pq* pq_duplicate(pq* node) {
    pq* q = new pq;
    q = init_priority_queue();
    if (node == NULL)
        return NULL;
    if (node->next == NULL)
        return NULL;
    while (node->next != NULL) {
        node = node->next;
        pq_insert(q, node->priority, node->huffnode);
    }
    return q;
}

// pq_insert an element into the correct place in the priority queue.
// Citation: Michael Asnes, Christian O'Keef
// modified version of previous 2013 work for
// University of Colorado at Boulder class CSCI2270, Fall 2012
pq* pq_insert(pq* &queue, int priority, huff_node* huffnode) {
    pq* new_pq = new pq;
    new_pq->priority = priority;
    new_pq->next = NULL;
    new_pq->huffnode = huffnode;

    if (queue->next == NULL) {
        queue->next = new_pq;
        return 0;
    }

    pq* cursor = queue;
    while (cursor->next != NULL && cursor->next->priority <= priority)
        cursor = cursor->next;
    new_pq->next = cursor->next;
    cursor->next = new_pq;
    return 0;

}

// Remove the highest priority element from the queue and return it.
// Citation: Michael Asnes, Christian O'Keef
// previous 2013 work for
// University of Colorado at Boulder class CSCI2270, Fall 2012
pq* pq_remove(pq* &queue) {
    if (queue == NULL || queue->next == NULL)
        return NULL;
    else {
        pq* pq_removed = queue->next;
        queue->next = queue->next->next;
        return pq_removed;
    }
}

// Return the highest priority element from the queue without removing it.
// Citation: Michael Asnes, Christian O'Keef
// modified version of previous 2013 work for
// University of Colorado at Boulder class CSCI2270, Fall 2012
// (This function is not necessary, but potentially useful for debugging)
pq* peek(pq* &queue) {
    if(queue == NULL || queue->next == NULL)
        return NULL;
    else
        return queue->next;
}



// print the priority queue for debugging purposes
void pq_print(pq* &queue) {
    pq* p = queue;

    // this if statment should avoid errors from passing a NULL pq
    if (p == NULL)  {
        cout << "ERROR: The priority queue passed to the pq_print function is NULL!" << endl;
        return;
    }
    else {
        // skip the initializer node
        // (the initializer node has no info, but is kept around
        // to make passing pointers to the priority queue easy)
        if (p->priority == INT_MIN)
            p = p->next;
    }

    /*
     *  TODO: for some reasons the while statement doesn't stop
     *  the queue from advancing before a seg-fault is encountered
     *  I've fixed the issue with an if/else break, but if there's
     *  time I should figure out why this is, in case it introduces
     *  other bugs
     */
    // Run through all items in the priority queue and print them.
    cout << "Priority Queue Values: " << endl;
    if (queue == NULL)
        cout << "pq_print() Error: The queue is NULL!" << endl;
    if (queue->next == NULL)
        cout << "pq_print() Error: The first item in the queue is gone!" << endl;
    while (queue != NULL && queue->next != NULL) {
        if (p->huffnode == NULL)
            cout << "pq_print() ERROR: pq* node->huffnode null! priority of pq node is: " <<  p->priority << endl;
        else {
            cout << "\"" << p->huffnode->text << "\" => " << p->huffnode->priority << "| key: " << p->huffnode->key << endl;
            if (p->next != NULL)
                p = p->next;
            else {
                break;
            }
        }
    }
    cout << endl;
}

// move through the priority queue, populating both the encoding huffman map, and the decoding huffman map (huffman_encoding and huffman_decoding).
void populate_huffman_maps(pq* &queue) {
    pq* p = queue;

    // this if statment should avoid errors from passing a NULL pq
    if (p == NULL)  {
        cout << "ERROR: The priority queue passed to the pq_print function is NULL!" << endl;
        return;
    }
    else {
        // skip the initializer node
        // (the initializer node has no info, but is kept around
        // to make passing pointers to the priority queue easy)
        if (p->priority == INT_MIN)
            p = p->next;
    }

    /*
     *  TODO: for some reasons the while statement doesn't stop
     *  the queue from advancing before a seg-fault is encountered
     *  I've fixed the issue with an if/else break, but if there's
     *  time I should figure out why this is, in case it introduces
     *  other bugs
     */
    // Run through all items in the priority queue
    if (queue == NULL)
        cout << "pq_print() Error: The queue is NULL!" << endl;
    if (queue->next == NULL)
        cout << "pq_print() Error: The first item in the queue is gone!" << endl;
    while (queue != NULL && queue->next != NULL) {
        if (p->huffnode == NULL)
            cout << "pq_print() ERROR: pq* node->huffnode null! priority of pq node is: " <<  p->priority << endl;
        else {
            huff_op_counter++;
            string text = p->huffnode->text;
            if (p->huffnode->text.length() > 1)
                cout << "Error: huffnode had a string longer than one character" << endl;
            string key = p->huffnode->key;
            if (text.length() > 1)
                cout << "ERROR: populate_huffman_maps pulled a huffman node with a string that is longer than 1 character! " << endl;
            huffman_encoding[text[0]] = key;
            huffman_decoding[key] = text[0];
            if (p->next != NULL)
                p = p->next;
            else {
                break;
            }
        }
    }
}

