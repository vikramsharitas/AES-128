#include "AES128.h"

using namespace std;

string S_Box[16][16] = {
    {"63", "7C", "77", "7B", "F2", "6B", "6F", "C5", "30", "01", "67", "2B", "FE", "D7", "AB", "76"},
    {"CA", "82", "C9", "7D", "FA", "59", "47", "F0", "AD", "D4", "A2", "AF", "9C", "A4", "72", "C0"},
    {"B7", "FD", "93", "26", "36", "3F", "F7", "CC", "34", "A5", "E5", "F1", "71", "D8", "31", "15"},
    {"04", "C7", "23", "C3", "18", "96", "05", "9A", "07", "12", "80", "E2", "EB", "27", "B2", "75"},
    {"09", "83", "2C", "1A", "1B", "6E", "5A", "A0", "52", "3B", "D6", "B3", "29", "E3", "2F", "84"},
    {"53", "D1", "00", "ED", "20", "FC", "B1", "5B", "6A", "CB", "BE", "39", "4A", "4C", "58", "CF"},
    {"D0", "EF", "AA", "FB", "43", "4D", "33", "85", "45", "F9", "02", "7F", "50", "3C", "9F", "A8"},
    {"51", "A3", "40", "8F", "92", "9D", "38", "F5", "BC", "B6", "DA", "21", "10", "FF", "F3", "D2"},
    {"CD", "0C", "13", "EC", "5F", "97", "44", "17", "C4", "A7", "7E", "3D", "64", "5D", "19", "73"},
    {"60", "81", "4F", "DC", "22", "2A", "90", "88", "46", "EE", "B8", "14", "DE", "5E", "0B", "DB"},
    {"E0", "32", "3A", "0A", "49", "06", "24", "5C", "C2", "D3", "AC", "62", "91", "95", "E4", "79"},
    {"E7", "C8", "37", "6D", "8D", "D5", "4E", "A9", "6C", "56", "F4", "EA", "65", "7A", "AE", "08"},
    {"BA", "78", "25", "2E", "1C", "A6", "B4", "C6", "E8", "DD", "74", "1F", "4B", "BD", "8B", "8A"},
    {"70", "3E", "B5", "66", "48", "03", "F6", "0E", "61", "35", "57", "B9", "86", "C1", "1D", "9E"},
    {"E1", "F8", "98", "11", "69", "D9", "8E", "94", "9B", "1E", "87", "E9", "CE", "55", "28", "DF"},
    {"8C", "A1", "89", "0D", "BF", "E6", "42", "68", "41", "99", "2D", "0F", "B0", "54", "BB", "16"}
};

string RS_Box[16][16] = {
    {"52", "09", "6a", "d5", "30", "36", "a5", "38", "bf", "40", "a3", "9e", "81", "f3", "d7", "fb"},
    {"7c", "e3", "39", "82", "9b", "2f", "ff", "87", "34", "8e", "43", "44", "c4", "de", "e9", "cb"},
    {"54", "7b", "94", "32", "a6", "c2", "23", "3d", "ee", "4c", "95", "0b", "42", "fa", "c3", "4e"},
    {"08", "2e", "a1", "66", "28", "d9", "24", "b2", "76", "5b", "a2", "49", "6d", "8b", "d1", "25"},
    {"72", "f8", "f6", "64", "86", "68", "98", "16", "d4", "a4", "5c", "cc", "5d", "65", "b6", "92"},
    {"6c", "70", "48", "50", "fd", "ed", "b9", "da", "5e", "15", "46", "57", "a7", "8d", "9d", "84"},
    {"90", "d8", "ab", "00", "8c", "bc", "d3", "0a", "f7", "e4", "58", "05", "b8", "b3", "45", "06"},
    {"d0", "2c", "1e", "8f", "ca", "3f", "0f", "02", "c1", "af", "bd", "03", "01", "13", "8a", "6b"},
    {"3a", "91", "11", "41", "4f", "67", "dc", "ea", "97", "f2", "cf", "ce", "f0", "b4", "e6", "73"},
    {"96", "ac", "74", "22", "e7", "ad", "35", "85", "e2", "f9", "37", "e8", "1c", "75", "df", "6e"},
    {"47", "f1", "1a", "71", "1d", "29", "c5", "89", "6f", "b7", "62", "0e", "aa", "18", "be", "1b"},
    {"fc", "56", "3e", "4b", "c6", "d2", "79", "20", "9a", "db", "c0", "fe", "78", "cd", "5a", "f4"},
    {"1f", "dd", "a8", "33", "88", "07", "c7", "31", "b1", "12", "10", "59", "27", "80", "ec", "5f"},
    {"60", "51", "7f", "a9", "19", "b5", "4a", "0d", "2d", "e5", "7a", "9f", "93", "c9", "9c", "ef"},
    {"a0", "e0", "3b", "4d", "ae", "2a", "f5", "b0", "c8", "eb", "bb", "3c", "83", "53", "99", "61"},
    {"17", "2b", "04", "7e", "ba", "77", "d6", "26", "e1", "69", "14", "63", "55", "21", "0c", "7d"}
};

