#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <filesystem>
//#include "Key_ID.h"
//#include "DistributedHashTable.h"

using namespace std;



//Queue Class to Handle when a machine is removed from the DHT ring
template<typename fid>
class Queue {
public:
    struct Node {
        fid data;
        Node* next;
        explicit Node(fid d, Node* n = nullptr) { data = d; next = n; }
    };
    Node* front = nullptr;
    Node* rear = nullptr;
    int Size;
public:
    Queue() {
        front = nullptr;
        rear = nullptr;
        Size = 0;
    }
    explicit Queue(Queue<fid>* to_copy) {
        Size = 0;
        if (to_copy == nullptr) {
            front = nullptr;
            rear = nullptr;
        }
        else {
            Node* copy = to_copy->front;
            while (copy != nullptr) {
                Node* temp = new Node(copy->data);
                if (front == nullptr) {
                    front = temp;
                    rear = temp;
                }
                else {
                    rear->next = temp;
                    rear = temp;
                }
                Size++;
                copy = copy->next;
            }
        }

    }
    void enqueue(fid data) {
        Node* new_node = new Node(data);
        if (front == nullptr) {
            front = new_node;
            rear = new_node;
        }
        else {
            rear->next = new_node;
            rear = new_node;
        }
        Size++;
    }
    fid dequeue() {
        fid ret = nullptr;
        if (!is_empty()) {
            Node* temp = front;
            front = front->next;
            ret = temp->data;
            delete temp;
            Size--;
        }
        else {
            front = nullptr;
            rear = nullptr;
        }
        return ret;
    }
    fid peek() {
        return front->data;
    }
    bool is_empty() {
        return (front == nullptr);
    }
    int size() {
        return Size;
    }
    void clear() {
        Node* temp = front;
        while (temp) {
            front = front->next;
            delete temp;
            temp = front;
            Size--;
        }
        rear = nullptr;
    }
    void print() {
        Node* temp = front;
        while (temp) {
            temp = temp->next;
        }
    }

};

template <class Key, class Pathname>
class BTreeNode {
public:
    struct File_Entries {
        Pathname File_Path;
        Key duplicatekey;
        File_Entries* next;

        File_Entries() {
            File_Path = "";
            duplicatekey = "";
            next = nullptr;
        }
        File_Entries(Pathname value, Key k) {
            duplicatekey = k;
            File_Path = value;
            next = nullptr;
        }
        void insert(Pathname value, Key k) {
            if (next == nullptr) {
                next = new File_Entries(value, k);
            }
            else {
                next->insert(value, k);
            }
        }
    };

    File_Entries* DupPath;
    Key* keys;
    Pathname* FilePath;
    int total_Order;
    BTreeNode** Childptr;
    int currentkeys;
    bool Is_Leaf;

    BTreeNode(int order, bool Is_Leaf1) {
        this->total_Order = order;
        Is_Leaf = Is_Leaf1;
        keys = new Key[this->total_Order - 1];
        FilePath = new Pathname[this->total_Order - 1];
        Childptr = new BTreeNode * [this->total_Order];
        this->currentkeys = 0;
        DupPath = nullptr;
    }

    // A function to traverse all nodes in a subtree BT_rooted with this node
//    void traverse() {
//        int i;
//        for (i = 0; i < this->currentkeys; i++) {
//            if (Is_Leaf == false)
//                Childptr[i]->traverse();
//            cout << " " << keys[i];
//        }
//
//        if (Is_Leaf == false)
//            Childptr[i]->traverse();
//    }


    BTreeNode* search(Key search_key)
    {
        int j = 0;
        while (j < this->currentkeys && search_key > keys[j]) {
            j++;
            if (keys[j] == search_key)
                return this;
        }
        if (Is_Leaf == true)
            return NULL;

        return Childptr[j]->search(search_key);
    }

    // A utility function to insert a new key in the subtree BT_rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void InsertNotFullNode(Key current_key, Pathname value, string path) {
        int i = this->currentkeys - 1;

