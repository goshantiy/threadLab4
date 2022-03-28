#pragma once
#include <thread>
#include <vector>
#include <string>
#include <regex>
#include <mutex>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
class TextAnalyzer
{
private:
    mutable std::mutex _mutex;
    std::map<char, size_t> _frequencySymbols;
public:
    TextAnalyzer() = default;
	std::string getText(std::string& name);
    void Frequency(std::string file);
    std::vector<std::pair<char, size_t>> MostCommon();
    std::vector<std::pair<char, size_t>> MostRare();    
    double probability(char&);
    void OutMap();
};
void TextAnalyzer::OutMap()
{
    for (auto& it : _frequencySymbols)
        std::cout << it.first <<" " << it.second << "\n";
}
std::string TextAnalyzer::getText(std::string& fileName)
{
    try {
        std::lock_guard<std::mutex> guard(_mutex);

        std::ifstream in(fileName);

        std::string data;

        data.assign((std::istreambuf_iterator<char>(in.rdbuf())), std::istreambuf_iterator<char>());

        return data;
    }
    catch (...) {
        return std::string();
    }
}
void TextAnalyzer::Frequency(std::string file)
{
    std::string text = getText(file);

    for (auto &it: text)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        ++_frequencySymbols[it];
    }
}
std::vector<std::pair<char, size_t>> TextAnalyzer::MostCommon()
{
    std::lock_guard<std::mutex> guard(_mutex);

    std::set<std::pair<size_t, char>> flip;

    for (auto const& kv : _frequencySymbols)
        flip.emplace(kv.second, kv.first);

    std::vector<std::pair<char, size_t>> result;

    int i = 0;

    for (auto it = flip.rbegin(); it != flip.rend() && i < 5; ++it, ++i)
        result.push_back({ it->second, it->first });

    return result;
}
std::vector<std::pair<char, size_t>> TextAnalyzer::MostRare()
{
    std::lock_guard<std::mutex> guard(_mutex);

    std::set<std::pair<size_t, char>> flip;

    for (auto const& kv : _frequencySymbols)
        flip.emplace(kv.second, kv.first);

    std::vector<std::pair<char, size_t>> result;

    int i = 0;

    for (auto it = flip.begin(); it != flip.end() && i < 3; ++it, ++i)
        result.push_back({ it->second, it->first });

    return result;
}
double TextAnalyzer::probability(char& symbol)
{
    size_t sum = 0;

    for (const auto& it : _frequencySymbols)
        //if (it.first != symbol)
            sum += it.second;

    if (sum == 0)
        return 0;
    std::cout<<_frequencySymbols[symbol]<<"/"<< sum << "=";
   
    return _frequencySymbols[symbol] / static_cast<double>(sum);
}