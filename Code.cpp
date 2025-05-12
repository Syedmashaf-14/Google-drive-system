//////......BASIC LIBRARY..........
#include <iostream>
#include <string>   
#include <cassert>  //for handling the error handling
#include <fstream>  //for file handling 
#include <iomanip>  //for using setw and setfilled
#include <Windows.h>    //for using the colors
#include <chrono>   //for get the current time and date


//  ANSII colors
#define red "\033[31m"  
#define green "\033[32m" 
#define blue "\033[34m"  
#define cyan "\033[36m"  
#define pink  "\033[35m" 
#define orange "\033[33m" 
#define yellow "\033[93m"
#define Dark_blue "\033[48;5;18m"   
#define reset "\033[0m"

using namespace std;

struct node     //the main node where data is being solved
{
    string file_names;
    int size;
    string owner;
    string date;
    string type;
    string content;

    node* next;
    node* prev;

    //constructors
    node() :file_names(" "), next(NULL), size(0), owner(" "), date(" "), type(" "), content(" "), prev(NULL) {}
    node(string name, int size, string owner, string date, string type, string c) :file_names(name),
        next(NULL), size(size), owner(owner),
        date(date), type(type), content(c), prev(NULL) {}
};

class AVL;
class AVLFolderSystem;


//..............................RECENTLY USED FILE...................//
class Queue
{
private:
    node* front;
    node* rear;
    int count;
public:
    Queue() :count(0)
    {
        front = rear = NULL;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void enqueue(string file_name, int size, string owner, string date, string type, string content)
    {
        node* newNode = new node(file_name, size, owner, date, type, content);

        if (rear == NULL)
        {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
        count++;
    }

    void dequeue(string& file_name, int& size, string& owner, string& date, string& type, string& content)
    {
        if (isEmpty())
        {
            cout << "Queue Underflow!\n";
            return;
        }
        node* temp = front;
        file_name = temp->file_names;
        size = temp->size;
        owner = temp->owner;
        date = temp->date;
        type = temp->type;
        content = temp->content;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        else
        {
            front->prev = NULL;
        }
        delete temp;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is empty.\n";
            return;
        }
        node* temp = front;
        cout << "Recently accessed files: " << endl;
        while (temp != NULL)
        {
            cout << "File Name: " << temp->file_names << " , Owner Name: " << temp->owner << " , size: " << temp->size << " ,date: " << temp->date << " ,content: " << temp->content << endl;
            temp = temp->next;
        }
        cout << endl;
    }
};


//........................RECYCLE BIN....................//
class stack
{
private:
    node* top;

public:

    stack() :top(NULL) {}
    bool isEmpty()
    {
        return top == NULL;
    }
    //storing the rcycle bin data

    void push(string file_name, int size, string owner, string date, string type, string content)
    {
        node* newNode = new node(file_name, size, owner, date, type, content);
        newNode->next = top;
        top = newNode;
    }


    void pop(string& file_name, int& size, string& owner, string& date, string& type, string& content)
    {
        if (isEmpty())
        {
            cout << "Stack Underflow!\n";
            return;
        }
        node* temp = top;
        file_name = temp->file_names;
        size = temp->size;
        owner = temp->owner;
        date = temp->date;
        type = temp->type;
        content = temp->content;
        top = top->next;
        delete temp;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Recycle bin is empty.\n";
            return;
        }
        node* temp = top;
        cout << "Files in recycle bin: " << endl;
        while (temp != NULL)
        {
            cout << "File Name: " << temp->file_names << " , Owner Name: " << temp->owner << " , size: " << temp->size << " ,date: " << temp->date << " ,content: " << temp->content << endl;
            temp = temp->next;
        }
        cout << endl;
    }
};


//....................META DATA...............
const int table_size = 100;
class hash_table
{
    string file_names[table_size];  //name of file
    string file_types[table_size];  //file type of file
    string file_dates[table_size];  //date of file
    string file_owners[table_size]; //onwer of file
    int file_sizes[table_size];     //size of file
    bool is_occupied[table_size];   //status of index

public:
    hash_table()    //default constructor
    {
        for (int i = 0; i < table_size; i++)
        {
            file_names[i] = " ";
            file_types[i] = " ";
            file_dates[i] = " ";
            file_owners[i] = " ";
            file_sizes[i] = 0;
            is_occupied[i] = false;
        }
    }


    int hash_value(int file_size)   //..returns the index position of the req file
    {
        return file_size % table_size;
    }

    string format_file_size(int size_in_kb) //conversion of Kb
    {
        string formatted_size;
        double size = size_in_kb;

        if (size < 1024)
        {
            formatted_size = to_string(size_in_kb) + " KB";
        }
        else if (size < 1024 * 1024)
        {
            formatted_size = to_string(size / 1024).substr(0, min((int)to_string(size / 1024).length(), 4)) + " MB";
        }
        else
        {
            formatted_size = to_string(size / (1024 * 1024)).substr(0, min((int)to_string(size / (1024 * 1024)).length(), 4)) + " GB";
        }

        return formatted_size;
    }

    void insert_at_table(string name, int size, string type, string date, string owner) //insert the data into hash table
    {
        int index = hash_value(size);       //determining the index postion that is vacent
        int starting_index = index;
        do
        {
            if (!is_occupied[index])
            {
                file_names[index] = name;
                file_types[index] = type;
                file_dates[index] = date;
                file_owners[index] = owner;
                file_sizes[index] = size;
                is_occupied[index] = true;
                return;
            }
            else
            {
                index = (index + 1) % table_size;   //linear open addressing
            }
        } while (index != starting_index);

        cout << "Table is full!\n";
    }

    bool search(string name)
    {
        for (int i = 0; i < table_size; i++)
        {
            if (is_occupied[i] && file_names[i] == name)
            {
                cout << "File found at index " << i << endl;
                display_file_info(i);
                return true;
            }
        }
        cout << "File not found" << endl;
        return false;
    }

    void delete_file_hash(string name)  //delete the file from the hash table
    {
        bool found = false;
        for (int i = 0; i < table_size; i++)
        {
            if (is_occupied[i] && file_names[i] == name)
            {
                is_occupied[i] = false;
                found = true;
                cout << "File removed from hash table: " << name << endl;
                break;
            }
        }

        if (!found)
        {
            cout << "File not found in hash table: " << name << endl;
        }
    }



    void display_file_info(int index)
    {
        if (!is_occupied[index])
        {
            cout << "No file at this index!" << endl;
            return;
        }

        cout << ".......... File Details .........." << endl;
        cout << "File Name: " << file_names[index] << endl;
        cout << "File Type: " << file_types[index] << endl;
        cout << "File Size: " << format_file_size(file_sizes[index]) << endl;
        cout << "Last Modified: " << file_dates[index] << endl;
        cout << "Owner: " << file_owners[index] << endl;


        cout << "...................................." << endl;
    }