        if (Is_Leaf) {

            for (int j = 0; j < currentkeys; j++) {
                if (keys[j] == current_key) {
                    //                    AddingMyFiles(value, path);
                    if (!DupPath) {
                        DupPath = new File_Entries(value, current_key);
                        //                        DupPath->insert(value);
                    }
                    else {
                        DupPath->insert(value, current_key);
                    }
                    //                    File_Path[i].insert(value);
                    return;
                }
            }


            while (i >= 0 && keys[i] > current_key) {


                FilePath[i + 1] = FilePath[i];
                keys[i + 1] = keys[i];
                i--;
            }
            i++;
            keys[i] = current_key;
            FilePath[i] = value;
            this->currentkeys += 1;
            //            AddingMyFiles(FilePath[i], path);
        }
        else {
            while (i >= 0 && keys[i] > current_key) {
                i--;
            }
            i++;

            if (Childptr[i]->currentkeys == this->total_Order - 1) {
                DivideChild(i, Childptr[i]);
                if (keys[i] < current_key) {
                    i++;
                }
            }
            Childptr[i]->InsertNotFullNode(current_key, value, path);
        }
    }

    // A utility function to split the child y of this node. i is the index
    // of y in child array C[]. The Child y must be full when this
    // function is called
    void DivideChild(int index, BTreeNode* divNode) {
        BTreeNode* newChild = new BTreeNode(divNode->total_Order, divNode->Is_Leaf);
        bool even = false;
        if (total_Order % 2 == 0) {
            newChild->currentkeys = (this->total_Order) / 2 - 1;
            even = true;
        }
        else {
            newChild->currentkeys = (this->total_Order) / 2;
        }

        int ind = newChild->currentkeys;
        int count = ind;
        if (even == true) {
            count++;
            newChild->currentkeys++;
        }
        for (int i = 0; i < count; i++) {
            newChild->keys[i] = divNode->keys[i + ind];
            cout << newChild->keys[i];
            newChild->FilePath[i] = divNode->FilePath[i + ind];
        }
        if (divNode->DupPath) {
            if (newChild->keys[0] < divNode->DupPath->duplicatekey) {
                newChild->DupPath = divNode->DupPath;
                divNode->DupPath = nullptr;
            }
        }

        if (divNode->Is_Leaf == false) {
            for (int j = 0; j < count; j++)
                newChild->Childptr[j] = divNode->Childptr[j + ind];
        }

        divNode->currentkeys = ind - 1;

        for (int j = this->currentkeys; j >= index + 1; j--)
            Childptr[j + 1] = Childptr[j];

        Childptr[index + 1] = newChild;

        for (int j = this->currentkeys - 1; j >= index; j--) {
            keys[j + 1] = keys[j];
            FilePath[j + 1] = FilePath[j];
        }

        keys[index] = divNode->keys[ind - 1];
        FilePath[index] = divNode->FilePath[ind - 1];


        this->currentkeys += 1;
    }

    // A wrapper function to remove the key k in the subtree BT_rooted with
    // this node.
    bool duplicatehelper(Key k, bool& nochange) {
        int i;
        for (i = 0; i < this->currentkeys; i++) {
            if (Is_Leaf == false)
                Childptr[i]->duplicatehelper(k, nochange);
            if (DupPath == nullptr && nochange == false) {
                return false;
            }
            //cout << keys[i];
            if (k == keys[i]) {


                cout << "Duplicate values found in this array" << endl << endl;
                cout << FilePath[i] << " ";
                File_Entries* temp = DupPath;
                bool dup = false;
                while (temp) {
                    if (DupPath->duplicatekey == k) {
                        cout << DupPath->File_Path << " ";
                        dup = true;
                    }
                    temp = temp->next;
                }
                if (dup == false) {
                    return false;
                }
                cout << endl << endl;
                string path;
                cout << "Write the path you want to remove" << endl;
                cin >> path;
                if (path == FilePath[i]) {
                    temp = DupPath;
                    if (temp->duplicatekey == k && temp->next == nullptr) {
                        FilePath[i] = temp->File_Path;
                        DupPath = DupPath->next;
                        nochange = true;
                        //                        deletingFile(DupPath->File_Path);
                        return true;
                    }
                    else {
                        while (temp->next != nullptr && temp->next->duplicatekey != k) {
                            temp = temp->next;
                        }
                        if (temp->next) {
                            //                            deletingFile(FilePath[i]);
                            FilePath[i] = temp->next->File_Path;
                            temp->next = temp->next->next;

                            return true;
                        }
                    }
                    cout << "InvalidPath so cannot delete" << endl;
                    return true;

                }
                else {
                    temp = DupPath;
                    if (temp->File_Path == path) {
                        //                        deletingFile(DupPath->File_Path);
                        DupPath = DupPath->next;
                    }
                    else {
                        while (!temp->next) {
                            if (temp->next->File_Path == path) {
                                //                                deletingFile(temp->next->File_Path);
                                temp->next = temp->next->next;
                                return true;
                            }
                            temp = temp->next;
                        }
                        cout << "InvalidPath so cannot delete" << endl;
                    }
                }


                return true;
            }
        }
        if (Is_Leaf == false) {
            Childptr[i]->duplicatehelper(k, nochange);
        }
    }


    void remove(Key removing_key) {

        bool DupP = false, nochange = false;
        DupP = duplicatehelper(removing_key, nochange);
        if (DupP) {
            return;
        }



        int index = search_key(removing_key);

        if (keys[index] == removing_key && index < this->currentkeys) {
            if (!Is_Leaf) {
                removeFromNonIs_Leaf(index);

            }
            else {
                Remove_Leaf(index);
            }
        }
        else {
            if (Is_Leaf) {
                cout << removing_key << " Key does not exist in the tree" << endl;
                return;
            }

            bool keyPresent = false;
            if (index == this->currentkeys) {
                keyPresent = true;
            }

            if (Childptr[index]->currentkeys < this->total_Order / 2) {
                RemoveDecision(index);
            }

            if (keyPresent && index > this->currentkeys) {
                Childptr[index - 1]->remove(removing_key);
            }
            else
                Childptr[index]->remove(removing_key);
        }
        return;
    }
    // A function that returns the index of the first key that is greater
    // or equal to k
    int search_key(Key s_key) {
        int index = 0;
        while (index < this->currentkeys && keys[index] < s_key) {
            index++;
        }
        return index;
    }


    // A function to remove the key present in idx-th position in
    // this node which is a Is_Leaf
    void Remove_Leaf(int index) {
        //        deletingFile(FilePath[index]);
        for (int j = index + 1; j < this->currentkeys; j++) {
            FilePath[j - 1] = FilePath[j];
            keys[j - 1] = keys[j];
        }
        this->currentkeys--;

        return;
    }

    // A function to remove the key present in idx-th position in
    // this node which is a non-Is_Leaf node
    void removeFromNonIs_Leaf(int idx) {
        Key k = keys[idx];

        if (Childptr[idx]->currentkeys >= this->total_Order) {
            Key predecessor = Finding_sucessor(idx);
            keys[idx] = predecessor;
            Childptr[idx]->remove(predecessor);
        }
        else if (Childptr[idx + 1]->currentkeys >= this->total_Order) {
            Key successor = finding_predecessor(idx);
            keys[idx] = successor;
            Childptr[idx + 1]->remove(successor);
        }
        else {
            Marge(idx);
            Childptr[idx]->remove(k);
        }
        return;

    }

    // A function to get the predecessor of the key- where the key
    // is present in the idx-th position in the node

    Key Finding_sucessor(int index) {
        BTreeNode* tempptr = Childptr[index];
        while (!tempptr->Is_Leaf) {
            tempptr = tempptr->Childptr[tempptr->currentkeys];
        }
        return tempptr->keys[tempptr->currentkeys - 1];
    }

    // A function to get the successor of the key- where the key
    // is present in the idx-th position in the node
    Key finding_predecessor(int index) {
        BTreeNode* tempptr = Childptr[index + 1];
        while (!tempptr->Is_Leaf) {
            tempptr = tempptr->Childptr[0];
        }
        return tempptr->keys[0];
    }

    // A function to RemoveDecision up the child node present in the idx-th
    // position in the C[] array if that child has fewer than t-1 keys
    void RemoveDecision(int index) {

        if (index != this->currentkeys && Childptr[index + 1]->currentkeys >= (this->total_Order) / 2)
            BorrowFromNextSibling(index);
        else if (index != 0 && Childptr[index - 1]->currentkeys >= (this->total_Order) / 2)
            BorrowFromPreviousSibling(index);
        else {
            if (index != this->currentkeys)
                Marge(index);
            else
                Marge(index - 1);
        }
        return;
    }

    // A function to borrow a key from the C[idx-1]-th node and place
    // it in C[idx]th node
    void BorrowFromPreviousSibling(int index) {
        BTreeNode* CurrentChild = Childptr[index];
        BTreeNode* SiblingNode = Childptr[index - 1];

        for (int j = CurrentChild->currentkeys - 1; j >= 0; j++) {
            CurrentChild->keys[j + 1] = CurrentChild->keys[j];
            CurrentChild->FilePath[j + 1] = CurrentChild->FilePath[j];
        }

        if (!CurrentChild->Is_Leaf) {
            for (int i = CurrentChild->currentkeys; i >= 0; --i)
                CurrentChild->Childptr[i + 1] = CurrentChild->Childptr[i];
        }

        CurrentChild->keys[0] = keys[index - 1];
        CurrentChild->FilePath[0] = FilePath[index - 1];

        if (!CurrentChild->Is_Leaf)
            CurrentChild->Childptr[0] = SiblingNode->Childptr[SiblingNode->currentkeys];

        keys[index - 1] = SiblingNode->keys[SiblingNode->currentkeys - 1];
        FilePath[index - 1] = SiblingNode->FilePath[SiblingNode->currentkeys - 1];

        CurrentChild->currentkeys += 1;
        SiblingNode->currentkeys -= 1;

        return;
    }

    // A function to borrow a key from the C[idx+1]-th node and place it
    // in C[idx]th node
    void BorrowFromNextSibling(int idx) {
        BTreeNode* CurrentChild = Childptr[idx];
        BTreeNode* SiblingNode = Childptr[idx + 1];

        CurrentChild->keys[CurrentChild->currentkeys] = keys[idx];
        CurrentChild->FilePath[CurrentChild->currentkeys] = FilePath[idx];

        if (!CurrentChild->Is_Leaf)
            CurrentChild->Childptr[CurrentChild->currentkeys + 1] = SiblingNode->Childptr[0];

        keys[idx] = SiblingNode->keys[0];
        FilePath[idx] = SiblingNode->FilePath[0];

        for (int i = 1; i < SiblingNode->currentkeys; ++i) {
            SiblingNode->keys[i - 1] = SiblingNode->keys[i];
            SiblingNode->FilePath[i - 1] = SiblingNode->FilePath[i];
        }

        if (!SiblingNode->Is_Leaf) {
            for (int i = 1; i <= SiblingNode->currentkeys; ++i)
                SiblingNode->Childptr[i - 1] = SiblingNode->Childptr[i];
        }

        CurrentChild->currentkeys += 1;
        SiblingNode->currentkeys -= 1;

        return;
    }

    // A function to Marge idx-th child of the node with (idx+1)th child of
    // the node
    void Marge(int index) {
        BTreeNode* CurrentChild = Childptr[index];
        BTreeNode* SiblingNode = Childptr[index + 1];

        CurrentChild->keys[this->total_Order - 1] = keys[index];

        for (int i = 0; i < SiblingNode->currentkeys; i++) {
            CurrentChild->keys[i + this->total_Order] = SiblingNode->keys[i];
            CurrentChild->FilePath[i + this->total_Order] = SiblingNode->FilePath[i];
        }

        if (!CurrentChild->Is_Leaf) {
            for (int i = 0; i <= SiblingNode->currentkeys; ++i)
                CurrentChild->Childptr[i + this->total_Order] = SiblingNode->Childptr[i];
        }
        //removing middle keys
        for (int i = index + 1; i < this->currentkeys; ++i)
            keys[i - 1] = keys[i];

        for (int i = index + 2; i <= this->currentkeys; ++i)
            Childptr[i - 1] = Childptr[i];

        CurrentChild->currentkeys += SiblingNode->currentkeys + 1;
        this->currentkeys--;

        delete(SiblingNode);
        return;
    }

    // Make BTree friend of this so that we can access private members of
    // this class in BTree functions
    // friend class BTree;
};

