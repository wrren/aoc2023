#include <aoc/input.h>
#include <string>
#include <map>
#include <iostream>
#include <regex>

struct node
{
    std::string name;
    std::string left;
    std::string right;
    bool ends_with_a;
    bool ends_with_z;
};

struct map
{
    std::string                     directions;
    std::map<std::string, node>     lookup;
};

bool parse_line(const std::string& line, map& m)
{
    const std::regex reg("([A-Z])");

    if(m.directions.empty())
    {
        m.directions = line;
    }
    else if(!line.empty())
    {
        const std::regex reg("([A-Z]{3}) = \\(([A-Z]{3}), ([A-Z]{3})\\)");
        std::smatch match;
        
        if(!std::regex_search(line, match, reg))
        {
            return false;
        }

        node n{ match[1].str(), match[2].str(), match[3].str(), match[1].str()[2] == 'A', match[1].str()[2] == 'Z' };
        m.lookup[n.name] = n;
    }

    return true;
}

size_t part_one(const map& m)
{
    auto start              = m.lookup.find("AAA");
    auto end                = m.lookup.find("ZZZ");
    auto steps              = 0;
    size_t direction_index  = 0;


    while(start != end)
    {
        auto direction = m.directions[direction_index % m.directions.size()];

        if(direction == 'R')
        {
            start = m.lookup.find(start->second.right);
        }
        else
        {
            start = m.lookup.find(start->second.left);
        }

        ++steps;
        ++direction_index;
    }

    return steps;
}

size_t part_two(const map& m)
{
    std::vector<__decltype(m.lookup)::const_iterator> current_nodes;
    for(auto it = m.lookup.begin(); it != m.lookup.end(); ++it)
    {
        if(it->second.ends_with_a)
        {
            current_nodes.push_back(it);
        }
    }

    size_t steps = 0;
    size_t direction_index  = 0;

    while(true)
    {
        auto direction = m.directions[direction_index % m.directions.size()];

        size_t ends_with_z = 0;

        for(size_t i = 0; i < current_nodes.size(); i++)
        {
            __decltype(m.lookup)::const_iterator next;

            if(direction == 'R')
            {
                next = m.lookup.find(current_nodes[i]->second.right);
            }
            else
            {
                next = m.lookup.find(current_nodes[i]->second.left);
            }

            if(next->second.ends_with_z)
            {
                ++ends_with_z;
            }

            current_nodes[i] = next;
        }

        ++steps;
        ++direction_index;

        if(ends_with_z == current_nodes.size())
        {
            break;
        }
    }

    return steps;
}

int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        map m;

        if(aoc::read_file_lines<map>(argv[i], &parse_line, m))
        {
            std::cout << "Steps (Part 1): " << part_one(m) << std::endl;
            std::cout << "Steps (Part 2): " << part_two(m) << std::endl;
        }
        else
        {
            std::cerr << "Failed to parse file " << argv[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}