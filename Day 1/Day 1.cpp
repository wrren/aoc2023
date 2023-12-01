#include <iostream>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <optional>
#include <vector>

std::optional<size_t> PartOne(const std::filesystem::path& Path)
{
    size_t Sum = 0;

    std::ifstream FileStream(Path, std::ios::in);

    if (!FileStream.is_open())
    {
        std::cout << "Path " << Path << " could not be opened." << std::endl;
        return {};
    }

    std::string Line;

    while (getline(FileStream, Line))
    {
        std::stringstream Digits;
        auto FirstDigit = Line.find_first_of("0123456789");
        auto LastDigit = Line.find_last_of("0123456789");

        if (FirstDigit == std::string::npos || LastDigit == std::string::npos)
        {
            std::cout << "Line " << Line << " does not contain at least two digits. Exiting..." << std::endl;
            return {};
        }

        Digits << Line[FirstDigit] << Line[LastDigit];
        int Number;
        Digits >> Number;

        Sum += Number;
    }

    return Sum;
}

std::optional<size_t> PartTwo(const std::filesystem::path& Path)
{
    size_t Sum = 0;

    static std::vector<std::string> NumberWords = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };

    std::ifstream FileStream(Path, std::ios::in);

    if (!FileStream.is_open())
    {
        std::cout << "Path " << Path << " could not be opened." << std::endl;
        return {};
    }

    std::string Line;

    while (getline(FileStream, Line))
    {
        size_t FirstDigitPosition   = std::string::npos;
        size_t LastDigitPosition    = 0;
        size_t FirstDigit           = 0;
        size_t FirstDigitIndex      = std::string::npos;
        size_t LastDigit            = 0;
        size_t LastDigitIndex       = std::string::npos;

        for (size_t i = 0; i < NumberWords.size(); i++)
        {
            auto& NumberWord = NumberWords[i];

            auto Position = Line.find(NumberWord);
            if (Position != std::string::npos && Position < FirstDigitPosition)
            {
                FirstDigitPosition = Position;
                FirstDigitIndex = i;
            }

            Position = Line.rfind(NumberWord);
            if (Position != std::string::npos && Position >= LastDigitPosition)
            {
                LastDigitPosition = Position;
                LastDigitIndex = i;
            }
        }

        if (FirstDigitIndex == std::string::npos || LastDigitIndex == std::string::npos)
        {
            return {};
        }

        auto Number = ((FirstDigitIndex % 10) * 10) + (LastDigitIndex % 10);
        Sum += Number;
    }

    return Sum;
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++)
    {
        std::filesystem::path Path = argv[i];

        if (!std::filesystem::exists(Path))
        {
            std::cout << "Path " << Path << " could not be opened." << std::endl;
            continue;
        }

        auto Sum = PartOne(Path);

        if (Sum.has_value())
        {
            std::cout << "Input " << Path << ", Part One Sum: " << *Sum << std::endl;
        }
        else
        {
            std::cout << "Failed to get value for path " << Path << std::endl;
        }

        Sum = PartTwo(Path);

        if (Sum.has_value())
        {
            std::cout << "Input " << Path << ", Part Two Sum : " << *Sum << std::endl;
        }
        else
        {
            std::cout << "Failed to get value for path " << Path << std::endl;
        }

    }

    return EXIT_SUCCESS;
}