template <class KeyID, class Pathname>
class BTree {
public:
    // list<int>* duplicate_keys;
    BTreeNode<KeyID, Pathname>* BT_root; // Pointer to the BT_root node
    int order; // Maximum degree
    int maxorder;

    void Breadth_First_Traversal_Helper(BTreeNode<KeyID, Pathname>* root) {
        if (root == nullptr)
            return;
        Queue<BTreeNode<KeyID, Pathname>*> q;
        q.enqueue(root);
        while (!q.is_empty()) {
            int nodesAtCurrentLevel = q.size();
            for (int i = 0; i < nodesAtCurrentLevel; i++) {
                BTreeNode<KeyID, Pathname>* current = q.peek();
                q.dequeue();
                for (int j = 0; j < current->currentkeys; j++) {
                    std::cout << current->keys[j] << " ";
                }
                for (int j = 0; j <= current->currentkeys; j++) {
                    if (!current->Is_Leaf and current->Childptr[j] != nullptr)
                        q.enqueue(current->Childptr[j]);
                }
                std::cout << "\t" << "\t";
            }
            std::cout << "\n";
        }
    }
public:

    BTree(int max) {
        BT_root = nullptr;
        order = max;
        //if (max % 2 == 0) {
        //    order = max / 2;
        //    //order -= 1;
        //}
        //else {
        //    order = max / 2;
        //    //order += 1;
        //}
    }