string Rcon[10] = { "01000000", "02000000", "04000000", "08000000", "10000000",
                 "20000000", "40000000", "80000000", "1b000000", "36000000" };



string XOR(string a, string b)
{
    int l = a.length();
    string c;
    if (l == b.length())
    {
        for (int i = 0; i < l; ++i)
        {
            if (a[i] == '0')
            {
                if (b[i] == '0')
                    c += '0';
                else
                    c += '1';
            }
            else
            {
                if (b[i] == '0')
                    c += '1';
                else
                    c += '0';
            }
        }
    }
    return c;
}

void RotWord(string& rw)
{
    for (size_t i = 0; i < 8; i++)
    {
        char a = rw[0];
        for (int i = 0; i < rw.length() - 1; i++)
        {
            rw[i] = rw[i + 1];
        }
        rw[rw.length() - 1] = a;
    }
}

void HtoB(string& a)
{
    unordered_map<char, string> map;
    map['0'] = "0000";
    map['1'] = "0001";
    map['2'] = "0010";
    map['3'] = "0011";
    map['4'] = "0100";
    map['5'] = "0101";
    map['6'] = "0110";
    map['7'] = "0111";
    map['8'] = "1000";
    map['9'] = "1001";
    map['a'] = "1010";
    map['b'] = "1011";
    map['c'] = "1100";
    map['d'] = "1101";
    map['e'] = "1110";
    map['f'] = "1111";
    map['A'] = "1010";
    map['B'] = "1011";
    map['C'] = "1100";
    map['D'] = "1101";
    map['E'] = "1110";
    map['F'] = "1111";
    string out = "";
    int l = a.length();
    for (int i = 0; i < l; i++)
    {
        out += map[a[i]];
    }
    a = out;
}

void BtoH(string& a)
{
    unordered_map<string, string> map;
    map["0000"] = "0";
    map["0001"] = "1";
    map["0010"] = "2";
    map["0011"] = "3";
    map["0100"] = "4";
    map["0101"] = "5";
    map["0110"] = "6";
    map["0111"] = "7";
    map["1000"] = "8";
    map["1001"] = "9";
    map["1010"] = "A";
    map["1011"] = "B";
    map["1100"] = "C";
    map["1101"] = "D";
    map["1110"] = "E";
    map["1111"] = "F";
    string out = "";
    int l = a.length();
    for (int i = 0; i < l; i += 4)
    {
        out += map[a.substr(i, 4)];
    }
    a = out;
}

string SubWord(string& sw)
{
    string temp;
    for (int i = 0; i < 32; i += 8)
    {
        int col = (sw[i + 7] - '0') + (sw[i + 6] - '0') * 2 + (sw[i + 5] - '0') * 4 + (sw[i + 4] - '0') * 8;
        int row = (sw[i + 3] - '0') + (sw[i + 2] - '0') * 2 + (sw[i + 1] - '0') * 4 + (sw[i] - '0') * 8;
        string val = S_Box[row][col];
        HtoB(val);
        for (int j = 0; j < 8; ++j)
            temp += val[j];
    }
    return temp;
}

void KeyExpansion(string key, string k[44])
{
    HtoB(key);
    for (int i = 0; i < 4; i++)
    {
        k[i] = key.substr(32 * i, 32);
    }

    int i = 4;

    while (i < 44)
    {
        if (i % 4 == 0)
        {
            string temp;
            temp = k[i - 1];
            RotWord(temp);
            string b = Rcon[i / 4 - 1];
            HtoB(b);
            temp = SubWord(temp);
            temp = XOR(k[i - 4], temp);
            k[i] = XOR(temp, b);
        }
        else
            k[i] = XOR(k[i - 4], k[i - 1]);
        ++i;
    }
}



