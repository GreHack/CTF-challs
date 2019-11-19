#include <iostream>
#include <cstdio>
#include <random>

#include <sys/ioctl.h>
#include <termios.h>

#include <sys/ptrace.h>

#include "termcolor.hpp"

using namespace std;

#pragma clang optimize off

/*
 * Some ideas were taken from http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html
 */

//#define DEBUG
//#define DEBUG_REVERSE_ENC
//#define DEBUG_DEC
//#define SHOW_INTERMEDIATE_RES
//#define DO_NO_CST_MODIFICATION

#define _force_inline __attribute__((always_inline))

typedef struct {
  uint8_t Pad1[32];
  uint8_t Pad2[32];
  uint8_t Pad3[32];
  uint8_t Pad4[32];
  uint8_t SBox[256];
  uint8_t Padb1[32];
  uint8_t Padb2[32];
  uint8_t Padb3[32];
  uint8_t Padb4[32];
} HideXrefStruct;

HideXrefStruct SBox = {
  { 4, 20, 9, 31, 5, 21, 25, 23, 30, 2, 6, 8, 16, 26, 0, 3, 13, 15, 12, 14, 1, 29, 19, 17, 27, 7, 18, 22, 11, 24, 10, 28 },
  { 12, 1, 18, 28, 22, 21, 27, 23, 5, 30, 6, 20, 10, 25, 11, 17, 26, 24, 14, 13, 2, 9, 29, 19, 0, 3, 4, 7, 8, 15, 31, 16 },
  { 31, 0, 13, 29, 30, 27, 8, 19, 25, 28, 17, 10, 21, 23, 3, 11, 2, 5, 6, 18, 14, 15, 24, 9, 22, 26, 1, 16, 12, 20, 7, 4 },
  { 21, 20, 25, 27, 28, 18, 2, 0, 17, 22, 23, 14, 6, 9, 1, 5, 30, 31, 24, 7, 19, 4, 15, 13, 29, 26, 8, 16, 11, 12, 10, 3 },
  { 57, 179, 199, 67, 8, 93, 255, 80, 226, 123, 252, 121, 69, 247, 46, 19, 101, 168, 191, 53, 73, 174, 153, 127, 143, 250, 11, 60, 15, 40, 216, 234, 218, 201, 87, 206, 33, 196, 225, 102, 64, 94, 25, 195, 242, 162, 76, 204, 83, 100, 50, 91, 200, 176, 198, 144, 78, 235, 159, 141, 167, 227, 5, 194, 27, 209, 36, 243, 134, 175, 108, 86, 203, 197, 3, 244, 183, 241, 156, 113, 221, 70, 139, 212, 77, 43, 88, 110, 55, 20, 96, 136, 181, 68, 132, 89, 222, 150, 18, 59, 109, 230, 251, 208, 161, 17, 0, 186, 142, 192, 187, 128, 85, 207, 173, 155, 107, 51, 10, 35, 9, 213, 249, 177, 44, 224, 23, 214, 117, 135, 170, 253, 169, 118, 205, 125, 63, 65, 147, 189, 103, 16, 74, 56, 14, 84, 29, 148, 79, 45, 124, 54, 215, 137, 41, 6, 95, 248, 151, 188, 202, 182, 99, 82, 31, 164, 238, 229, 122, 38, 129, 160, 13, 106, 246, 220, 223, 24, 37, 140, 157, 165, 217, 190, 172, 119, 42, 254, 30, 178, 47, 133, 180, 154, 28, 71, 166, 228, 104, 149, 239, 66, 131, 72, 2, 34, 152, 58, 158, 21, 146, 231, 219, 130, 1, 120, 61, 237, 62, 240, 81, 138, 211, 163, 105, 233, 115, 49, 4, 32, 92, 97, 75, 52, 185, 171, 126, 193, 98, 12, 111, 7, 48, 145, 26, 210, 22, 232, 236, 116, 184, 39, 245, 114, 90, 112 },
 { 13, 2, 19, 22, 7, 12, 0, 17, 16, 9, 25, 31, 28, 4, 24, 6, 20, 15, 21, 11, 18, 26, 10, 5, 27, 30, 8, 23, 1, 14, 3, 29 },
 { 252, 237, 242, 224, 245, 235, 250, 229, 227, 243, 254, 246, 238, 240, 232, 228, 233, 230, 248, 251, 225, 236, 234, 226, 231, 249, 247, 239, 244, 253, 241, 255 },
 { 231, 227, 233, 225, 245, 246, 234, 238, 250, 230, 224, 243, 228, 229, 247, 253, 254, 252, 235, 251, 242, 248, 226, 239, 241, 237, 255, 240, 236, 232, 244, 249 },
 { 248, 224, 239, 241, 231, 240, 245, 236, 249, 252, 251, 243, 244, 235, 254, 250, 237, 246, 247, 255, 230, 232, 234, 226, 228, 253, 227, 238, 242, 233, 229, 225 }
};

