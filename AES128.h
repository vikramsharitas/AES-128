#pragma once

#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include <unordered_map>

string AESencrypt(string iv, string pt);

string AESdecrypt(string iv, string ct);

void hex_toLL(string hex, long long int out);