void SubBytes(string& mtx)
{
    string out;
    for (int i = 0; i < 16; ++i)
    {
        int col = (mtx[8 * i + 7] - '0') * 1 + (mtx[8 * i + 6] - '0') * 2 + (mtx[8 * i + 5] - '0') * 4 + (mtx[8 * i + 4] - '0') * 8;
        int row = (mtx[8 * i + 3] - '0') * 1 + (mtx[8 * i + 2] - '0') * 2 + (mtx[8 * i + 1] - '0') * 4 + (mtx[8 * i + 0] - '0') * 8;
        string val = S_Box[row][col];
        HtoB(val);
        out += val;
    }
    mtx = out;
}

void ShiftRows(string& mtx)
{
    string temp1 = mtx.substr(32, 32);
    RotWord(temp1);
    string temp2 = mtx.substr(64, 32);
    RotWord(temp2);
    RotWord(temp2);
    string temp3 = mtx.substr(96, 32);
    RotWord(temp3);
    RotWord(temp3);
    RotWord(temp3);
    string temp = mtx.substr(0, 32);
    mtx = temp + temp1 + temp2 + temp3;
}

string GFMul(string a, string b) 
{
    string x1b = "1b";
    HtoB(x1b);
    int l = a.length();
    string c, out;
    out = "00";
    HtoB(out);
    for (int i = 0; i < l; i++)
    {
        c = a;
        if (b[i] == '1')
        {
            for (int j = 0; j < (l - i - 1); j++)
            {
                c += '0';
                if (c[0] == '0')
                {
                    c = c.substr(1, c.length() - 1);
                }
                else
                {
                    c = c.substr(1, c.length() - 1);
                    c = XOR(c, x1b);
                }
            }
            out = XOR(out, c);
        }
    }
    return out;
}

void transpose(string& mtx)
{
    BtoH(mtx);
    string m;
    for (int i = 0; i < 32; i++)
    {
        m += mtx[i];
    }
    for (int i = 0; i < 4; i++)
    {
        mtx[8 * i + 0] = m[2 * i + 0];
        mtx[8 * i + 1] = m[2 * i + 1];
        mtx[8 * i + 2] = m[2 * i + 8];
        mtx[8 * i + 3] = m[2 * i + 9];
        mtx[8 * i + 4] = m[2 * i + 16];
        mtx[8 * i + 5] = m[2 * i + 17];
        mtx[8 * i + 6] = m[2 * i + 24];
        mtx[8 * i + 7] = m[2 * i + 25];
    }
    HtoB(mtx);
}

void MixColumns(string& mtx)
{
    string a = "02", b = "03";
    HtoB(a);
    HtoB(b);
    string out1, out2, out3, out4;
    transpose(mtx);
    for (int i = 0; i < 4; ++i)
    {
        out1 += XOR(XOR(XOR(GFMul(a, mtx.substr(32 * i + 0, 8)), GFMul(b, mtx.substr(32 * i + 8, 8))), mtx.substr(32 * i + 16, 8)), mtx.substr(32 * i + 24, 8));
        out2 += XOR(XOR(XOR(mtx.substr(32 * i + 0, 8), GFMul(a, mtx.substr(32 * i + 8, 8))), GFMul(b, mtx.substr(32 * i + 16, 8))), mtx.substr(32 * i + 24, 8));
        out3 += XOR(XOR(XOR(mtx.substr(32 * i + 0, 8), mtx.substr(32 * i + 8, 8)), GFMul(a, mtx.substr(32 * i + 16, 8))), GFMul(b, mtx.substr(32 * i + 24, 8)));
        out4 += XOR(XOR(XOR(GFMul(b, mtx.substr(32 * i + 0, 8)), mtx.substr(32 * i + 8, 8)), mtx.substr(32 * i + 16, 8)), GFMul(a, mtx.substr(32 * i + 24, 8)));
    }
    mtx = out1 + out2 + out3 + out4;
}

void AddRoundKey(string& mtx, string k[4])
{
    string a, b, c, d;
    for (int i = 0; i < 4; ++i)
    {
        a += XOR(mtx.substr(i * 8 + 0, 8), k[i].substr(0, 8));
        b += XOR(mtx.substr(i * 8 + 32, 8), k[i].substr(8, 8));
        c += XOR(mtx.substr(i * 8 + 64, 8), k[i].substr(16, 8));
        d += XOR(mtx.substr(i * 8 + 96, 8), k[i].substr(24, 8));
    }
    mtx = a + b + c + d;
}


