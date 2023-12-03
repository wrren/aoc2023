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
}