    void traverse() {
        if (BT_root)
            BT_root->traverse();
    }

    void insert(KeyID addingKey, Pathname Path_name, string path) {
        if (BT_root == nullptr) {
            BT_root = new BTreeNode<KeyID, Pathname>(order, true);
            BT_root->keys[0] = addingKey;
            BT_root->FilePath[0] = Path_name;
            BT_root->currentkeys = 1;
            //AddingMyFiles(BT_root->FilePath[0], path);
        }
        else {

            //duplicate Path_name is beeing inserted
            if (BT_root->search(addingKey) != NULL) {
                return;
            }

            if (BT_root->currentkeys == order - 1) {
                BTreeNode<KeyID, Pathname>* newNode = new BTreeNode<KeyID, Pathname>(order, false);
                newNode->Childptr[0] = BT_root;

                newNode->DivideChild(0, BT_root);

                int i = 0;
                if (newNode->keys[0] < addingKey)
                    i++;
                newNode->Childptr[i]->InsertNotFullNode(addingKey, Path_name, path);

                BT_root = newNode;
            }
            else {
                BT_root->InsertNotFullNode(addingKey, Path_name, path);
            }
        }
    }

    void remove(KeyID deletingKey) {

        if (BT_root) {
            BT_root->remove(deletingKey);

            if (BT_root->currentkeys == 0) {
                if (!BT_root->Is_Leaf) {
                    BTreeNode< KeyID, Pathname>* tempNode = BT_root;
                    BT_root = BT_root->Childptr[0];
                    delete tempNode;
                }
                else {
                    BT_root = nullptr;
                }

            }
        }
        else {
            cout << "The tree is empty\n";
            return;
        }

    }

    void Storing_Btree_Queue(Queue<BTreeNode<KeyID, Pathname>*>* temp_store) {
        temp_store = new Queue<BTreeNode<KeyID, Pathname>*>();
        if (BT_root == nullptr) {
            return;
        }
        Queue<BTreeNode<KeyID, Pathname>*> q;
        q.enqueue(BT_root);
        while (!q.is_empty()) {
            int nodesAtCurrentLevel = q.size();
            for (int i = 0; i < nodesAtCurrentLevel; i++) {
                BTreeNode<KeyID, Pathname>* current = q.peek();
                q.dequeue();
                temp_store->enqueue(current);
                for (int j = 0; j <= current->currentkeys; j++) {
                    if (!current->Is_Leaf and current->Childptr[j] != nullptr)
                        q.enqueue(current->Childptr[j]);
                }
            }
        }
    }

    void Breadth_First_Traversal() {
        cout << "Breadth First Traversal: " << endl;
        Breadth_First_Traversal_Helper(BT_root);
    }
};