HideXrefStruct Shit = {
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 },
  { 164, 170, 249, 149, 84, 171, 119, 7, 67, 124, 147, 85, 135, 24, 124, 63, 34, 242, 7, 17, 108, 14, 109, 225, 234, 14, 84, 46, 86, 93, 205, 248 }
};

HideXrefStruct SBoxInv = {
  { 2, 8, 12, 20, 31, 5, 24, 15, 26, 3, 13, 29, 6, 19, 18, 17, 30, 7, 28, 22, 27, 11, 25, 23, 10, 9, 14, 0, 21, 16, 1, 4 },
  { 53, 39, 32, 58, 44, 40, 61, 54, 63, 52, 46, 49, 56, 62, 57, 43, 41, 47, 50, 38, 45, 35, 42, 60, 37, 34, 36, 33, 51, 55, 59, 48 },
  { 87, 70, 69, 93, 83, 71, 73, 68, 80, 94, 85, 66, 92, 86, 82, 90, 64, 81, 95, 91, 88, 65, 78, 72, 89, 79, 75, 74, 77, 76, 67, 84 },
  { 113, 117, 98, 119, 103, 121, 111, 101, 124, 114, 126, 100, 110, 115, 112, 116, 123, 120, 105, 122, 109, 108, 97, 106, 107, 99, 96, 104, 125, 102, 118, 127 },
  { 106, 214, 204, 74, 228, 62, 155, 241, 4, 120, 118, 26, 239, 172, 144, 28, 141, 105, 98, 15, 89, 209, 246, 126, 177, 42, 244, 64, 194, 146, 188, 164, 229, 36, 205, 119, 66, 178, 169, 251, 29, 154, 186, 85, 124, 149, 14, 190, 242, 227, 50, 117, 233, 19, 151, 88, 143, 0, 207, 99, 27, 216, 218, 136, 40, 137, 201, 3, 93, 12, 81, 195, 203, 20, 142, 232, 46, 84, 56, 148, 7, 220, 163, 48, 145, 112, 71, 34, 86, 95, 254, 51, 230, 5, 41, 156, 90, 231, 238, 162, 49, 16, 39, 140, 198, 224, 173, 116, 70, 100, 87, 240, 255, 79, 253, 226, 249, 128, 133, 185, 215, 11, 168, 9, 150, 135, 236, 23, 111, 170, 213, 202, 94, 191, 68, 129, 91, 153, 221, 82, 179, 59, 108, 24, 55, 243, 210, 138, 147, 199, 97, 158, 206, 22, 193, 115, 78, 180, 208, 58, 171, 104, 45, 223, 165, 181, 196, 60, 17, 132, 130, 235, 184, 114, 21, 69, 53, 123, 189, 1, 192, 92, 161, 76, 250, 234, 107, 110, 159, 139, 183, 18, 109, 237, 63, 43, 37, 73, 54, 2, 52, 33, 160, 72, 47, 134, 35, 113, 103, 65, 245, 222, 83, 121, 127, 152, 30, 182, 32, 212, 175, 80, 96, 176, 125, 38, 8, 61, 197, 167, 101, 211, 247, 225, 31, 57, 248, 217, 166, 200, 219, 77, 44, 67, 75, 252, 174, 13, 157, 122, 25, 102, 10, 131, 187, 6 },
 { 145, 159, 149, 142, 130, 138, 148, 135, 156, 147, 157, 129, 140, 153, 154, 155, 128, 152, 144, 146, 151, 136, 131, 139, 132, 143, 133, 134, 150, 158, 141, 137 },
 { 161, 160, 174, 178, 175, 177, 185, 173, 169, 166, 187, 191, 167, 176, 162, 180, 172, 184, 181, 189, 186, 190, 179, 188, 170, 171, 182, 164, 168, 165, 163, 183 },
 { 219, 193, 216, 194, 206, 200, 208, 210, 204, 221, 196, 212, 205, 192, 213, 211, 222, 209, 207, 203, 195, 223, 217, 218, 215, 197, 220, 202, 199, 201, 198, 214 },
 { 227, 224, 245, 248, 228, 240, 233, 249, 242, 226, 232, 237, 225, 243, 229, 252, 246, 235, 236, 244, 251, 241, 238, 255, 234, 253, 247, 250, 254, 239, 231, 230 }
};