    void display_all_files()
    {
        cout << "............... MY DRIVE ................" << endl;
        cout << left << setw(20) << "NAME" << setw(15) << "TYPE" << setw(15) << "SIZE"
            << setw(20) << "LAST MODIFIED" << setw(15) << "OWNER" << endl;
        cout << "..............................................." << endl;

        bool files_found = false;
        for (int i = 0; i < table_size; i++)
        {
            if (is_occupied[i])
            {
                files_found = true;
                cout << left << setw(20) << file_names[i] << setw(15) << file_types[i]
                    << setw(15) << format_file_size(file_sizes[i]) << setw(20) << file_dates[i]
                    << setw(15) << file_owners[i] << endl;
            }
        }

        if (!files_found)
        {
            cout << "No files found in drive!" << endl;
        }
        cout << "........................................." << endl;
    }
};


//............................storing the version of file...................//
class doubly_link_list
{
public:
    doubly_link_list* next;
    doubly_link_list* prev;
    string file_names;
    int size;
    string owner;
    string date;
    string type;
    string content;
    doubly_link_list() :next(NULL), prev(NULL), size(0), owner(" "), date(" "), type(" "), content() {}
    doubly_link_list(int size, string owner, string date, string type, string content) :next(NULL), prev(NULL), size(size), owner(owner), date(date), type(type), content(content) {}

};

//.............................main class for version.................//
class version
{
public:
    doubly_link_list* root;
    version() :root(NULL) {}

    void insert_at_list(int size, string owner, string date, string type, string content, doubly_link_list*& root)
    {
        doubly_link_list* newnode = new doubly_link_list(size, owner, date, type, content);
        if (root == NULL)
        {
            root = newnode;
        }
        else
        {
            doubly_link_list* temp = root;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newnode;
            newnode->prev = temp;
        }
    }

    void display_file_version(doubly_link_list* root)
    {
        if (root == NULL)
        {
            cout << "No data available \n";
            return;
        }
        doubly_link_list* temp = root;
        while (temp != NULL)
        {
            cout << "The version size:  " << temp->size << endl;
            cout << " owner: " << temp->owner << endl;
            cout << " type: " << temp->type << endl;
            cout << " date&time: " << temp->date << endl;
            cout << " content: " << temp->content << endl;
            cout << ".............................\n";
            temp = temp->next;
        }
    }

};


//..........................SIMPLE AVL class.............//
class AVL
{
public:
    //same paramters
    string name;
    int size;
    string type;
    string date;
    string owner;
    string content;
    int height;


    AVL* avlLeft;   //left node
    AVL* avlRight;  //right node
    AVL* parent;    //folder
    AVL* firstChild;    //first child of folder =file
    AVL* nextSibling;
    AVL* prevSibling;

    //constructors
    AVL() : name(" "), size(0), type(" "), date(" "), owner(" "), content(" "),
        height(1), avlLeft(NULL), avlRight(NULL), parent(NULL), firstChild(NULL),
        nextSibling(NULL), prevSibling(NULL) {}

    AVL(string name, int size, string type, string date, string owner, string content)
        : name(name), size(size), type(type), date(date), owner(owner), content(content),
        height(1), avlLeft(NULL), avlRight(NULL), parent(NULL), firstChild(NULL),
        nextSibling(NULL), prevSibling(NULL) {}
};

class AVLFolderSystem
{
private:
    AVL* root;      //the pointer of AVL class
    stack* recyclebin;
    Queue* recentFiles;
    hash_table h;   //hash table

    // The doubly_link_list* pointer for version history root:
    doubly_link_list* delta;

    //The basic function of AVL....//

