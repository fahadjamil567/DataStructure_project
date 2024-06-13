// FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DistributedHashTable.h"
#include "Key_ID.h"
using namespace std;


int main() {
    int Identifier_Space_Bits;
    cout << "Welcome to the Distributed Hash Table" << endl;
    cout << "Please Specify Identifier Space Size in Bits : ";
    cin >> Identifier_Space_Bits;
    DistributedHashTable<BTree<string, string>>* DHT = new DistributedHashTable<BTree<string, string>>(Identifier_Space_Bits);
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    DHT->Add_Random_Machine();
    Key_ID key(Identifier_Space_Bits, "0010");
    Key_ID key1(Identifier_Space_Bits, "0100");
    Key_ID key2(Identifier_Space_Bits, "0110");
    Key_ID key3(Identifier_Space_Bits, "1000");
    Key_ID key4(Identifier_Space_Bits, "1010");
    Key_ID key5(Identifier_Space_Bits, "1100");
    Key_ID key6(Identifier_Space_Bits, "1110");
    //    key1.set_key("010");
    DHT->Add_File(key, "file1", "file1.txt");
    DHT->Add_File(key1, "file2", "file2.txt");
    DHT->Add_File(key2, "file3", "file3.txt");
    DHT->Add_File(key3, "file4", "file4.txt");
    DHT->select_current_machine("0010");
    DHT->Add_File(key4, "file5", "file5.txt");
    DHT->Add_File(key5, "file6", "file6.txt");
    DHT->Add_File(key6, "file7", "file7.txt");
    DHT->Add_File(key, "file8", "file8.txt");
    DHT->Add_File(key1, "file9", "file9.txt");
    //DHT->Remove_Machine("0010");
    DHT->print_all_Identifier_Space();
    //    DHT->Search_Machine(key)->Root->Breadth_First_Traversal();
    //    DHT->Search_Machine(key1)->Root->Breadth_First_Traversal();

    //    DHT->User_Interface();
    //    delete DHT;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