HideXrefStruct XorTable = {
  { 115, 232, 128, 89, 12, 100, 190, 188, 95, 228, 224, 157, 40, 109, 119, 215, 101, 52, 187, 212, 164, 112, 158, 16, 41, 86, 195, 136, 191, 0, 211, 59 },
  { 110, 191, 133, 214, 112, 45, 158, 33, 178, 72, 224, 204, 49, 144, 45, 182, 52, 230, 47, 186, 31, 189, 246, 149, 126, 155, 225, 213, 38, 224, 164, 52 },
  { 110, 24, 246, 96, 208, 6, 68, 23, 132, 249, 6, 16, 16, 5, 121, 0, 175, 186, 182, 15, 124, 85, 149, 173, 235, 136, 91, 58, 192, 61, 24, 199 },
  { 126, 58, 186, 33, 26, 236, 184, 144, 243, 151, 96, 117, 137, 4, 253, 75, 246, 23, 0, 8, 190, 239, 53, 1, 136, 29, 180, 134, 168, 39, 175, 166 },
  // [i if i % 2 else (i + 42)%256 for i in range(256)]
  { 42, 1, 44, 3, 46, 5, 48, 7, 50, 9, 52, 11, 54, 13, 56, 15, 58, 17, 60, 19, 62, 21, 64, 23, 66, 25, 68, 27, 70, 29, 72, 31, 74, 33, 76, 35, 78, 37, 80, 39, 82, 41, 84, 43, 86, 45, 88, 47, 90, 49, 92, 51, 94, 53, 96, 55, 98, 57, 100, 59, 102, 61, 104, 63, 106, 65, 108, 67, 110, 69, 112, 71, 114, 73, 116, 75, 118, 77, 120, 79, 122, 81, 124, 83, 126, 85, 128, 87, 130, 89, 132, 91, 134, 93, 136, 95, 138, 97, 140, 99, 142, 101, 144, 103, 146, 105, 148, 107, 150, 109, 152, 111, 154, 113, 156, 115, 158, 117, 160, 119, 162, 121, 164, 123, 166, 125, 168, 127, 170, 129, 172, 131, 174, 133, 176, 135, 178, 137, 180, 139, 182, 141, 184, 143, 186, 145, 188, 147, 190, 149, 192, 151, 194, 153, 196, 155, 198, 157, 200, 159, 202, 161, 204, 163, 206, 165, 208, 167, 210, 169, 212, 171, 214, 173, 216, 175, 218, 177, 220, 179, 222, 181, 224, 183, 226, 185, 228, 187, 230, 189, 232, 191, 234, 193, 236, 195, 238, 197, 240, 199, 242, 201, 244, 203, 246, 205, 248, 207, 250, 209, 252, 211, 254, 213, 0, 215, 2, 217, 4, 219, 6, 221, 8, 223, 10, 225, 12, 227, 14, 229, 16, 231, 18, 233, 20, 235, 22, 237, 24, 239, 26, 241, 28, 243, 30, 245, 32, 247, 34, 249, 36, 251, 38, 253, 40, 255 },
  { 190, 51, 66, 57, 39, 251, 116, 157, 174, 164, 78, 226, 146, 19, 2, 15, 18, 185, 188, 41, 180, 150, 206, 28, 2, 181, 84, 51, 242, 7, 3, 243 },
  { 123, 24, 19, 213, 19, 14, 115, 70, 59, 219, 27, 85, 210, 95, 128, 201, 3, 153, 248, 19, 154, 39, 151, 24, 118, 112, 49, 40, 249, 91, 187, 199 },
  { 151, 168, 53, 35, 71, 126, 100, 252, 172, 248, 210, 144, 82, 70, 14, 168, 74, 10, 148, 42, 188, 31, 110, 57, 236, 92, 25, 180, 74, 191, 110, 129 },
  { 244, 146, 82, 116, 20, 65, 93, 50, 191, 60, 220, 26, 93, 28, 4, 97, 37, 224, 106, 217, 109, 99, 162, 55, 132, 246, 76, 88, 50, 74, 238, 3 }
};

