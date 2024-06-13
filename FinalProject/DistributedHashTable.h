#pragma once

#include <iostream>
#include <cmath>
#include <random>
#include "HashFunction.h"
#include "B_Tree.h"
#include "Key_ID.h"
#include <filesystem>

using namespace std;


//The Routing Table Class which is a Doubly Linked List with Pointers to Circular Linked List node

template<typename index>
class RoutingTable {
private:
    struct Node {
        index Next_Machine;
        string Key;
        Node* next;
        Node* prev;
        explicit Node(index next_machine = nullptr, string key = "", Node* next = nullptr, Node* prev = nullptr) : Next_Machine(next_machine), Key(key), next(next), prev(prev) {}
    };
    Node* head;
    Node* tail;
    int size;
public:
    explicit RoutingTable(Node* head = nullptr, Node* tail = nullptr, int size = 0) : head(head), tail(tail), size(size) {}

    void insert(index key, string key_id, int count) {
        Node* temp = new Node(key);
        temp->Key = key_id;

        if (head == nullptr) {
            head = temp;
            tail = temp;
        }
        else {
            Node* current = head;
            Node* prev_current = nullptr;
            for (int i = 0; i < count; i++) {
                prev_current = current;
                current = current->next;
            }
            if (current == head and current != nullptr) {
                current->Next_Machine = key;
                current->Key = key_id;
            }
            else if (current == nullptr) {
                prev_current->next = temp;
                temp->prev = prev_current;
                tail = temp;
            }
            else {
                current->Key = key_id;
                current->Next_Machine = key;
            }
        }
        size++;
    }
    index search(index key) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->Key == key) {
                return temp->Key;
            }
            temp = temp->next;
        }
        return nullptr;
    }
    void remove(index key) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->Key == key) {
                if (temp == head) {
                    head = head->next;
                    head->prev = nullptr;
                    delete temp;
                    size--;
                    return;
                }
                else if (temp == tail) {
                    tail = tail->prev;
                    tail->next = nullptr;
                    delete temp;
                    size--;
                    return;
                }
                else {
                    temp->prev->next = temp->next;
                    temp->next->prev = temp->prev;
                    delete temp;
                    size--;
                    return;
                }
            }
            temp = temp->next;
        }
    }

    index Search_By_Key(Key_ID Key) {
        Node* temp = head;
        if (head == nullptr) {
            return nullptr;
        }
        cout << " -> Using Routing Table: ";
        while (temp->next != nullptr) {
            //            cout << " -> " << temp->Key;
            if (temp->Key == Key.get_key()) {
                cout << "Jumping to " << temp->Key << endl;
                return temp->Next_Machine;
            }
            else if (temp->Key > Key.get_key()) {
                if (temp->prev == nullptr) {
                    cout << "Jumping to " << temp->Key << endl;
                    return temp->Next_Machine;
                }
                else {
                    cout << "Jumping to " << temp->Key << endl;
                    return temp->prev->Next_Machine;
                }
            }
            temp = temp->next;
        }
        cout << "Jumping to " << temp->Key << endl;
        return temp->Next_Machine;
    }
    void print() {
        Node* temp = head;
        cout << "Routing Table: ";
        while (temp != nullptr) {
            cout << temp->Key << " ";
            temp = temp->next;
        }
        cout << endl;
    }

};




//The Node class which will be used in the DHT ring
template<typename B_tree>
class Node {
public:
    Key_ID Key;
    B_tree* Root;
    RoutingTable<Node*>* Routing_Table;
    Node* next;
    explicit Node(Key_ID key_id, B_tree* root = nullptr, Node* next = nullptr) : Key(key_id), Root(root), next(next) {
        Routing_Table = new RoutingTable<Node*>();
    }
    ~Node() {
        delete Routing_Table;
        delete Root;
        delete next;
    }
    void Initialize_Btree(int degree) {
        Root = new B_tree(degree);
    }
    void print() {
        cout << "Key: " << Key << endl;
        cout << "Root: " << Root << endl;
        cout << "Next: " << next << endl;
    }

    void print_routing_table() {
        Routing_Table->print();
    }

