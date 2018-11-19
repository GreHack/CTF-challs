#include <iostream>
#include <string>
#include <string.h>

// KeccakTools (https://github.com/KeccakTeam/KeccakTools) objects are needed.
#include "Keccak-f.h"

using namespace std;

int main(int argc, char *argv[]) {
    char kmac[49] = "\xdc\x6b\x4b\xd3\x52\x42\xda\x54\x3e\xca\x12\x65\x42\x05\xc1\x1b\xbc\xce\x76\xe1\x49\x62\x3b\x38\x7f\xcd\x94\x12\xd6\x3d\x00\x2e\x7a\x8d\xc3\xd3\xa2\xd7\x7d\xbd\x73\x66\x68\xf9\xb3\x14\xd0\x20";
    char message[26] = "username=Nics,admin=false";

    UINT8 state[50] = {0}; 
    KeccakF keccakF(400);

    // BF loop
    for (uint32_t i = 0; i < (1 << 16); i++) {
        // Load the guessed final state
        for (int k = 0; k < 48; k++) {
            state[k] = kmac[k];
        }
        state[48] = i >> 8;
        state[49] = i & 0xFF;

        keccakF.inverse(state);
        for (int k = 0; k < 25; k++)
            state[k] ^= message[k];

        // Don't forget the padding
        state[25] ^= 1;

        keccakF.inverse(state);
        // Valid first intermediate state must end with two zero bytes
        if (!state[49] && !state[48]) {// && strstr((char *)state, "GH18{")) { // Restriction to flag format (optional)
            cout << i;
            cout << (char *)state << endl;
        }


    }
}