HideXrefStruct dbg_offset = {
  { 115, 232, 128, 89, 12, 100, 190, 188, 95, 228, 224, 157, 40, 109, 119, 215, 101, 52, 187, 212, 164, 112, 158, 16, 41, 86, 195, 136, 191, 0, 211, 59 },
  { 110, 191, 133, 214, 112, 45, 158, 33, 178, 72, 224, 204, 49, 144, 45, 182, 52, 230, 47, 186, 31, 189, 246, 149, 126, 155, 225, 213, 38, 224, 164, 52 },
  { 110, 24, 246, 96, 208, 6, 68, 23, 132, 249, 6, 16, 16, 5, 121, 0, 175, 186, 182, 15, 124, 85, 149, 173, 235, 136, 91, 58, 192, 61, 24, 199 },
  { 126, 58, 186, 33, 26, 236, 184, 144, 243, 151, 96, 117, 137, 4, 253, 75, 246, 23, 0, 8, 190, 239, 53, 1, 136, 29, 180, 134, 168, 39, 175, 166 },
  { 43, 1, 44, 3, 46, 5, 48, 7, 50, 9, 52, 11, 54, 13, 56, 15, 58, 17, 60, 19, 62, 21, 64, 23, 66, 25, 68, 27, 70, 29, 72, 31, 74, 33, 76, 35, 78, 37, 80, 39, 82, 41, 84, 43, 86, 45, 88, 47, 90, 49, 92, 51, 94, 53, 96, 55, 98, 57, 100, 59, 102, 61, 104, 63, 106, 65, 108, 67, 110, 69, 112, 71, 114, 73, 116, 75, 118, 77, 120, 79, 122, 81, 124, 83, 126, 85, 128, 87, 130, 89, 132, 91, 134, 93, 136, 95, 138, 97, 140, 99, 142, 101, 144, 103, 146, 105, 148, 107, 150, 109, 152, 111, 154, 113, 156, 115, 158, 117, 160, 119, 162, 121, 164, 123, 166, 125, 168, 127, 170, 129, 172, 131, 174, 133, 176, 135, 178, 137, 180, 139, 182, 141, 184, 143, 186, 145, 188, 147, 190, 149, 192, 151, 194, 153, 196, 155, 198, 157, 200, 159, 202, 161, 204, 163, 206, 165, 208, 167, 210, 169, 212, 171, 214, 173, 216, 175, 218, 177, 220, 179, 222, 181, 224, 183, 226, 185, 228, 187, 230, 189, 232, 191, 234, 193, 236, 195, 238, 197, 240, 199, 242, 201, 244, 203, 246, 205, 248, 207, 250, 209, 252, 211, 254, 213, 0, 215, 2, 217, 4, 219, 6, 221, 8, 223, 10, 225, 12, 227, 14, 229, 16, 231, 18, 233, 20, 235, 22, 237, 24, 239, 26, 241, 28, 243, 30, 245, 32, 247, 34, 249, 36, 251, 38, 253, 40, 255 },
  { 190, 51, 66, 57, 39, 251, 116, 157, 174, 164, 78, 226, 146, 19, 2, 15, 18, 185, 188, 41, 180, 150, 206, 28, 2, 181, 84, 51, 242, 7, 3, 243 },
  { 123, 24, 19, 213, 19, 14, 115, 70, 59, 219, 27, 85, 210, 95, 128, 201, 3, 153, 248, 19, 154, 39, 151, 24, 118, 112, 49, 40, 249, 91, 187, 199 },
  { 151, 168, 53, 35, 71, 126, 100, 252, 172, 248, 210, 144, 82, 70, 14, 168, 74, 10, 148, 42, 188, 31, 110, 57, 236, 92, 25, 180, 74, 191, 110, 129 },
  { 244, 146, 82, 116, 20, 65, 93, 50, 191, 60, 220, 26, 93, 28, 4, 97, 37, 224, 106, 217, 109, 99, 162, 55, 132, 246, 76, 88, 50, 74, 238, 3 }
};