    int getHeight(AVL* node)    //returns the height
    {
        if (node == NULL)
        {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(AVL* node) //calculate the balance factor of AVL
    {
        if (node == NULL)
        {
            return 0;
        }
        return getHeight(node->avlLeft) - getHeight(node->avlRight);
    }

    void updateHeight(AVL* node)    //updating height after insertion and deletion
    {
        if (node == NULL)
        {
            return;
        }
        node->height = 1 + max(getHeight(node->avlLeft), getHeight(node->avlRight));
    }

    AVL* rightRotate(AVL* y)
    {
        AVL* x = y->avlLeft;
        AVL* T2 = x->avlRight;

        x->avlRight = y;
        y->avlLeft = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    AVL* leftRotate(AVL* x)
    {
        AVL* y = x->avlRight;
        AVL* T2 = y->avlLeft;

        y->avlLeft = x;
        x->avlRight = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    AVL* insertNode(AVL* node, AVL* newItem)  //inserting the data by the names
    {
        if (node == NULL)
        {
            return newItem;
        }

        if (newItem->name < node->name)
        {
            node->avlLeft = insertNode(node->avlLeft, newItem);
        }
        else if (newItem->name > node->name)
        {
            node->avlRight = insertNode(node->avlRight, newItem);
        }
        else
            return node;

        updateHeight(node);
        int balance = getBalanceFactor(node);

        if (balance > 1 && newItem->name < node->avlLeft->name)
        {
            return rightRotate(node);
        }

        if (balance < -1 && newItem->name > node->avlRight->name)
        {
            return leftRotate(node);
        }

        if (balance > 1 && newItem->name > node->avlLeft->name)
        {
            node->avlLeft = leftRotate(node->avlLeft);
            return rightRotate(node);
        }

        if (balance < -1 && newItem->name < node->avlRight->name)
        {
            node->avlRight = rightRotate(node->avlRight);
            return leftRotate(node);
        }

        return node;
    }

    AVL* minValueNode(AVL* node)
    {
        AVL* current = node;
        while (current->avlLeft != NULL)
        {
            current = current->avlLeft;
        }

        return current;
    }

    AVL* deleteNode(AVL* root, string name) //deleting the file
    {
        if (root == NULL)
        {
            return root;
        }

        if (name < root->name)
        {
            root->avlLeft = deleteNode(root->avlLeft, name);
        }

        else if (name > root->name)
        {
            root->avlRight = deleteNode(root->avlRight, name);
        }

        else
        {
            if ((root->avlLeft == NULL) || (root->avlRight == NULL))
            {
                AVL* temp = root->avlLeft ? root->avlLeft : root->avlRight;

                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;

                delete temp;
            }
            else
            {
                AVL* temp = minValueNode(root->avlRight);

                root->name = temp->name;
                root->size = temp->size;
                root->type = temp->type;
                root->date = temp->date;
                root->owner = temp->owner;
                root->content = temp->content;
                root->firstChild = temp->firstChild;

                root->avlRight = deleteNode(root->avlRight, temp->name);
            }
        }

        if (root == NULL)
        {
            return root;
        }

        updateHeight(root);

        int balance = getBalanceFactor(root);

        if (balance > 1 && getBalanceFactor(root->avlLeft) >= 0)
            return rightRotate(root);

        if (balance > 1 && getBalanceFactor(root->avlLeft) < 0)
        {
            root->avlLeft = leftRotate(root->avlLeft);
            return rightRotate(root);
        }

        if (balance < -1 && getBalanceFactor(root->avlRight) <= 0)
        {
            return leftRotate(root);
        }

        if (balance < -1 && getBalanceFactor(root->avlRight) > 0)
        {
            root->avlRight = rightRotate(root->avlRight);
            return leftRotate(root);
        }

        return root;
    }

    //seraching for the content of the file alternative of hash table search
    AVL* search(AVL* root, string name)
    {
        if (root == NULL || root->name == name)
        {
            return root;
        }

        if (name < root->name)
        {
            return search(root->avlLeft, name);
        }
        else
        {
            return search(root->avlRight, name);
        }
    }

    void displayHierarchyHelper(AVL* folder, int depth)
    {
        if (folder == NULL)
        {
            return;
        }

        for (int i = 0; i < depth; i++)
            cout << "  ";
        cout << "- " << folder->name;
        if (folder->type != "folder")
        {
            cout << " (" << folder->size << " KB, " << folder->type << ")";
        }
        cout << endl;

        if (folder->firstChild)
        {
            displayHierarchyHelper(folder->firstChild, depth + 1);
        }

        if (folder->nextSibling)
        {
            displayHierarchyHelper(folder->nextSibling, depth);
        }
    }


    void inOrderTraversal(AVL* root)
    {
        if (root == NULL)
        {
            return;
        }
        inOrderTraversal(root->avlLeft);

        if (root->type != "folder")
        {
            cout << "Name: " << root->name << endl;
            cout << "Size: " << root->size << " KB" << endl;
            cout << "Type: " << root->type << endl;
            cout << "Date: " << root->date << endl;
            cout << "Owner: " << root->owner << endl;
            cout << "Content: " << root->content << endl;
            cout << ".............................." << endl;
        }


        inOrderTraversal(root->avlRight);
    }


    //search detail of folder
    AVL* findFolderByName(string folderName, AVL* currentFolder) {
        if (!currentFolder)
        {
            return NULL;
        }

        if (currentFolder->name == folderName && currentFolder->type == "folder")
        {
            return currentFolder;
        }

        AVL* found = NULL;
        AVL* child = currentFolder->firstChild;
        while (child)
        {
            found = findFolderByName(folderName, child);
            if (found)
            {
                return found;
            }
            child = child->nextSibling;
        }

        return NULL;
    }


public:
    version v;

    AVLFolderSystem()
    {
        recyclebin = new stack();
        recentFiles = new Queue();
        root = new AVL("My Drive", 0, "folder", getCurrentDateTime(), "System", "Root folder");
        delta = NULL;  // initialize version history root to null    
    }

    string getCurrentDateTime()
    {
        time_t now = time(NULL);
        tm local_time;
        char buffer[26];

        localtime_s(&local_time, &now);
        asctime_s(buffer, sizeof(buffer), &local_time);

        string dateTimeStr = string(buffer);
        dateTimeStr.erase(dateTimeStr.find_last_not_of(" \n\r\t") + 1);

        return dateTimeStr;
    }

    //calling the create the nodes(folder)....
    AVL* createItem(string name, int size, string owner, string date, string type, string content)
    {
        return new AVL(name, size, type, date, owner, content);
        
    }

    //creating the file into the folder function
    void addToFolder(AVL* parent, AVL* newItem)
    {
        if (!parent)
        {
            cout << "Error: Parent folder is null." << endl;
            delete newItem;
            return;
        }

        if (!parent->firstChild)
        {
            parent->firstChild = newItem;
        }
        else
        {
            AVL* temp = parent->firstChild;
            while (temp->nextSibling)
            {
                temp = temp->nextSibling;
            }
            temp->nextSibling = newItem;
            newItem->prevSibling = temp;
        }
        newItem->parent = parent;

        root = insertNode(root, newItem);
    }

    //search file in the folder
    AVL* findItemByName(string name)
    {
        return search(root, name);
    }

    //deleting the file content in the folder
    void deleteItem(string name)
    {
        AVL* node = findItemByName(name);
        if (!node)
        {
            cout << "File/folder not found: " << name << endl;
            return;
        }

        if (node == root) {
            cout << "Cannot delete the root folder 'My Drive'." << endl;
            return;
        }

        recyclebin->push(node->name, node->size, node->owner, node->date, node->type, node->content);
        h.delete_file_hash(node->name);
        if (node->parent)
        {
            if (node->parent->firstChild == node)
            {
                node->parent->firstChild = node->nextSibling;
                if (node->nextSibling)
                {
                    node->nextSibling->prevSibling = NULL;
                }
            }
            else
            {
                if (node->prevSibling)
                    node->prevSibling->nextSibling = node->nextSibling;
                if (node->nextSibling)
                    node->nextSibling->prevSibling = node->prevSibling;
            }
        }

        root = deleteNode(root, name);
        cout << "Item deleted and moved to recycle bin: " << name << endl;


    }
    //...........COMPRESSION........//
    string compressRLE(const string& file_content)
    {
        string compressed;
        int count = 1;

        if (file_content.empty())
        {
            return "";
        }

        for (int i = 1; i <= file_content.length(); i++)
        {
            if (i < file_content.length() && file_content[i] == file_content[i - 1])
            {
                count++;
            }
            else
            {
                if (count >= 2)
                {
                    compressed += to_string(count) + file_content[i - 1];
                }
                else
                {
                    compressed += file_content[i - 1];
                }
                count = 1;
            }
        }

        return compressed;
    }

    
    //reading of fille and compression is also being added in it
    void readFile(string name,bool status)
    {
        AVL* node = findItemByName(name);
        if (!node)  //check if null 
        {
            cout << "File not found: " << name << endl;
            return;
        }
        if (node->type == "folder")//check whether file is not searched but a folder is being searched
        {
            cout << "Cannot read content of a folder." << endl;
            return;
        }
        //else part

        cout << ".......... File Details .........." << endl;
        cout << "File Name: " << node->name << endl;
        cout << "File Type: " << node->type << endl;
        cout << "File Size: " << node->size << " KB" << endl;
        cout << "Last Modified: " << node->date << endl;
        cout << "Owner: " << node->owner << endl;
        string alpha = node->content;
        if (status == true)
        {
            node->content = compressRLE(node->content);
            cout << "Content: " << node->content << endl;
        }
        else if (status == false)
        {
            
            cout << "Content: " << alpha << endl;
        }
        cout << "...................................." << endl;

        recentFiles->enqueue(node->name, node->size, node->owner, node->date, node->type, node->content);
    }


    //updating the content of file
    void updateFile(string oldName, string newName, int size, string owner, string type, string content)
    {
        AVL* node = findItemByName(oldName);


        if (!node)
        {
            cout << "File not found: " << oldName << endl;
            return;
        }

        if (node->type == "folder")
        {
            cout << "Cannot update a folder like a file. Use specific folder operations if needed." << endl;
            return;
        }

        // Store previous version
        v.insert_at_list(node->size, node->owner, node->date, node->type, node->content, delta);

        if (oldName != newName)
        {

            if (findItemByName(newName))
            {
                cout << "Error: An item with the new name '" << newName << "' already exists." << endl;
                return;
            }


            AVL* newNode = new AVL(newName, size, type, getCurrentDateTime(), owner, content);

            //copying down the path that the previous file is having inorder to maintain the path

            newNode->parent = node->parent; //copying the folder the file was in
            newNode->prevSibling = node->prevSibling;   //copying the prev sibling
            newNode->nextSibling = node->nextSibling;   //copying the next sibling


            newNode->firstChild = node->firstChild; //copying the child


            AVL* child = newNode->firstChild;
            while (child) {
                child->parent = newNode;
                child = child->nextSibling;
            }


            if (newNode->prevSibling)
            {
                newNode->prevSibling->nextSibling = newNode;
            }
            if (newNode->nextSibling)
            {
                newNode->nextSibling->prevSibling = newNode;
            }


            if (newNode->parent && newNode->parent->firstChild == node)
            {
                newNode->parent->firstChild = newNode;
            }

            root = insertNode(root, newNode);
            node->firstChild = NULL;
            root = deleteNode(root, oldName);
            h.delete_file_hash(oldName);
            h.insert_at_table(newName, size, type, getCurrentDateTime(), owner);
            v.insert_at_list(size, owner, getCurrentDateTime(), type, content, delta);
            cout << "File renamed and updated successfully: " << oldName << " -> " << newName << endl;
        }
        else
        {

            node->size = size;
            node->owner = owner;
            node->date = getCurrentDateTime();
            node->type = type;
            node->content = content;
            h.delete_file_hash(oldName);
            h.insert_at_table(oldName, size, type, getCurrentDateTime(), owner);
            cout << "File updated successfully: " << oldName << endl;
        }
    }
    //display the folder and file included in it

    void displayHierarchy()
    {
        cout << ".......... FOLDER HIERARCHY .........." << endl;
        if (root) {
            displayHierarchyHelper(root, 0);
        }
        else {
            cout << "File system is empty." << endl;
        }
    }

    //in ordertranversal
    void displayFilesSortedBySize()
    {
        cout << ".......... FILES SORTED BY NAME .........." << endl;
        inOrderTraversal(root);
    }

    AVL* createFolder(string folderName)
    {
        AVL* parent = root;

        if (!parent) {
            cout << "Error: Root folder not found." << endl;
            return nullptr;
        }

        AVL* child = parent->firstChild;
        while (child)
        {
            if (child->name == folderName) {
                cout << "An item with the name '" << folderName << "' already exists in this folder." << endl;
                return child;
            }
            child = child->nextSibling;
        }

        string currentTime = getCurrentDateTime();
        AVL* newFolder = createItem(folderName, 0, parent->owner, currentTime, "folder", "");

        addToFolder(parent, newFolder);

        cout << "Folder created successfully: " << folderName << endl;
        return newFolder;
    }

    AVL* createFileInFolder(string folderName, string fileName, int size, string owner, string type, string content) {
        AVL* parent = findFolderByName(folderName, root);

        if (!parent)
        {
            cout << "Folder does not exist: " << folderName << endl;
            return nullptr;
        }

        AVL* child = parent->firstChild;
        while (child)
        {
            if (child->name == fileName) {
                cout << "An item with the name '" << fileName << "' already exists in folder '" << folderName << "'." << endl;
                return child;
            }
            child = child->nextSibling;
        }


        string currentTime = getCurrentDateTime();
        AVL* newFile = createItem(fileName, size, owner, currentTime, type, content);

        addToFolder(parent, newFile);

        cout << "File created successfully: " << fileName << " in folder: " << parent->name << endl;
        return newFile;
    }

    void listFolderContents(string folderName)
    {
        AVL* folder = findFolderByName(folderName, root);

        if (!folder)
        {
            cout << "Folder does not exist: " << folderName << endl;
            return;
        }

        cout << "Contents of folder: " << folder->name << endl;
        cout << "----------------------------------" << endl;

        AVL* child = folder->firstChild;
        if (!child)
        {
            cout << "Folder is empty" << endl;
            return;
        }

        cout << left << setw(20) << "NAME" << setw(15) << "TYPE" << setw(15) << "SIZE"
            << setw(20) << "LAST MODIFIED" << setw(20) << "OWNER" << endl;
        cout << "------------------------------------------------------------------------" << endl;

        while (child)
        {
            string sizeStr = child->type == "folder" ? "-" : to_string(child->size) + " KB";
            cout << left << setw(20) << child->name << setw(15) << child->type
                << setw(15) << sizeStr << setw(20) << child->date
                << setw(15) << child->owner << endl;
            child = child->nextSibling;
        }
    }

    bool copyFileDetails(string file_name, int& size, string& type, string& date, string& content, string& owner)
    {
        AVL* node = findItemByName(file_name);
        if (node == NULL)
        {
            return false;
        }

        if (node->type == "folder")
        {
            return false;
        }


        size = node->size;
        type = node->type;
        date = node->date;
        content = node->content;
        owner = node->owner;

        return true; // Return true to indicate success
    }

    stack* getRecycleBin() { return recyclebin; }
    Queue* getRecentFiles() { return recentFiles; }
    hash_table* getHashTable() { return &h; }

    doubly_link_list* getDelta() { return delta; } 
};

//.............................GRAPHS....................//
const int MAX_USERS = 10;

//first page 
struct loginedge
{
    string lastLoginTime;
    string lastLogoutTime;
};

//graphs instincts
struct user
{
    string username; //the user name for loging and sign up
    string password;  // the password
    string securityQuestion;
    string securityAnswer;
    loginedge loginInfo;    //obj of login edge
};


//graphs//
class graphs
{
private:
    user users[MAX_USERS];      //initializing the graphs 
    bool adjacencyMatrix[MAX_USERS][MAX_USERS] = { false }; //the status
    int userCount = 0;

    string getCurrentDateTime()         //current date and time
    {
        time_t now = time(NULL);
        tm local_time;
        char buffer[26];

        localtime_s(&local_time, &now);
        asctime_s(buffer, sizeof(buffer), &local_time);

        string dateTimeStr = string(buffer);
        dateTimeStr.erase(dateTimeStr.find_last_not_of(" \n\r\t") + 1);

        return dateTimeStr;
    }

    int findUserIndex(string username)  //check whether the user name exist or not
    {
        for (int i = 0; i < userCount; i++)
        {
            if (users[i].username == username)
            {
                return i;
            }
        }
        return -1;
    }

public:
    //sign up page//
    bool signup(string username, string password, string question, string answer)
    {
        if (userCount >= MAX_USERS)
        {
            cout << "Error: Maximum users reached." << endl;
            return false;
        }

        if (findUserIndex(username) != -1)
        {
            cout << "Error: Username '" << username << "' already exists." << endl;
            return false;
        }

        users[userCount++] = { username, password, question, answer, {} };
        cout << "User '" << username << "' signed up successfully." << endl;
        return true;
    }

    //connect the user for sharing
    bool connectUsers(string user1, string user2)
    {
        bool permission = false;

        int first_user_index = findUserIndex(user1);
        int second_user_index = findUserIndex(user2);
        if (first_user_index == -1 || second_user_index == -1)
        {
            cout << "Error: One or both users not found." << endl;
            return false;
        }

        if (first_user_index == second_user_index)
        {
            cout << "Error: Cannot connect a user to themselves." << endl;
            return false;
        }

        if (adjacencyMatrix[first_user_index][second_user_index]) {
            cout << "Users '" << user1 << "' and '" << user2 << "' are already connected." << endl;
            return false;
        }
        cout << "Enter if you want to give the permission: " << endl;
        cin >> permission;

        if (permission == true)
        {
            adjacencyMatrix[first_user_index][second_user_index] = true;
        }
        else
        {
            cout << "Permission is not granted: " << endl;
        }
       // adjacencyMatrix[second_user_index][first_user_index] = false;
        cout << "Users '" << user1 << "' and '" << user2 << "' connected successfully." << endl;
        return true;
    }

    //login page
    bool login(string username, string password)
    {
        int user_index = findUserIndex(username);
        if (user_index != -1 && users[user_index].password == password)
        {
            users[user_index].loginInfo.lastLoginTime = getCurrentDateTime();
            return true;
        }
        return false;
    }

    //log out
    void logout(string username)
    {
        int index_no = findUserIndex(username);
        if (index_no != -1)
        {
            users[index_no].loginInfo.lastLogoutTime = getCurrentDateTime();
            cout << "User '" << username << "' logged out successfully." << endl;
        }
        else {
            cout << "Error: User '" << username << "' not found." << endl;
        }
    }


    //resetting the password
    bool resetPassword(string username, string answer, string newPassword)
    {
        int index_no = findUserIndex(username);
        if (index_no != -1 && users[index_no].securityAnswer == answer)//if security ans is equal to user input then reset the credentials
        {
            users[index_no].password = newPassword;
            cout << "Password for user '" << username << "' reset successfully." << endl;
            return true;
        }
        cout << "Error: Invalid username or security answer." << endl;
        return false;
    }

    void displayUser(string username)   //display the users
    {
        int user_index = findUserIndex(username);
        if (user_index == -1)
        {
            cout << "Error: User '" << username << "' not found." << endl;
            return;
        }

        cout << ".......... User Details .........." << endl;
        cout << "User: " << username << endl;
        cout << "Last Login: " << users[user_index].loginInfo.lastLoginTime << endl;
        cout << "Last Logout: " << users[user_index].loginInfo.lastLogoutTime << endl;

        cout << "Connections: ";
        bool hasConnections = false;
        for (int i = 0; i < userCount; i++)
        {
            if (adjacencyMatrix[user_index][i])
            {
                cout << users[i].username << (i == userCount - 1 ? "" : ", ");
                hasConnections = true;
            }
        }
        if (!hasConnections) {
            cout << "None";
        }
        cout << endl;
        cout << ".................................." << endl;
    }

    bool userExists(string username) {
        return findUserIndex(username) != -1;
    }
};





class file_operation;

//the access like admin editor and viewer
class acess_control
{
protected:
    graphs* control;
    AVLFolderSystem* folder;
    string currentUser;

public:
    acess_control(graphs* c, AVLFolderSystem* avl, string user) : control(c), folder(avl), currentUser(user) {}

    //all the function of file and file operations
    virtual void readFile(string name_file,bool status) = 0;
    virtual void createFile(string folder_name, string file_name, int size, string file_type, string content) = 0;
    virtual void createFolder(string folder_name) = 0;
    virtual void deleteItem(string name) = 0;
    virtual void modifyFile(string old_name, string newname, int size, string type, string content) = 0;
    virtual void shareFile(string file_name, string share_whom) = 0;
    virtual void listFolderContents(string folder_name) = 0;
    virtual void displayHierarchy() = 0;
    virtual void searchFile(string file_name) = 0;
    virtual void displayAllFiles() = 0;
    virtual void displayRecycleBin() = 0;
    virtual void displayRecentFiles() = 0;

    virtual ~acess_control() = default;
};

//inheriting the admin 
class admin :public acess_control
{
    //admin can peform all the task
public:
    admin(graphs* c, AVLFolderSystem* avl, string user) :acess_control(c, avl, user) {}

    void readFile(string name_file, bool status) override
    {
        folder->readFile(name_file,status);
    }

    void createFile(string folder_name, string file_name, int size, string file_type, string content) override
    {
        folder->createFileInFolder(folder_name, file_name, size, currentUser, file_type, content);
        AVL* createdFile = folder->findItemByName(file_name);
        if (createdFile && createdFile->type != "folder")
        {
            folder->getHashTable()->insert_at_table(file_name, size, file_type, folder->getCurrentDateTime(), createdFile->owner);
        }
    }
    void createFolder(string folder_name) override
    {
        folder->createFolder(folder_name);
    }
    void deleteItem(string name) override
    {
        folder->deleteItem(name);
    }
    void modifyFile(string old_name, string newname, int size, string type, string content) override
    {
        folder->updateFile(old_name, newname, size, currentUser, type, content);
    }
    void shareFile(string file_name, string share_whom) override
    {
        AVL* fileNode = folder->findItemByName(file_name);
        if (!fileNode)
        {
            cout << "Error: File '" << file_name << "' not found." << endl;
            return;
        }
        if (fileNode->type == "folder")
        {
            cout << "Error: Cannot share a folder like a file." << endl;
            return;
        }
        if (!control->userExists(share_whom))
        {
            cout << "Error: User '" << share_whom << "' does not exist." << endl;
            return;
        }

        control->connectUsers(currentUser, share_whom);
        folder->readFile(file_name,false);
        folder->getHashTable()->insert_at_table(file_name, fileNode->size, fileNode->type, folder->getCurrentDateTime(), share_whom);
        cout << "File '" << file_name << "' shared with '" << share_whom << "'." << endl;
    }
    void listFolderContents(string folder_name) override { folder->listFolderContents(folder_name); }
    void displayHierarchy() override { folder->displayHierarchy(); }
    void searchFile(string file_name) override { folder->getHashTable()->search(file_name); }
    void displayAllFiles() override { folder->getHashTable()->display_all_files(); }
    void displayRecycleBin() override { folder->getRecycleBin()->display(); }
    void displayRecentFiles() override { folder->getRecentFiles()->display(); }
};

class editor :public acess_control
{
    //editors cannot execute the task
public:
    editor(graphs* c, AVLFolderSystem* avl, string user) :acess_control(c, avl, user) {}

    void readFile(string name_file, bool status) override
    {
        folder->readFile(name_file,status);
    }
    void createFile(string folder_name, string file_name, int size, string file_type, string content) override
    {
        folder->createFileInFolder(folder_name, file_name, size, currentUser, file_type, content);
        AVL* createdFile = folder->findItemByName(file_name);
        if (createdFile && createdFile->type != "folder") {
            folder->getHashTable()->insert_at_table(file_name, size, file_type, folder->getCurrentDateTime(), createdFile->owner);
        }
    }
    void createFolder(string folder_name) override
    {
        folder->createFolder(folder_name);
    }
    void deleteItem(string name) override
    {
        cout << "Editors do not have permission to delete items." << endl;
    }
    void modifyFile(string old_name, string newname, int size, string type, string content) override
    {
        AVL* fileNode = folder->findItemByName(old_name);
        if (!fileNode) {
            cout << "Error: File '" << old_name << "' not found." << endl;
            return;
        }
        if (fileNode->owner != currentUser)
        {
            cout << "Error: You do not own this file and cannot modify it." << endl;
            return;
        }
        folder->updateFile(old_name, newname, size, currentUser, type, content);
    }
    void shareFile(string file_name, string share_whom) override
    {
        AVL* fileNode = folder->findItemByName(file_name);
        if (!fileNode) {
            cout << "Error: File '" << file_name << "' not found." << endl;
            return;
        }
        if (fileNode->type == "folder") {
            cout << "Error: Cannot share a folder like a file." << endl;
            return;
        }
        if (fileNode->owner != currentUser) {
            cout << "Error: You do not own this file and cannot share it." << endl;
            return;
        }
        if (!control->userExists(share_whom)) {
            cout << "Error: User '" << share_whom << "' does not exist." << endl;
            return;
        }

        control->connectUsers(currentUser, share_whom);
        folder->readFile(file_name,false);
        folder->getHashTable()->insert_at_table(file_name, fileNode->size, fileNode->type, folder->getCurrentDateTime(), share_whom);
        cout << "File '" << file_name << "' shared with '" << share_whom << "'." << endl;
    }
    void listFolderContents(string folder_name) override { folder->listFolderContents(folder_name); }
    void displayHierarchy() override { folder->displayHierarchy(); }
    void searchFile(string file_name) override { folder->getHashTable()->search(file_name); }
    void displayAllFiles() override { folder->getHashTable()->display_all_files(); }
    void displayRecycleBin() override {
        cout << "Editors do not have permission to view the recycle bin." << endl;
    }
    void displayRecentFiles() override { folder->getRecentFiles()->display(); }
};

class viewer :public acess_control
{

    //only can view the content of the file//
public:
    viewer(graphs* c, AVLFolderSystem* avl, string user) :acess_control(c, avl, user) {}

 void readFile(string name_file, bool status) override
    {
        AVL* fileNode = folder->findItemByName(name_file);
        if (!fileNode) {
            cout << "Error: File '" << name_file << "' not found." << endl;
            return;
        }
        if (fileNode->type == "folder") {
            cout << "Cannot read content of a folder." << endl;
            return;
        }
        folder->readFile(name_file,status);
    }
    void createFile(string folder_name, string file_name, int size, string file_type, string content) override
    {
        cout << "Viewers do not have permission to create files." << endl;
    }
    void createFolder(string folder_name) override
    {
        cout << "Viewers do not have permission to create folders." << endl;
    }
    void deleteItem(string name) override
    {
        cout << "Viewers do not have permission to delete items." << endl;
    }
    void modifyFile(string old_name, string newname, int size, string type, string content) override
    {
        cout << "Viewers do not have permission to modify files." << endl;
    }
    void shareFile(string file_name, string share_whom) override
    {
        cout << "Viewers do not have permission to share files." << endl;
    }
    void listFolderContents(string folder_name) override { folder->listFolderContents(folder_name); }
    void displayHierarchy() override { folder->displayHierarchy(); }
    void searchFile(string file_name) override { folder->getHashTable()->search(file_name); }
    void displayAllFiles() override { folder->getHashTable()->display_all_files(); }
    void displayRecycleBin() override {
        cout << "Viewers do not have permission to view the recycle bin." << endl;
    }
    void displayRecentFiles() override { folder->getRecentFiles()->display(); }
};



//the main class in which all the obj are being present
class file_operation
{
public:
    AVLFolderSystem* folderSystem;  //file opeation
    graphs g;   //graph 
    acess_control* acess;   //check the status of user

    file_operation() : g(), acess(NULL)
    {
        folderSystem = new AVLFolderSystem();
    }

    string getCurrentDateTime()
    {
        time_t now = time(nullptr);
        tm local_time;
        char buffer[26];

        localtime_s(&local_time, &now);
        asctime_s(buffer, sizeof(buffer), &local_time);

        string dateTimeStr = string(buffer);
        dateTimeStr.erase(dateTimeStr.find_last_not_of(" \n\r\t") + 1);

        return dateTimeStr;
    }

    void create_file(string folderName, string file_name, int size, string type, string content)
    {
        if (acess)
        {
            acess->createFile(folderName, file_name, size, type, content);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void delete_item(string name)
    {
        if (acess) {
            acess->deleteItem(name);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void create_folder(string folder_name)
    {
        if (acess) {
            acess->createFolder(folder_name);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void list_folder_contents(string folder_name)
    {
        if (acess) {
            acess->listFolderContents(folder_name);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void display_hierarchy()
    {
        if (acess) {
            acess->displayHierarchy();
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void search_file(string file_name)
    {
        if (acess)
        {
            acess->searchFile(file_name);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void display_all_files()
    {
        if (acess)
        {
            acess->displayAllFiles();
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void open_file(string file_name,bool status)
    {
        if (acess)
        {
            acess->readFile(file_name,status);
        }
        else
        {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void update_file(string oldName, string newName, int size, string type, string content)
    {
        if (acess)
        {
            acess->modifyFile(oldName, newName, size, type, content);
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void share_file(string file_name, string share_whom)
    {
        if (acess)
        {
            acess->shareFile(file_name, share_whom);
        }
        else
        {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void display_recycle_bin()
    {
        if (acess) {
            acess->displayRecycleBin();
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void display_recent_files()
    {
        if (acess)
        {
            acess->displayRecentFiles();
        }
        else {
            cout << "Error: User not logged in or access control not set." << endl;
        }
    }

    void setAccessControl(string userType, string username)
    {
        delete acess;
        acess = nullptr;

        if (userType == "Admin" || userType=="admin")
        {
            acess = new admin(&g, folderSystem, username);
        }
        else if (userType == "Editor" || userType=="editor") {
            acess = new editor(&g, folderSystem, username);
        }
        else if (userType == "Viewer" || userType=="viewer")
        {
            acess = new viewer(&g, folderSystem, username);
        }
        else {
            cout << "Invalid: Access control not set." << endl;
        }
    }


};
class cloud_sinc
{
    string file_names;
    int size;
    string owner;
    string date;
    string type;
    string content;

    cloud_sinc* nexts;
    

    //constructors
    cloud_sinc() :file_names(" "), nexts(NULL), size(0), owner(" "), date(" "), type(" "), content(" ") {}
    cloud_sinc(string name, int size, string owner, string date, string type, string c) :file_names(name),
        nexts(NULL), size(size), owner(owner),
        date(date), type(type), content(c) {}

};
class cloud_ADT
{
private:
    node* front;
    node* rear;
    int counts;
public:
    cloud_ADT() :counts(0)
    {
        front = rear = NULL;
    }

    bool isEmpty()
    {
        return front == NULL;
    }

    void enqueues(string file_name, int size, string owner, string date, string type, string content)
    {
        node* newNode = new node(file_name, size, owner, date, type, content);

        if (rear == NULL)
        {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
        counts++;
    }

    void dequeues(string& file_name, int& size, string& owner, string& date, string& type, string& content)
    {
        if (isEmpty())
        {
            cout << "cloud is empty\n";
            return;
        }
        node* temp = front;
        file_name = temp->file_names;
        size = temp->size;
        owner = temp->owner;
        date = temp->date;
        type = temp->type;
        content = temp->content;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        else
        {
            front->prev = NULL;
        }
        delete temp;
        counts--;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Cloud is empty.\n";
            return;
        }
        node* temp = front;
        cout << "files in storage: " << endl;
        while (temp != NULL)
        {
            cout << "File Name: " << temp->file_names << " , Owner Name: " << temp->owner << " , size: " << temp->size << " ,date: " << temp->date << " ,content: " << temp->content << endl;
            temp = temp->next;
        }
        cout << endl;
    }
};



int main()
{
    try 
    {
        
        file_operation fo;  // main file operation object
        doubly_link_list* delta = nullptr;   // for storing the version history root
        version v;  // version object

        string currentUser = "";
        bool isLoggedIn = false;
        string user_type;
        bool compression = false;
        cloud_ADT cloud;
        bool cloud_status = NULL;

        while (true)
        {
            try {
                if (!isLoggedIn) {
                    // Login/Signup menu
                    cout << red << "--------------------------------------------------" << endl;
                    cout << green << "              GOOGLE DRIVE SYSTEM            \n";
                    cout << "1. Login" << endl;
                    cout << "2. Sign Up" << endl;
                    cout << "3. Reset Password" << endl;
                    cout << "0. Exit" << endl;
                    cout << red << "--------------------------------------------------" << endl;
                    cout << "Enter your choice: ";
                    int choice;
                    if (!(cin >> choice)) {
                        throw runtime_error("Invalid input. Expected a number.");
                    }
                    cin.ignore(); 

                    switch (choice)
                    {
                    case 1:
                    {
                        cout << blue << "..............LOGIN PAGE........................\n";
                        string username, password;
                        cout << "Enter username: ";
                        getline(cin, username);
                        cout << "Enter password: ";
                        getline(cin, password);
                        if (fo.g.login(username, password))
                        {
                            currentUser = username;
                            cout << "Welcome on the google drive Mr/Miss: " << currentUser << " at: " << fo.getCurrentDateTime() << endl;
                            isLoggedIn = true;

                            cout << "Enter your user type (Admin, Editor, Viewer): ";
                            getline(cin, user_type);
                           
                            fo.setAccessControl(user_type, currentUser);
                        }
                        else
                        {
                            cerr << red << "Invalid username or password!" << reset << endl;
                           
                        }
                        break;
                    }
                    case 2:
                    {
                        cout << blue << "..............SIGN UP PAGE........................\n";
                        string username, password, question, answer;
                        cout << "Enter username: ";
                        getline(cin, username);
                        cout << "Enter password: ";
                        getline(cin, password);
                        cout << "Enter security question: ";
                        getline(cin, question);
                        cout << "Enter security answer: ";
                        getline(cin, answer);
                        fo.g.signup(username, password, question, answer);
                        break;
                    }
                    case 3:
                    {
                        cout << blue << "..............RESET CREDENTIALS PAGE........................\n";
                        string username, answer, newPassword;
                        cout << "Enter username: ";
                        getline(cin, username);
                        cout << "Enter security answer: ";
                        getline(cin, answer);
                        cout << "Enter new password: ";
                        getline(cin, newPassword);
                        fo.g.resetPassword(username, answer, newPassword);
                        break;
                    }
                    case 0:
                    {
                        cout << "Exiting program..." << endl;
                        return 0;
                    }
                    default:
                        cout << red << "Invalid choice!" << reset << endl;
                    }
                }
                else {
                    // Main menu after login
                    system("cls");
                    cout << red << "--------------------------------------------------" << reset<<endl;
                    cout << green << "              GOOGLE DRIVE SYSTEM            " << reset<<endl;
                    cout << red << "--------------------------------------------------" << reset << endl;
                    cout << "Logged in as: " << currentUser << " (" << user_type << ")" << endl;
                    cout << "1. Create File" << endl;
                    cout << "2. Create Folder" << endl;
                    cout << "3. List Folder Contents" << endl;
                    cout << "4. Display Hierarchy" << endl;
                    cout << "5. Search File" << endl;
                    cout << "6. Display All Files (Hash Table)" << endl;
                    cout << "7. Open File" << endl;
                    cout << "8. Update File" << endl;
                    cout << "9. Delete Item" << endl;
                    cout << "10. Share File" << endl;
                    cout << "11. Display Recycle Bin" << endl;
                    cout << "12. Display Recent Files" << endl;
                    cout << "13. Logout" << endl;
                    cout << "14. File version" << endl;
                    cout << "15. To view cloud: " << endl;
                    cout << "0. Exit" << endl;
                    cout << "Enter your choice: ";
                    int choice;
                    if (!(cin >> choice)) {
                        throw runtime_error("Invalid input. Expected a number.");
                    }
                    cin.ignore();

                    switch (choice)
                    {
                    case 1:
                    {
                        system("cls");
                        try {
                            cout << red << "..............CREATE FILE........................\n";
                            string folderName, fileName, fileType, fileContent;
                            int fileSize;
                            ofstream write("current_data.txt", ios::app);
                            if (!write)
                            {
                                throw runtime_error("Failed to open 'current_data.txt' for writing");
                            }
                            cout << "Enter folder name to create file in: ";
                            getline(cin, folderName);
                            cout << "Enter file name: ";
                            getline(cin, fileName);
                            cout << "Enter file size (KB): ";
                            if (!(cin >> fileSize))
                            {
                                throw runtime_error("Invalid input for file size");
                            }

                            cin.ignore();
                            cout << "Enter file type(doc,pfd,excel etc): ";
                            getline(cin, fileType);
                            cout << "Enter file content: ";
                            getline(cin, fileContent);

                            cout << "Enter if you wanted to sinc these files in the cloud storage or not :" << endl;
                            cin >> cloud_status;

                            if (cloud_status == true)
                            {
                                cloud.enqueues(fileName, fileSize, currentUser, fo.getCurrentDateTime(), fileType, fileContent);
                            }
                            else
                            {
                                cout << "You have chossen not to sinc the file in cloud \n";
                            }



                            write << "Folder Name: " << folderName << endl;
                            write << "File name: " << fileName << endl;
                            write << "Size: " << fileSize << endl;
                            write << "Type: " << fileType << endl;
                            write << "content: " << fileContent << endl;
                            write << ".................................\n";
                            write.close();
                            fo.create_file(folderName, fileName, fileSize, fileType, fileContent);
                            cout << blue << "..................................................." << reset<<endl;
                        }
                        catch (const exception& e) 
                        {
                            cerr << red << "Error during file creation: " << e.what() << reset << endl;
                        }
                        cin.ignore();
                        break;
                    }
                    case 2:
                    {
                        system("cls");
                        try {
                            cout << red << "..............CREATE FOLDER........................" << reset<<endl;
                            string folderName;
                            cout << "Enter folder name: ";
                            getline(cin, folderName);
                            fo.create_folder(folderName);
                            break;
                        }
                        catch (const exception& e) 
                        {
                            cerr << red << "Error creating folder: " << e.what() << reset << endl;
                        }
                        cin.ignore();
                    }
                    case 3:
                    {
                        system("cls");
                        try 
                        {
                            cout << red << "..............FOLDER CONTENT........................"<<reset<<endl;
                            string folderName;
                            cout << "Enter folder name: ";
                            getline(cin, folderName);
                            fo.list_folder_contents(folderName);
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e)
                        {
                            cerr << red << "Error listing folder contents: " << e.what() << reset << endl;
                        }
                        
                    }
                    case 4:
                        system("cls");
                        try 
                        {
                            cout << red << "..............DISPLAY HIERARCHY........................"<<reset<<endl;
                            fo.display_hierarchy();
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying hierarchy: " << e.what() << reset << endl;
                        }
                      
                    case 5:
                        system("cls");
                        try {
                            cout << red << "..............SEARCH FILE........................"<<reset<<endl;
                            string fileName;
                            cout << "Enter file name to search: ";
                            getline(cin, fileName);
                            fo.search_file(fileName);
                            cin.ignore();
                            break;
                            
                        }
                        catch (const exception& e) {
                            cerr << red << "Error searching file: " << e.what() << reset << endl;
                        }
                       
                    case 6:
                        system("cls");
                        try 
                        {
                            cout << red << "..............DISPLAY ALL FILE........................"<<reset<<endl;
                            fo.display_all_files();
                            cin.ignore();
                            break;
                            
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying all files: " << e.what() << reset << endl;
                        }
                       
                    case 7:
                        system("cls");
                        try 
                        {
                            string fileName;
                            cout << red << "..............OPEN FILE........................"<<reset<<endl;
                            cout << "Enter file name to open: ";
                            getline(cin, fileName);
                            cout << "Enter you if you want to apply the compression if applicable or not: ";
                            cin >> compression;
                            fo.open_file(fileName,compression);
                            Sleep(5000);
                            break;
                            
                        }
                        catch (const exception& e) {
                            cerr << red << "Error opening file: " << e.what() << endl;
                        }
                       
                    case 8:
                        system("cls");
                        try 
                        {
                            cout << red << "..............UPDATE FILE........................" << reset << endl;
                            string oldName, newName, fileType, fileContent;
                            int fileSize;
                            ofstream update_write("updated.txt", ios::app);
                            if (!update_write)
                            {
                                throw runtime_error("Failed to open 'updated.txt' for writing");
                            }

                            cout << "Enter old file name: ";
                            getline(cin, oldName);
                            cout << "Enter new file name: ";
                            getline(cin, newName);
                            cout << "Enter new file size (KB): ";
                            if (!(cin >> fileSize)) throw runtime_error("Invalid input for file size");
                            cin.ignore();
                            cout << "Enter new file type: ";
                            getline(cin, fileType);
                            cout << "Enter new file content: ";
                            getline(cin, fileContent);

                            update_write << "File name: " << newName << endl;
                            update_write << "Size: " << fileSize << endl;
                            update_write << "Type: " << fileType << endl;
                            update_write << "content: " << fileContent << endl;
                            update_write << ".................................\n";
                            update_write.close();
                           
                            fo.update_file(oldName, newName, fileSize, fileType, fileContent);
                        }
                        catch (const exception& e) {
                            cerr << red << "Error updating file: " << e.what() << reset << endl;
                        }
                        cin.ignore();
                      
                        break;

                    case 9:
                        system("cls");
                        cout << red << "..............DELETE FILE........................" << reset << endl;
                        try 
                        {
                            string nameToDelete;
                            cout << "Enter name of item to delete: ";
                            getline(cin, nameToDelete);
                            fo.delete_item(nameToDelete);
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) 
                        {
                            cerr << red << "Error deleting item: " << e.what() << endl;
                        }
                        
                    case 10:
                        system("cls");
                        cout << red << "..............SHARE FILE........................" << reset << endl;

                        try {
                            string fileName, shareWith;
                            cout << "Enter file name to share: ";
                            getline(cin, fileName);
                            cout << "Enter username to share with: ";
                            getline(cin, shareWith);
                            fo.share_file(fileName, shareWith);
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error sharing file: " << e.what() << endl;
                        }
                     
                    case 11:
                        system("cls");
                        cout << red << "..............RECYCLE BIN........................" << reset << endl;
                        try {
                            fo.display_recycle_bin();
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying recycle bin: " << e.what() << endl;
                        }
                       
                    case 12:
                        system("cls");
                        cout << red << "..............RECENT FILES........................" << reset << endl;
                        try {
                            fo.display_recent_files();
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying recent files: " << e.what() << endl;
                        }
                      
                    case 13:
                        system("cls");
                        cout << red << "..............LOG OUT........................" << reset << endl;
                        try {
                            fo.g.logout(currentUser);
                            currentUser = "";
                            isLoggedIn = false;
                            delete fo.acess;
                            fo.acess = nullptr;
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error during logout: " << e.what() << endl;
                        }
                      
                    case 14:
                        system("cls");
                        cout << red << "..............FILE VERSION........................" << reset << endl;
                        try {
                            fo.folderSystem->getDelta();
                       
                            v.display_file_version(fo.folderSystem->getDelta());
                            cin.ignore();
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying file version: " << e.what() << endl;
                        }

                    case 15:
                        system("cls");
                        cout << red << "..............CLOUD........................" << reset << endl;
                        try
                        {
                           
                            cloud.display();
                            cin.ignore();
                            Sleep(2500);
                            break;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error displaying cloud: " << e.what() << endl;
                        }
                    case 0:
                        try {
                            if (isLoggedIn)
                            {
                                fo.g.logout(currentUser);
                            }

                            cout << "Exiting program...." << endl;
                            return 0;
                        }
                        catch (const exception& e) {
                            cerr << red << "Error during program exit: " << e.what() << endl;
                            return 1;
                        }
                    default:
                        cout << red << "Invalid choice!" << reset << endl;
                    }
                }
            }
            catch (const exception& e) {
                cerr << red << "An unexpected error occurred: " << e.what() << reset << endl;
               
                break;
            }
        }
    }
    catch (const exception& e) 
    {
        cerr << red << "error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

