#include <unordered_map>
#include <vector>

void buildBadCharTable(const std::string &pattern, std::unordered_map<char, int> &badCharTable)
{
    int m = pattern.size();
    for (int i = 0; i < m - 1; ++i)
    {
        badCharTable[pattern[i]] = i;
    }
}

void buildGoodSuffixTable(const std::string &pattern, std::vector<int> &goodSuffixTable)
{
    int m = pattern.size();
    std::vector<int> border(m + 1, 0);
    int i = m, j = m + 1;
    border[i] = j;

    while (i > 0)
    {
        while (j <= m && pattern[i - 1] != pattern[j - 1])
        {
            if (goodSuffixTable[j] == 0)
            {
                goodSuffixTable[j] = j - i;
            }
            j = border[j];
        }
        --i;
        --j;
        border[i] = j;
    }

    j = border[0];
    for (i = 0; i <= m; ++i)
    {
        if (goodSuffixTable[i] == 0)
        {
            goodSuffixTable[i] = j;
        }
        if (i == j)
        {
            j = border[j];
        }
    }
}

int BMSearch(const std::string &text, const std::string &pattern)
{
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return -1;

    std::unordered_map<char, int> badCharTable;
    std::vector<int> goodSuffixTable(m + 1, 0);

    buildBadCharTable(pattern, badCharTable);
    buildGoodSuffixTable(pattern, goodSuffixTable);

    int s = 0; // s是文本和模式对齐的起始位置
    while (s <= (n - m))
    {
        int j = m - 1;

        // 从右向左匹配
        while (j >= 0 && pattern[j] == text[s + j])
        {
            --j;
        }

        // 匹配成功
        if (j < 0)
        {
            return s;
        }
        else
        {
            // 使用坏字符和好前缀规则进行移动
            char badChar = text[s + j];
            int badCharShift = badCharTable.count(badChar) ? std::max(1, j - badCharTable[badChar]) : j + 1;
            int goodSuffixShift = goodSuffixTable[j + 1];
            s += std::max(badCharShift, goodSuffixShift);
        }
    }
    return -1;
}

