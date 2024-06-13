#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
using namespace std;

bool deletingFile(const string& file_Path) {

    const char* myfilepath = file_Path.c_str();

    if (remove(myfilepath) == 0) {
        cout << "My file is Deleted " << file_Path << endl;
        return true;
    }
    cout << "File was unable to delete " << file_Path << endl;
    return false;

}
//
//void AddingMyFiles(string& FilePath, string My_file_Folder) {
//
//    filesystem::path sourcePath(FilePath);
//    string My_file_Name = sourcePath.filename().string();
//
//    string My_destinationFilePath = My_file_Folder + "/" + My_file_Name;
//
//
//    ifstream my_sourceFile(FilePath, ios::binary);
//
//    if (!my_sourceFile) {
//        cerr << "cannot open source files" << My_file_Name << std::endl;
//        return;
//    }
//
//    ofstream my_destinationFile(My_destinationFilePath, ios::binary);
//
//    if (!my_destinationFile) {
//        cerr << "Error opening destination file: " << My_destinationFilePath << std::endl;
//        return;
//    }
//    FilePath = My_destinationFilePath;
//    my_destinationFile << my_sourceFile.rdbuf();
//
//    my_sourceFile.close();
//    my_destinationFile.close();
//
//    cout << "congratulations :) file is copied to folder" << std::endl;
//}

void addData_to_file() {
    cout << "Enter Filename you want to write your data in\n";
    string filename;
    cin >> filename;
    filename = filename + ".txt";
    ofstream MyFile(filename);
    cout << endl;
    cout << "Now enter the data you want to insert in the program" << endl;
    string fileData;
    cin >> fileData;
    // Write to the file
    MyFile << fileData;

    MyFile.close();
}

void FileReading() {
    cout << "Enter the file from which you want to read data" << endl;

    string FileName;
    cin >> FileName;
    FileName = FileName + ".txt";

    // Read from the text file
    ifstream inFile(FileName);
    string temp = "", fileData = "";

    while (inFile >> temp)
    {
        fileData = fileData + temp;
    }
    inFile.close();

    cout << "Data stored in file is:\n";
    cout << fileData << endl;

}