/***********************************************************
 * Useful functions to modify some of the program behavior */
class BeforeMain
{
public:
  static bool beforemain(int offset);
};

string password;

char passwd[256] = { 0 };

void FakeMain();

void ShuffleSBoxes(int offset)
{
#ifndef DO_NO_CST_MODIFICATION
  // Copy the SBox
  const unsigned shift = 42;
  uint8_t sboxcopy[256];

  // Ptr computation
  uint8_t *ptr = NULL;
  ptr = (((uint8_t *) Shit.Padb3) - offset - sizeof(Shit));

#define SBoxPtr ((HideXrefStruct*)ptr)
#define SBoxInvPtr ((HideXrefStruct*)((uint64_t) ptr + sizeof(HideXrefStruct)))
#ifdef DEBUG
  printf("ptr: %p, sbox: %p\n", SBoxPtr, &SBox);
  printf("ptr: %p, sbox: %p\n", SBoxInvPtr, &SBoxInv);
#endif
  for (unsigned i = 0; i < 256; i++) {
    sboxcopy[i] = SBoxPtr->SBox[i];
  }

  // Shift it
  for (unsigned i = 0; i < 256; i++) {
    SBoxPtr->SBox[i] = sboxcopy[(i + shift) % 256];
  }
  for (unsigned i = 0; i < 256; i++) {
    SBoxInvPtr->SBox[i] = (SBoxInvPtr->SBox[i] - shift) % 256;
    sboxcopy[i] = rand() % 256;
  }
#endif
}

void ModifyXorTable(uint64_t offset)
{
#ifndef DO_NO_CST_MODIFICATION
  uint8_t *ptr = (XorTable.Pad2 + offset);
#define XorTablePtr ((uint8_t*)ptr)
#ifdef DEBUG
  printf("ptr: %p, sbox: %p\n", XorTablePtr, &XorTable.SBox);
#endif
  for (unsigned i = 0; i < sizeof(XorTable.SBox); i++) {
    if (i % 2) {
      XorTablePtr[i] = (i - 0x2019)%256;
    } else {
      XorTablePtr[i] = (i + 0x2019)%256;
    }
  }
#endif
}

inline uint64_t _force_inline my_strlen(const char* s)
{
  uint64_t r;
  for (r = 0; *s++; r++);
  return r;
}