    void print_Btree_Maintain() {
        Root->Breadth_First_Traversal();
    }
};
//The DHT ring Class which is a Circular Linked List with a pointer to a B-Tree root node
template<typename B_tree>
class DHT_Ring {
private:
    Node<B_tree>* head;
    Node<B_tree>* tail;
public:
    DHT_Ring() : head(nullptr), tail(nullptr) {}
    void insert(string filename, B_tree* root = nullptr) {
        Key_ID key_id(160);
        key_id.generate_key_id(filename);
        Node<B_tree>* temp = new Node<B_tree>(key_id, root);
        if (head == nullptr) {
            head = temp;
            head->next = head;
            tail = head;
        }
        else {
            Node<B_tree>* current = head;
            while (current->next->Key < key_id and current->next != head) {
                current = current->next;
            }
            temp->next = current->next;
            current->next = temp;
            if (temp->next == head) {
                tail = temp;
            }
        }
    }
    void insert(Key_ID existing_key, int degree, B_tree* root = nullptr) {
        Node<B_tree>* temp = new Node<B_tree>(existing_key, root);
        temp->Initialize_Btree(degree);
        if (head == nullptr) {
            head = temp;
            head->next = head;
            tail = head;
        }
        else {
            Node<B_tree>* current = head;
            if (current->Key > existing_key) {
                temp->next = current;
                head = temp;
                tail->next = head;
                return;
            }
            while (current->next->Key < existing_key and current->next != head) {
                current = current->next;
            }
            temp->next = current->next;
            current->next = temp;
            if (temp->next == head) {
                tail = temp;
            }
        }
    }
    bool search(Key_ID key_id) {
        if (head == nullptr) {
            return false;
        }
        Node<B_tree>* current = head;
        while (current->next != head) {
            if (current->Key == key_id) {
                return true;
            }
            current = current->next;
        }
        if (current->Key == key_id) {
            return true;
        }
        return false;
    }

    Node<B_tree>* search2(Key_ID key_id, Node<B_tree>* Current_Machine = nullptr) {
        if (Current_Machine == nullptr) {
            Current_Machine = head;
        }
        if (Current_Machine->Key == key_id) {
            return Current_Machine;
        }
        else {
            Node<B_tree>* current = Current_Machine->Routing_Table->Search_By_Key(key_id);
            return current;
        }
    }

    void remove(Key_ID key_id) {
        Node<B_tree>* current = head;
        while (!(current->next->Key == key_id)) {
            current = current->next;
        }
        Node<B_tree>* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
    void print() {
        Node<B_tree>* current = head;
        while (current->next != head) {
            cout << current->Key << " ";
            current = current->next;
        }
        cout << current->Key << endl;
    }
    Node<B_tree>* getHead() {
        return head;
    }
};

//The DistributedHashTable class which will manage the DHT ring and the routing table
template<typename B_tree>
class DistributedHashTable {
public:
    int Identifier_Space_Bits;
    int Identifier_Space_Size;
    DHT_Ring<B_tree>* Identifier_Space;
    int Machine_Count;
    Node<B_tree>* Current_Machine;

    string Routing_Table_Key_Helper(string current_key, int power) {
        Key_ID key(Identifier_Space_Bits, current_key);
        string power_string = "";
        for (int i = Identifier_Space_Bits - 1; i >= 0; i--) {
            if (i == power) {
                power_string = power_string + "1";
            }
            else {
                power_string = power_string + "0";
            }
        }
        string result = key + power_string;
        return result;
    }

    void Routing_Table_Helper(DHT_Ring<B_tree>* Head) {
        Node<B_tree>* current = Head->getHead();
        do {
            //creating routing table for each node
            for (int i = 0; i < Identifier_Space_Bits; i++) {
                string gen_key = Routing_Table_Key_Helper(current->Key.get_key(), i);
                Key_ID key(Identifier_Space_Bits, gen_key);
                Node<B_tree>* temp = Head->getHead();
                while (temp->Key < key and temp->next != Head->getHead()) {
                    temp = temp->next;
                }
                current->Routing_Table->insert(temp, temp->Key.get_key(), i);
            }
            //            current->Routing_Table->print();
            current = current->next;
        } while (current != Head->getHead());
    }



public:
    DistributedHashTable(int identifier_space_bits) : Identifier_Space_Bits(identifier_space_bits) {
        Identifier_Space = new DHT_Ring<B_tree>();
        Identifier_Space_Size = pow(2, Identifier_Space_Bits + 1) - 1;
        Machine_Count = 0;
        Current_Machine = nullptr;
    }

