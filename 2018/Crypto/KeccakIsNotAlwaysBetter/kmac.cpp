#include <iostream>
#include <string>
#include <string.h>

// KeccakTools header
#include "Keccak-f.h"

using namespace std;

int kmac(const string key, const string message, char output[48]) {
    uint16_t input_size;
    char *input;
    string padded_message = message;
    // Efficient Keccak-f
    UINT8 state[50] = {0}; 
    KeccakF keccakF(400);

    if (key.size() != 48) {
        cerr << "The key must be 384 bits long, not ";
        cerr << key.size()*8;
        cerr << "!" << endl;
        return 1;
    }
    input_size = 48 + message.size() + 1;
    if (input_size % 48) {
        input_size = ((input_size / 48) + 1) * 48;
    }

    input = (char *) malloc(input_size*sizeof(char));

    key.copy(input, 48);
    message.copy(input + 48, message.size());

    // Pad
    input[48 + message.size()] = '\1';
    for (int i = 48 + message.size() + 1; i < input_size; i++) {
        input[i] = '\0';
    }


    // Absorb
    for (int k = 0; k < input_size / 48; k++) {
        for (int i = k*48; i < (k+1)*48; i++) {
            state[i%48] ^= input[i];
        }
        keccakF(state);
    }

    // Squeeze
    for (int i = 0; i < 48; i++) {
        output[i] = state[i];
    }
    
    free(input);
    return 0;
}

int main(int argc, char *argv[]) {
    string message, key;
    char output[48];

    // 384 bits key
    cin >> key;
    cin >> message;

    if (kmac(key, message, output)) {
        cerr << "An error occured!" << endl;
        return 1;
    }

    // 384 bits HMAC
    for (int i = 0; i < 48; i++) {
        cout.width(2);
        cout.fill('0');
        cout << hex << (int)(UINT8)output[i];
    }

    return 0;
}
