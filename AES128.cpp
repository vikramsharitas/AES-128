#include "AES128.h"

using namespace std;

typedef bitset<8> byte;
typedef bitset<32> word;

extern byte S_Box[16][16] = {
    {0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76},
    {0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0},
    {0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15},
    {0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75},
    {0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84},
    {0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF},
    {0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8},
    {0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2},
    {0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73},
    {0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB},
    {0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79},
    {0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08},
    {0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A},
    {0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E},
    {0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF},
    {0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16}
};

extern byte RS_Box[16][16] = {
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
    {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
    {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
    {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
    {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
    {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
    {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
    {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
    {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}
};

extern word Rcon[10] = { 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
                 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000 };



word Word(byte& k1, byte& k2, byte& k3, byte& k4)
{
    word result(0x00000000);
    word temp;
    temp = k1.to_ulong();  // K1
    temp <<= 24;
    result |= temp;
    temp = k2.to_ulong();  // K2
    temp <<= 16;
    result |= temp;
    temp = k3.to_ulong();  // K3
    temp <<= 8;
    result |= temp;
    temp = k4.to_ulong();  // K4
    result |= temp;
    return result;
}

word RotWord(word& rw)
{
    word high = rw << 8;
    word low = rw >> 24;
    return high | low;
}

word SubWord(word& sw)
{
    word temp;
    for (int i = 0; i < 32; i += 8)
    {
        int row = sw[i + 7] * 8 + sw[i + 6] * 4 + sw[i + 5] * 2 + sw[i + 4];
        int col = sw[i + 3] * 8 + sw[i + 2] * 4 + sw[i + 1] * 2 + sw[i];
        byte val = S_Box[row][col];
        for (int j = 0; j < 8; ++j)
            temp[i + j] = val[j];
    }
    return temp;
}

void KeyExpansion(byte key[16], word w[44])
{
    word temp;
    int i = 0;
    while (i < 4)
    {
        w[i] = Word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
        ++i;
    }

    i = 4;

    while (i < 4 * (10 + 1))
    {
        temp = w[i - 1];
        if (i % 4 == 0)
        {
            temp = RotWord(temp);
            w[i] = w[i - 4] ^ SubWord(temp) ^ Rcon[i / 4 - 1];
        }
        else
            w[i] = w[i - 4] ^ temp;
        ++i;
    }
}



void SubBytes(byte mtx[4 * 4])
{
    for (int i = 0; i < 16; ++i)
    {
        int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
        int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
        mtx[i] = S_Box[row][col];
    }
}

void ShiftRows(byte mtx[4 * 4])
{
    byte temp = mtx[4];
    for (int i = 0; i < 3; ++i)
        mtx[i + 4] = mtx[i + 5];
    mtx[7] = temp;

    for (int i = 0; i < 2; ++i)
    {
        temp = mtx[i + 8];
        mtx[i + 8] = mtx[i + 10];
        mtx[i + 10] = temp;
    }

    temp = mtx[15];
    for (int i = 3; i > 0; --i)
        mtx[i + 12] = mtx[i + 11];
    mtx[12] = temp;
}

byte GFMul(byte a, byte b) 
{
    byte p = 0;
    byte hi_bit_set;
    for (int counter = 0; counter < 8; counter++) {
        if ((b & byte(1)) != 0) {
            p ^= a;
        }
        hi_bit_set = (byte)(a & byte(0x80));
        a <<= 1;
        if (hi_bit_set != 0) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

void MixColumns(byte mtx[4 * 4])
{
    byte arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = mtx[i + j * 4];

        mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
        mtx[i + 4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];
        mtx[i + 8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);
        mtx[i + 12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);
    }
}

void AddRoundKey(byte mtx[4 * 4], word k[4])
{
    for (int i = 0; i < 4; ++i)
    {
        word k1 = k[i] >> 24;
        word k2 = (k[i] << 8) >> 24;
        word k3 = (k[i] << 16) >> 24;
        word k4 = (k[i] << 24) >> 24;

        mtx[i] = mtx[i] ^ byte(k1.to_ulong());
        mtx[i + 4] = mtx[i + 4] ^ byte(k2.to_ulong());
        mtx[i + 8] = mtx[i + 8] ^ byte(k3.to_ulong());
        mtx[i + 12] = mtx[i + 12] ^ byte(k4.to_ulong());
    }
}

void transpose(byte mtx[16])
{
    byte m[16];
    for (int i = 0; i < 16; i++)
    {
        m[i] = mtx[i];
    }
    for (int i = 0; i < 4; i++)
    {
        mtx[4 * i + 0] = m[i + 0];
        mtx[4 * i + 1] = m[i + 4];
        mtx[4 * i + 2] = m[i + 8];
        mtx[4 * i + 3] = m[i + 12];
    }
}



void InvSubBytes(byte mtx[4 * 4])
{
    for (int i = 0; i < 16; ++i)
    {
        int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
        int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
        mtx[i] = RS_Box[row][col];
    }
}

void InvMixColumns(byte mtx[4 * 4])
{
    byte arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = mtx[i + j * 4];

        mtx[i] = GFMul(0x0e, arr[0]) ^ GFMul(0x0b, arr[1]) ^ GFMul(0x0d, arr[2]) ^ GFMul(0x09, arr[3]);
        mtx[i + 4] = GFMul(0x09, arr[0]) ^ GFMul(0x0e, arr[1]) ^ GFMul(0x0b, arr[2]) ^ GFMul(0x0d, arr[3]);
        mtx[i + 8] = GFMul(0x0d, arr[0]) ^ GFMul(0x09, arr[1]) ^ GFMul(0x0e, arr[2]) ^ GFMul(0x0b, arr[3]);
        mtx[i + 12] = GFMul(0x0b, arr[0]) ^ GFMul(0x0d, arr[1]) ^ GFMul(0x09, arr[2]) ^ GFMul(0x0e, arr[3]);
    }
}



string AESencrypt(string iv, string pt)
{
    byte k[16];
    byte in[16];
    word w[44];
    //cin >> pt >> iv;
    unsigned short tempValue;
    for (short int i = 0; i < 16; i++)
    {
        istringstream ost(iv.substr(i * 2, 2));
        ost >> hex >> tempValue;
        k[i] = tempValue;
    }

    while (pt.length() < 32)
    {
        pt = '0' + pt;
    }
    for (short int i = 0; i < 16; i++)
    {
        istringstream ost(pt.substr(i * 2, 2));
        ost >> hex >> tempValue;
        in[i] = tempValue;
    }
    //pt = "54776F204F6E65204E696E652054776F", key = "5468617473206D79204B756E67204675";
    KeyExpansion(k, w);
    transpose(in);
    word key[4];
    for (int i = 0; i < 4; ++i)
        key[i] = w[i];
    AddRoundKey(in, key);

    for (int round = 1; round < 10; ++round)
    {
        SubBytes(in);
        ShiftRows(in);
        MixColumns(in);
        for (int i = 0; i < 4; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(in, key);
    }

    SubBytes(in);
    ShiftRows(in);
    for (int i = 0; i < 4; ++i)
        key[i] = w[4 * 10 + i];
    AddRoundKey(in, key);
    transpose(in);

    stringstream s;
    s.str("");
    for (int i = 0; i < 16; ++i)
    {
        if (in[i].to_ulong() >= 0x10)
            s << hex << in[i].to_ulong();
        else
            s << '0' << hex << in[i].to_ulong();
    }
    return s.str();
}



string AESdecrypt(string iv, string ct)
{
    byte k[16];
    byte in[16];
    word w[44];
    //cin >> ct >> iv;
    unsigned short tempValue;
    for (short int i = 0; i < 16; i++)
    {
        istringstream ost(iv.substr(i * 2, 2));
        ost >> hex >> tempValue;
        k[i] = tempValue;
    }

    for (short int i = 0; i < 16; i++)
    {
        istringstream ost(ct.substr(i * 2, 2));
        ost >> hex >> tempValue;
        in[i] = tempValue;
    }
    //pt = "54776F204F6E65204E696E652054776F", key = "5468617473206D79204B756E67204675";
    KeyExpansion(k, w);
    transpose(in);
    word key[4];
    for (int i = 0; i < 4; ++i)
        key[i] = w[4 * 10 + i];
    AddRoundKey(in, key);

    for (int round = 9; round > 0; --round)
    {
        ShiftRows(in);
        ShiftRows(in);
        ShiftRows(in);
        InvSubBytes(in);
        for (int i = 0; i < 4; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(in, key);
        InvMixColumns(in);
    }

    ShiftRows(in);
    ShiftRows(in);
    ShiftRows(in);
    InvSubBytes(in);
    for (int i = 0; i < 4; ++i)
        key[i] = w[i];
    AddRoundKey(in, key);
    transpose(in);

    stringstream s;
    s.str("");
    for (int i = 0; i < 16; ++i)
    {
        if (in[i].to_ulong() >= 0x10)
            s << hex << in[i].to_ulong();
        else
            s << '0' << hex << in[i].to_ulong();
    }
    return s.str();
}



void hex_toLL(string hex, long long int out)
{
    out = 0;
    unordered_map<char, int> mp;
    mp['0'] = 0x00;
    mp['1'] = 0x01;
    mp['2'] = 0x02;
    mp['3'] = 0x03;
    mp['4'] = 0x04;
    mp['5'] = 0x05;
    mp['6'] = 0x06;
    mp['7'] = 0x07;
    mp['8'] = 0x08;
    mp['9'] = 0x09;
    mp['a'] = 0x0a;
    mp['b'] = 0x0b;
    mp['c'] = 0x0c;
    mp['d'] = 0x0d;
    mp['e'] = 0x0e;
    mp['f'] = 0x0f;
    int a = hex.length();
    out = mp[a - 1];
    for (int i = a - 2; a >= 0; --a)
    {
        out << 4;
        out += mp[i];
    }
}
