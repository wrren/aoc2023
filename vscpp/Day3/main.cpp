#include <aoc/input.h>
#include <vector>
#include <string>
#include <iostream>
#include <set>

struct pivot
{
    size_t  x;
    size_t  y;
    char    type;
};

struct engine_schematic
{
    std::vector<std::string>    map;
    std::vector<pivot>          pivots;
    std::vector<int>            engine_parts;
};

#define IS_ENGINE_PART_CHARACTER(c) (c >= '0' && c <= '9')

bool form_part_number(const engine_schematic& schematic, size_t x, size_t y, size_t& part_number)
{
    auto line   = schematic.map[y];
    size_t start = x;
    size_t end = x + 1;
    std::stringstream   prefix, suffix;

    if(IS_ENGINE_PART_CHARACTER(line[x]))
    {
        for(int i = x; i >= 0; i--)
        {
	        if(IS_ENGINE_PART_CHARACTER(line[i]))
	        {
                start = i;
	        }
            else
            {
                break;
            }
        }
        for(int i = x; i < line.length(); i++)
        {
            if (IS_ENGINE_PART_CHARACTER(line[i]))
            {
                end = i+1;
            }
            else
            {
                break;
            }
        }

        auto part = line.substr(start, end - start);
        part_number = std::atol(part.c_str());
        return true;
    }

    return false;
}

int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        engine_schematic schematic;
        
        if(!aoc::read_file_lines<engine_schematic>(argv[1], [](const std::string& line, engine_schematic& schematic)->bool
        {
            schematic.map.push_back(line);
            size_t y = schematic.map.size() - 1;
            size_t offset = 0;
            size_t position = line.find_first_not_of(".0123456789", offset);
            
            while(position != std::string::npos)
            {
				schematic.pivots.push_back({ position, y, line[position]});
                offset = position + 1;
                position = line.find_first_not_of(".0123456789", offset);
            }
            
            return true;
        }, schematic))
        {
            std::cerr << "failed to read file " << argv[i] << std::endl;
        }

        size_t sum = 0;
        size_t gear_sums = 0;

        for(auto& pivot : schematic.pivots)
        {
            std::set<int> part_numbers;

	        for(int x = -1; x <= 1; x++)
	        {
		        for(int y = -1; y <= 1; y++)
		        {
                    if(x == 0 && y == 0)
                    {
                        continue;
                    }

                    size_t part_number;
                    
                    if(form_part_number(schematic, pivot.x + x, pivot.y + y, part_number))
                    {
                        part_numbers.insert(part_number);
                    }
		        }
	        }

            for (auto& pn : part_numbers)
            {
                schematic.engine_parts.push_back(pn);
            }

            if(pivot.type == '*' && part_numbers.size() == 2)
            {
                size_t ratio = 1;
                for (auto& pn : part_numbers)
                {
                    ratio *= pn;
                }
                gear_sums += ratio;
            }
        }

        for(auto& part_number : schematic.engine_parts)
        {
            sum += part_number;
        }

        std::cout << "part number sum " << sum << std::endl;
        std::cout << "gear ratio sum " << gear_sums << std::endl;

    }
}