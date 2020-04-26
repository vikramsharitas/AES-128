#pragma once

#include <iostream>
#include <bitset>
#include <sstream>
#include <string>
#include <unordered_map>

//void KeyExpansion(std::string key, std::string k[44]);

std::string AESencrypt(std::string iv, std::string pt);

std::string AESdecrypt(std::string iv, std::string ct);