void InvSubBytes(string& mtx)
{
    string out;
    for (int i = 0; i < 16; ++i)
    {
        int col = (mtx[8 * i + 7] - '0') * 1 + (mtx[8 * i + 6] - '0') * 2 + (mtx[8 * i + 5] - '0') * 4 + (mtx[8 * i + 4] - '0') * 8;
        int row = (mtx[8 * i + 3] - '0') * 1 + (mtx[8 * i + 2] - '0') * 2 + (mtx[8 * i + 1] - '0') * 4 + (mtx[8 * i + 0] - '0') * 8;
        string val = RS_Box[row][col];
        HtoB(val);
        out += val;
    }
    mtx = out;
}

void InvMixColumns(string& mtx)
{
    string a = "0e", b = "0b", c = "0d", d = "09";
    HtoB(a);
    HtoB(b);
    HtoB(c);
    HtoB(d);
    string out1, out2, out3, out4;
    transpose(mtx);
    for (int i = 0; i < 4; ++i)
    {
        out1 += XOR(XOR(XOR(GFMul(a, mtx.substr(32 * i + 0, 8)), GFMul(b, mtx.substr(32 * i + 8, 8))), GFMul(c, mtx.substr(32 * i + 16, 8))), GFMul(d, mtx.substr(32 * i + 24, 8)));
        out2 += XOR(XOR(XOR(GFMul(d, mtx.substr(32 * i + 0, 8)), GFMul(a, mtx.substr(32 * i + 8, 8))), GFMul(b, mtx.substr(32 * i + 16, 8))), GFMul(c, mtx.substr(32 * i + 24, 8)));
        out3 += XOR(XOR(XOR(GFMul(c, mtx.substr(32 * i + 0, 8)), GFMul(d, mtx.substr(32 * i + 8, 8))), GFMul(a, mtx.substr(32 * i + 16, 8))), GFMul(b, mtx.substr(32 * i + 24, 8)));
        out4 += XOR(XOR(XOR(GFMul(b, mtx.substr(32 * i + 0, 8)), GFMul(c, mtx.substr(32 * i + 8, 8))), GFMul(d, mtx.substr(32 * i + 16, 8))), GFMul(a, mtx.substr(32 * i + 24, 8)));
    }
    mtx = out1 + out2 + out3 + out4;
}


//Encrypts plaintext using aes with key
string AESencrypt(string iv, string pt)
{
    string w[44];
    while (pt.length() < 32)
    {
        pt = '0' + pt;
    }
    while (iv.length() < 32)
    {
        iv = '0' + iv;
    }

    KeyExpansion(iv, w);

    HtoB(pt);
    transpose(pt);
    
    string key[4];
    for (int i = 0; i < 4; ++i)
        key[i] = w[i];
    AddRoundKey(pt, key);

    for (int round = 1; round < 10; ++round)
    {
        SubBytes(pt);
        ShiftRows(pt);
        MixColumns(pt);
        for (int i = 0; i < 4; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(pt, key);
    }

    SubBytes(pt);
    ShiftRows(pt);
    for (int i = 0; i < 4; ++i)
        key[i] = w[4 * 10 + i];
    AddRoundKey(pt, key);
    
    transpose(pt);
    BtoH(pt);

    return pt;
}


////Decrypts ciphertext using aes with key
string AESdecrypt(string iv, string ct)
{
    string w[44];
    while (iv.length() < 32)
    {
        iv = '0' + iv;
    }

    //pt = "54776F204F6E65204E696E652054776F", key = "5468617473206D79204B756E67204675";
    KeyExpansion(iv, w);

    HtoB(ct);
    transpose(ct);

    string key[4];
    for (int i = 0; i < 4; ++i)
        key[i] = w[4 * 10 + i];
    AddRoundKey(ct, key);

    for (int round = 9; round > 0; --round)
    {
        ShiftRows(ct);
        ShiftRows(ct);
        ShiftRows(ct);
        InvSubBytes(ct);
        for (int i = 0; i < 4; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(ct, key);
        InvMixColumns(ct);
    }

    ShiftRows(ct);
    ShiftRows(ct);
    ShiftRows(ct);
    InvSubBytes(ct);
    for (int i = 0; i < 4; ++i)
        key[i] = w[i];
    AddRoundKey(ct, key);

    transpose(ct);
    BtoH(ct);

    return ct;
}