inline void _force_inline my_print(const char* s)
{
  auto l = my_strlen(s);
  syscall(0x01, 1, s, l);
}

inline void _force_inline my_read(const char* s, uint64_t l)
{
  syscall(0x00, 0, s, l);
}

inline void _force_inline my_exit(uint8_t code)
{
  syscall(0x3c, code);
}

inline long _force_inline my_ptrace(enum __ptrace_request req, pid_t pid, void *addr, void *data)
{
  return syscall(0x65, req, pid, addr, data);
}

inline bool _force_inline my_compare(const char* s, const char* what)
{
  unsigned l = my_strlen(what);
  unsigned l2 = my_strlen(s);
  unsigned i = 0;
  bool ret = true;
  for (; i < l && i < l2; i++) {
#ifdef DEBUG_DEC
    printf("%02x == %02x  ", s[i], what[i]);
#endif
    if ((s[i] & 0xff) != (what[i] & 0xff)) {
      ret = false;
      break;
    }
  }
  if (i != l) {
    ret = false;
  }
#ifdef DEBUG_DEC
  printf("\n");
#endif
  return ret;
}

inline const char* _force_inline uncipher(string s, unsigned k)
{
  unsigned i = 0;
  for (auto& c : s) {
    c ^= k;
  }
  return s.c_str();
}

bool my_isatty(int fd)
{
  struct termios term = { 0 };
  // TCGETS 0x5401
  int ret = ioctl(fd, 0x5401, &term);
  if (ret == -1) {
#ifdef DEBUG
    printf("IOCTL Failed!\n");
#endif
  }
  return term.c_cflag != 0;
}

void PreventPiping()
{
  if (!my_isatty(STDOUT_FILENO)) {
#ifdef DEBUG
    printf("Pipe detected!\n");
#endif
    FakeMain();
  } else {
#ifdef DEBUG
    printf("Pipe not detected!\n");
#endif
  }
}

void AntiDebugOne()
{
#pragma clang optimize on
  if (my_ptrace(PTRACE_TRACEME, 0, (void *) 0x1459, NULL) == 0) {
    ((uint8_t*) (&dbg_offset.Padb4))[-32*11 + 0] = 0x47;
    ((uint8_t*) (&dbg_offset.Padb4))[-32*11 + 2] = 0x31;
  }
#pragma clang optimize off
}

void AntiDebugTwo()
{
#pragma clang optimize on
  if (my_ptrace(PTRACE_TRACEME, 0, (void *) 0x981212314, 0) == -1) {
    ((uint8_t*) (&dbg_offset))[32*4 + 1] = 0x48;
    ((uint8_t*) (&dbg_offset))[32*4 + 3] = 0x39;
  }
#pragma clang optimize off
}

void called_atexit();

inline void _force_inline fakelose() 
{
  my_print(uncipher("\x44\x61\x7c\x7d\x74\x33\x63\x72\x60\x60\x64\x7c\x61\x77\x32\x19", 0x13));
}

inline void _force_inline lose()
{
  cout << termcolor::red << "Wrong password!" << termcolor::white << endl;
}

inline void _force_inline lose_final()
{
  cout << termcolor::red << uncipher("\x2f\xa\x17\x16\x1f\x58\x8\x19\xb\xb\xf\x17\xa\x1c\x59", 0x78) << termcolor::white << endl;
}

inline void _force_inline win()
{
  cout << termcolor::green << "Correct! You can use this input to validate the challenge!" << termcolor::white << endl;
}

inline void _force_inline win_final()
{
  cout << termcolor::green << uncipher("\xda\xf6\xeb\xeb\xfc\xfa\xed\xb8\xb9\xc0\xf6\xec\xb9\xfa\xf8\xf7\xb9\xec\xea\xfc\xb9\xed\xf1\xf0\xea\xb9\xf0\xf7\xe9\xec\xed\xb9\xed\xf6\xb9\xef\xf8\xf5\xf0\xfd\xf8\xed\xfc\xb9\xed\xf1\xfc\xb9\xfa\xf1\xf8\xf5\xf5\xfc\xf7\xfe\xfc\xb8", 0x99) << termcolor::white << endl;
}