    ~DistributedHashTable() {
        delete Identifier_Space;
    }

    void Add_Random_Machine() {
        if (Machine_Count == Identifier_Space_Size) {
            cout << "Identifier Space is Full" << endl;
            return;
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        string key = "";
        for (int i = 0; i < Identifier_Space_Bits; i++) {
            key = key + std::to_string(dis(gen));
        }
        Key_ID key_id(Identifier_Space_Bits, key);
        //search if duplicate key exists
        while (Identifier_Space->search(key_id)) {
            key = "";
            for (int i = 0; i < Identifier_Space_Bits; i++) {
                key = key + std::to_string(dis(gen));
            }
            key_id = Key_ID(Identifier_Space_Bits, key);
        }
        cout << "Key: " << key_id << endl;
        /*Identifier_Space->insert(key_id, Identifier_Space_Bits);
        std::filesystem::path dirPath(key_id.get_key());
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directory(dirPath);
        }*/
        Machine_Count++;
        Routing_Table_Helper(Identifier_Space);
    }

    void Add_Specific_Machine(string key_id) {
        //Check if key_id is valid
        if (key_id.length() != Identifier_Space_Bits) {
            cout << "Invalid Key" << endl;
            return;
        }
        Key_ID key(Identifier_Space_Bits, key_id);
        //search if duplicate key exists
        if (Identifier_Space->search(key)) {
            cout << "Duplicate Key" << endl;
            return;
        }
        /*Identifier_Space->insert(key_id, Identifier_Space_Bits);
        std::filesystem::path dirPath(key_id);
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directory(dirPath);
        }*/
        Machine_Count++;
        Routing_Table_Helper(Identifier_Space);
    }

    Node<B_tree>* Search_Machine(Key_ID key, Node<B_tree>* Current_Machine = nullptr) {
        if (Current_Machine == nullptr) {
            Current_Machine = Identifier_Space->getHead();
        }
        if (key == Current_Machine->Key) {
            return Current_Machine;
        }
        else {
            Node<B_tree>* current = Current_Machine->Routing_Table->Search_By_Key(key);
            return current;
        }
    }

    string Search_File(Key_ID key, string file_name, Node<B_tree> Current_Machine = nullptr) {
        if (Current_Machine == nullptr) {
            Current_Machine = this->Current_Machine;
        }
        if (Current_Machine.Key == key) {
            //Search in B-Tree
            return Current_Machine.Root->search(key, file_name);
        }
        else {
            Node<B_tree>* current = Current_Machine.Routing_Table->Search_By_Key(key);
            return current->Root->search(key, file_name);
        }
    }

    void print() {
        Identifier_Space->print();
    }

    void print_routing_tables() {
        Node<B_tree>* current = Identifier_Space->getHead();
        do {
            cout << "Routing Table for " << current->Key << endl;
            current->Routing_Table->print();
            current = current->next;
        } while (current != Identifier_Space->getHead());
    }


    void Routing_Table() {
        Routing_Table_Helper(Identifier_Space);
    }


    void Add_File(Key_ID key, string file_name, string file_path, Node<B_tree>* Current_Machine = nullptr) {
        if (Current_Machine == nullptr) {
            Current_Machine = this->Current_Machine;
            if (Current_Machine == nullptr) {
                Current_Machine = Identifier_Space->getHead();

            }
        }
        cout << "Adding File: " << file_name << " | ";
        cout << "Machine Path: " << Current_Machine->Key;
        if (Current_Machine->Key == key) {
            //Add to B-Tree
            Current_Machine->Root->insert(key.get_key(), file_name, file_path);
        }
        else {
            Node<B_tree>* current = Current_Machine->Routing_Table->Search_By_Key(key);
            current->Root->insert(key.get_key(), file_name, file_path);
        }
        cout << " | File Added" << endl;
    }

    void print_all_Identifier_Space() {
        Node<B_tree>* current = Identifier_Space->getHead();
        do {
            cout << "Identifier Space for " << current->Key << endl;
            current->Root->Breadth_First_Traversal();
            current = current->next;
            cout << "---------------------------------";
            cout << endl << endl;

        } while (current != Identifier_Space->getHead());
    }

    void User_Interface() {
        int option;
        int count = 0;

        //User Interface Menu 1
        stringstream Menu_1, Menu_1a, Menu_2;
        Menu_1 << "Welcome to the Distributed Hash Table" << endl
            << "Please select an option" << endl
            << "1. Specify Number of Machines to Add" << endl
            << "2. Add Random Machine" << endl
            << "3. Add Specific Machine" << endl
            << "4. Remove Machine" << endl
            << "5. Print Identifier Space" << endl
            << "6. Select a Specific Machine" << endl
            << "7. Exit" << endl;
        //User Interface Menu 1a
        Menu_1a << "Specify Number of Machines to Add : ";
        //User Interface Menu 2 Specify Number of Machines to Add
        Menu_2 << "Specifing Number of Machines to Add" << endl
            << "1. Add Random Machine" << endl
            << "2. Add Specific Machine" << endl
            << "3. Exit" << endl;

        while (true) {
            cout << Menu_1.str();
            cin >> option;
            if (option == 1) {
                cout << Menu_1a.str();
                int number_of_machines;
                cin >> number_of_machines;
                for (int i = 0; i < number_of_machines; i++) {
                    Add_Random_Machine();
                }
            }
            else if (option == 2) {
                Add_Random_Machine();
            }
            else if (option == 3) {
                string key_id;
                cout << "Enter Key ID: ";
                cin >> key_id;
                Add_Specific_Machine(key_id);
            }
            else if (option == 4) {
                string key_id;
                cout << "Enter Key ID: ";
                cin >> key_id;
                Remove_Machine(key_id);
            }
            else if (option == 5) {
                print();
            }
            else if (option == 6) {
                string key_id;
                cout << "Enter Key ID: ";
                cin >> key_id;
                Key_ID key(Identifier_Space_Bits, key_id);
                if (Identifier_Space->search(key)) {
                    Node<B_tree>* current = Identifier_Space->getHead();
                    while (current->Key <= key and current->next != Identifier_Space->getHead()) {
                        current = current->next;
                    }
                    Current_Machine = current;
                    cout << "Machine Selected" << endl;
                    break;
                }
                else {
                    cout << "Machine does not exist" << endl;
                }
            }
            else if (option == 7) {
                cout << "Exiting" << endl;
                return;
            }
            else {
                cout << "Invalid Option" << endl;
            }
        }
    }

    void select_current_machine(string key_id) {
        Key_ID key(Identifier_Space_Bits, key_id);
        if (Identifier_Space->search(key)) {
            Node<B_tree>* current = Identifier_Space->getHead();
            while (current->Key <= key and current->next != Identifier_Space->getHead()) {
                current = current->next;
            }
            Current_Machine = current;
            cout << "Machine Selected" << endl;
        }
        else {
            cout << "Machine does not exist" << endl;
        }
    }

    //Removing a Machine from the DHT Ring But saving and reallocating the Btree maintained by the machine
    void Remove_Machine(string key_id) {
        //Check if key_id is valid
        if (key_id.length() != Identifier_Space_Bits) {
            cout << "Invalid Key" << endl;
            return;
        }
        Key_ID key(Identifier_Space_Bits, key_id);
        //search if duplicate key exists
        if (!Identifier_Space->search(key)) {
            cout << "Key does not exist" << endl;
            return;
        }

        Queue<BTreeNode<string, string>*>* temp_store = new Queue<BTreeNode<string, string>*>();
        Node<B_tree>* current = Identifier_Space->search2(key);
        current->Root->Storing_Btree_Queue(temp_store);
        Identifier_Space->remove(key);
        Machine_Count--;
        Routing_Table_Helper(Identifier_Space);
        //        Reallocating the Btree
        while (!temp_store->is_empty()) {
            BTreeNode<string, string>* temp = temp_store->dequeue();
            for (int i = 0; i < Identifier_Space_Bits; i++) {
                if (temp->keys[i] != "") {
                    Add_File(key, temp->keys[i], temp->FilePath[i], nullptr);
                }
            }
        }
        /*std::filesystem::path dirPath(key_id);
        if (std::filesystem::exists(dirPath)) {*/
            //            Queue<string>* file_names = new Queue<string>();
            //            for (const auto & entry : std::filesystem::directory_iterator(dirPath)) {
            //                file_names->enqueue(entry.path().string());
            //            }
            //            std::filesystem::remove_all(dirPath);
        //}
    }
};


