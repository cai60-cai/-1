// bm.h
#ifndef BM_H
#define BM_H

#include <string>
#include <unordered_map>
#include <vector>

void buildBadCharTable(const std::string &pattern, std::unordered_map<char, int> &badCharTable);
void buildGoodSuffixTable(const std::string &pattern, std::vector<int> &goodSuffixTable);
int BMSearch(const std::string &text, const std::string &pattern);

#endif