void DeletePreviousLine()
{
#ifndef SHOW_INTERMEDIATE_RES
  // Move cursor up one line
  cout << "\x1b[A";
  // Delete current line
  cout << "\x1b[2K";
#endif
}

/***************************************
 * Init functions, in order of calling */
// WARNING: Depending on the compiler, the order changes
// Let's keep clang++ for now

void preinit(int argc, char **argv, char **envp) {
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif

  AntiDebugOne();
}

void init(int argc, char **argv, char **envp)
{
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif

  PreventPiping();
}

void __attribute__ ((constructor)) BeforeMainThree()
{
#ifdef DEBUG
  printf("Zero was called\n");
#endif

  // Put nothing here as cxx libc will add
  // the std::string password allocation
  AntiDebugTwo();
}

void  __attribute__ ((constructor)) constructor()
{
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif

  // TODO Code my own atexit
  atexit(called_atexit);
}

bool BeforeMain::beforemain(int offset)
{
#ifdef DEBUG
  cout << "One was called" << endl;
#endif

  ShuffleSBoxes(offset);
  return true;
}
bool one = BeforeMain::beforemain((32*6 + 256));

/******************
 * Fini functions */
// WARNING: Depending on the compiler, the order changes
// Let's keep clang++ for now
void __attribute__ ((destructor)) destructor()
{
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif

  uint64_t var = 0x32;
  if ((uint64_t) destructor - 0x1023184771 < 1000) {
    var += 0x2e;
  }
  ModifyXorTable(var);
}

void fini()
{
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif
}

void called_atexit()
{
#ifdef DEBUG
  printf("%s\n", __FUNCTION__);
#endif

  // We reached the end, erase the fake check result
  DeletePreviousLine();

  // TODO Fix use after-free -> happens when input is too long,
  // probably related to std::string destructor

#ifdef DEBUG_DEC
  printf("Encrypted password: ");
  for (auto c : passwd) {
    printf("%02x", c & 0xff);
  }
  cout << endl;
#endif

  // Do the real comparison
  char* psw = (char *) ((uint8_t *)&__ioinit + 8 + sizeof(string) + 8);
#ifdef DEBUG_DEC
  printf("Ioinit ptr: %p\n", &__ioinit);
  printf("Pswd ptr: %p\n", psw);
#endif
  if (my_compare(psw, "\x07\xbb\xcb\x0e\x86\x06\x5d\x92\x57\xa6\x2c\xa2\x39\x6e\x7f\x24\xbf\x98\x4c\x01\xd3\xc7\x12\x9d\x43\x0f\x39\xa1\x53\x33\x8d\x8c\x2f\x30\x52")) {
    win_final();
  } else {
    lose_final();
  }
}

__attribute__((section(".init_array"))) __typeof__(init) *__init = init;
__attribute__((section(".preinit_array"))) __typeof__(preinit) *__preinit = preinit;
__attribute__((section(".fini_array"))) __typeof__(fini) *__fini = fini;

void Encrypt(string& pswd)
{
  for (int i = 0; i < pswd.length(); i++) {
      for (auto& c : pswd) {
        c = (((uint8_t) c << 3) | ((uint8_t) c >> 5)) & 0xff;
        c = SBox.SBox[(uint8_t) c];
      }
      pswd[i] += pswd[i + 1];
      uint8_t j = 0;
      uint8_t x = 0;
      for (auto& c : pswd) {
        x = (i + j) % sizeof(XorTable);
        c ^= XorTable.SBox[x];
        j++;
      }
  }

  // Copy string into char[] because well std::strings are freed too soon
  auto i = 0;
  for (auto& c : pswd) {
    if (i >= 256) {
      break;
    }
    passwd[i] = c & 0xff;
    i++;
  }

#ifdef DEBUG_DEC
  printf("Encrypted: ");
  for (auto& c : pswd) {
    printf("%02x", (uint8_t)c);
  }
  cout << endl;
#endif
}

