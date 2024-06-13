#pragma once
#include <iostream>
#include <cmath>
#include "HashFunction.h"

using namespace std;

//Key ID generation class
class Key_ID {
private:
    int Identifier_Space_Bits;
    string Key;
public:
    explicit Key_ID(int identifier_space_bits, string existing_key = "") : Identifier_Space_Bits(identifier_space_bits), Key(existing_key) {}

    Key_ID(const Key_ID& key_id) {
        Identifier_Space_Bits = key_id.Identifier_Space_Bits;
        Key = key_id.Key;
    }

    string hextobin(char a) {
        if (a == '0') {
            return "0000";
        }
        else if (a == '1') {
            return "0001";
        }
        else if (a == '2') {
            return "0010";
        }
        else if (a == '3') {
            return "0011";
        }
        else if (a == '4') {
            return "0100";
        }
        else if (a == '5') {
            return "0101";
        }
        else if (a == '6') {
            return "0110";
        }
        else if (a == '7') {
            return "0111";
        }
        else if (a == '8') {
            return "1000";
        }
        else if (a == '9') {
            return "1001";
        }
        else if (a == 'a') {
            return "1010";
        }
        else if (a == 'b') {
            return "1011";
        }
        else if (a == 'c') {
            return "1100";
        }
        else if (a == 'd') {
            return "1101";
        }
        else if (a == 'e') {
            return "1110";
        }
        else if (a == 'f') {
            return "1111";
        }
        else {
            cout << "Error" << endl;
            return "Error";
        }
    }

    string generate_key_id(string data) {
        string hash = picosha2::hash256_hex_string(data);
        //        cout << hash << endl;
        string binary = "";
        for (int i = 0; i < 64; i++) {
            binary = binary + hextobin(hash[i]);
        }
        std::string rightmost;
        if (Identifier_Space_Bits > 0 && Identifier_Space_Bits <= 64 * 4) {
            rightmost = binary.substr(64 * 4 - Identifier_Space_Bits);
            //            std::cout << "Rightmost " << Identifier_Space_Bits << " bits: " << rightmost << std::endl;
            //            cout << rightmost.length() << endl;
        }
        Key = rightmost;
        return Key;
    }

    string get_key() {
        return Key;
    }

    bool operator < (const Key_ID& key_id) const {
        for (int i = 0; i < Identifier_Space_Bits; i++) {
            if (Key[i] < key_id.Key[i]) {
                return true;
            }
            else if (Key[i] > key_id.Key[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator > (const Key_ID& key_id) const {
        for (int i = 0; i < Identifier_Space_Bits; i++) {
            if (Key[i] > key_id.Key[i]) {
                return true;
            }
            else if (Key[i] < key_id.Key[i]) {
                return false;
            }
        }
        return false;
    }
    bool operator == (const Key_ID& key_id) const {
        for (int i = 0; i < this->Identifier_Space_Bits; i++) {
            if (Key[i] != key_id.Key[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator >= (const Key_ID& key_id) const {
        for (int i = 0; i < Identifier_Space_Bits; i++) {
            if (Key[i] > key_id.Key[i]) {
                return true;
            }
            else if (Key[i] < key_id.Key[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator <= (const Key_ID& key_id) const {
        for (int i = 0; i < Identifier_Space_Bits; i++) {
            if (Key[i] < key_id.Key[i]) {
                return true;
            }
            else if (Key[i] > key_id.Key[i]) {
                return false;
            }
        }
        return true;
    }
    string operator + (const string& key_id) const {
        //        cout << "Identifier Space Bits: " << Identifier_Space_Bits << endl;
        //        cout << "Key:    " << Key << endl;
        //        cout << "Key ID: " << key_id << endl;
        string result = "";
        for (int i = Identifier_Space_Bits - 1; i >= 0; i--) {
            result = result + '0';
        }
        char carry = '0';
        for (int i = Identifier_Space_Bits - 1; i >= 0; i--) {
            if (Key[i] == '0' && key_id[i] == '0') {
                result[i] = carry;
                carry = '0';
            }
            else if (Key[i] == '0' && key_id[i] == '1') {
                if (carry == '0') {
                    result[i] = '1';
                }
                else {
                    result[i] = '0';
                    carry = '1';
                }
            }
            else if (Key[i] == '1' && key_id[i] == '0') {
                if (carry == '0') {
                    result[i] = '1';
                }
                else {
                    result[i] = '0';
                    carry = '1';
                }
            }
            else if (Key[i] == '1' && key_id[i] == '1') {
                result[i] = carry;
                carry = '1';
            }
        }
        return result;
    }
    friend ostream& operator << (ostream& out, const Key_ID& key_id) {
        out << key_id.Key;
        return out;
    }
    void set_key(string key) {
        Key = key;
    }
};



