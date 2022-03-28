#include <iostream>
#include "TextAnalyzer.h"
int main()  
{
    std::vector<std::string> filenames
    {
        "text1.txt",
        "text2.txt"
    };
    TextAnalyzer txt;
    std::vector<std::thread> threads;
    for (const auto& file : filenames)
        threads.push_back(std::thread([&]() {
       txt.Frequency(file);
            }));
    for (size_t i = 0; i < threads.size(); ++i)
        if (threads.at(i).joinable())
            threads.at(i).join();
    int command;
    txt.OutMap();
    bool flag = true;
    while (flag)
    {
        std::cout << "Enter command:";
        std::cin >> command;
        switch (command)
        {
        case 0:
        {
            std::cout << "MostCommon:\n";
            for (const auto& it : txt.MostCommon())
                std::cout << "\t" << it.first << " " << it.second << std::endl;
            break;
        }
        case 1:
        {
            std::cout << "MostRare:\n";
            for (const auto& it : txt.MostRare())
                std::cout << "\t" << it.first << " " << it.second << std::endl;
            break;
        }
        case 2:
        {
            char symbol;

            std::cout << "Enter the symbol: " << std::flush;
            std::cin >> symbol;
            std::cout << txt.probability(symbol) << std::endl;
            break;
        }
        case 3:
            flag = false;
            break;
        default:
            break;
        }
    }
}