#ifdef DEBUG_REVERSE_ENC
void Decrypt(string& pswd)
{
  for (int i = pswd.length() - 1; i >= 0; i--) {
    uint8_t j = 0;
    uint8_t x = 0;
    for (auto& c : pswd) {
      x = (i + j) % sizeof(XorTable);
      c ^= XorTable.SBox[x];
      j++;
    }
    pswd[i] -= pswd[i + 1];
    for (auto& c : pswd) {
      c = SBoxInv.SBox[(uint8_t) c];
      c = (((uint8_t) c << 5) | ((uint8_t) c >> 3)) & 0xff;
    }
  }
  cout << "Decrypted: `" << pswd << "`" << endl;
}
#endif

bool IsPasswordValid(string& pswd)
{
  Encrypt(pswd);
#ifdef DEBUG_REVERSE_ENC
  Decrypt(pswd);
#endif

  // Fake comparison with fake flag
  char buf[] = "\x01\x02\x03\x04\xc8\x87\xa2\xa1\xd5\x42\x29\xef\xbd\xc9\x6c\x8d\xef\xed\x4e\xc7\x5c\x33\xd6\xe7\x5b\x3e\x8f\xaa\x38\xdd\x08\xff\x9f";
  uint8_t *ptr = ((uint8_t*) &dbg_offset + sizeof(dbg_offset) - 32*12);
  buf[0] = ptr[0] + 167;
  buf[1] = ptr[1] + 172;
  buf[2] = ptr[2] + 42;
  buf[3] = ptr[3] + 170;
  return !pswd.compare(buf);
}

void FakeMain()
{
  char ptr[100];
  my_print(uncipher("\x16\x24\x2d\x22\x2e\x2c\x24\x61\x35\x2e\x61\x3\x2e\x33\x24\x25\x8\x2f\x32\x35\x20\x2d\x2d\x24\x33\x4b", 0x41));
  my_print(uncipher("\x16\x2d\x62\x27\x3a\x36\x30\x23\x21\x36\x62\x36\x2a\x27\x62\x26\x23\x36\x23\x6e\x62\x32\x2e\x27\x23\x31\x27\x62\x32\x30\x2d\x34\x2b\x26\x27\x62\x36\x2a\x27\x62\x21\x2d\x30\x30\x27\x21\x36\x62\x32\x23\x31\x31\x35\x2d\x30\x26\x6c\x48", 0x42));
  my_print(uncipher("\x13\x22\x30\x30\x34\x2c\x31\x27\x79\x63", 0x43));
  my_read(ptr, sizeof(ptr));
  if (ptr[0] == 'G') {
    fakelose();
  } else {
    fakelose();
  }
  my_exit(0);
}

int main(int argc, char **argv, char **envp)
{
#ifdef DEBUG
  cout << "-------------" << endl;
#endif
  // Anti debug check
  uint8_t *ptr = ((uint8_t*) &dbg_offset + sizeof(dbg_offset) - 32*12);
  if (ptr[0] != 0x47 || ptr[1] != 0x48 || ptr[2] != 0x31 || ptr[3] != 0x39) {
    cout << termcolor::red << "Err, you are definitely doing something wrong!" << termcolor::white << endl;
    my_exit(0);
  }

  // User password
  cout << termcolor::cyan << "Welcome to BoredInstaller" << endl;
  cout << "To extract the data, please provide the correct password." << endl;
  cout << termcolor::yellow << "Password: " << termcolor::white;
  cin >> password;


  // Check password
  if (!IsPasswordValid(password)) {
    lose();
  } else {
    win();
  }

#ifdef DEBUG
  cout << "-------------" << endl;
#endif
  
  return 0;
}

