#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <functional>

namespace aoc
{
	template<typename T>
	bool read_file_lines(const std::filesystem::path& path, std::function<bool (const std::string&, T&)> reducer, T& initial = {})
	{
        std::ifstream stream(path, std::ios::in);

        if (!stream.is_open())
        {
            return false;
        }

        std::string line;

        while (getline(stream, line))
        {
            if(!reducer(line, initial))
            {
                return false;
            }
        }

        return true;
	}

    std::string trim(const std::string& input, const std::string& delimiter = " ")
    {
        auto start  = input.find_first_not_of(delimiter);
        auto end    = input.find_last_not_of(delimiter);

        if(start == std::string::npos)
        {
            return "";
        }
        if(end == std::string::npos)
        {
            return input.substr(start);
        }

        return input.substr(start, end);
    }

    std::vector<std::string> split(const std::string& input, const std::string& delimiter)
    {
        std::vector<std::string> split;

        auto start = input.find_first_not_of(delimiter);

        while (start != std::string::npos)
        {
            auto end = input.find_first_of(delimiter, start + 1);

            if (end != std::string::npos)
            {
                split.push_back(input.substr(start, end - start));
            }
            else
            {
                split.push_back(input.substr(start));
                break;
            }

            start = input.find_first_not_of(delimiter, end + 1);
        }

        return split;
    }

    template<typename T>
    std::vector<T> split_transform(const std::string& input, const std::string& delimiter, std::function<T(const std::string&)> transformer)
    {
        std::vector<T> split;

        auto start = input.find_first_not_of(delimiter);

        while (start != std::string::npos)
        {
            auto end = input.find_first_of(delimiter, start + 1);

            if (end != std::string::npos)
            {
                split.push_back(transformer(input.substr(start, end - start)));
            }
            else
            {
                split.push_back(transformer(input.substr(start)));
                break;
            }

            start = input.find_first_not_of(delimiter, end + 1);
        }

        return split;
    }
}