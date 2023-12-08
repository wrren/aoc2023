#include <aoc/input.h>
#include <string>
#include <map>
#include <iostream>
#include <regex>
#include <numeric>

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

size_t count_steps_to(const map& m, std::map<std::string, node>::const_iterator start, std::function<bool(const node&)> evaluator)
{
    size_t steps            = 0;
    size_t direction_index  = 0;

    while(!evaluator(start->second))
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
    
    return count_steps_to(m, start, [](const node& n){ return n.name == "ZZZ"; });
}


size_t part_two(const map& m)
{
    std::vector<size_t> steps;
    for(auto it = m.lookup.begin(); it != m.lookup.end(); ++it)
    {
        if(it->second.ends_with_a)
        {
            steps.push_back(count_steps_to(m, it, [](const node& n){ return n.name[2] == 'Z'; }));
        }
    }

    size_t lcm = std::lcm(steps[0], steps[1]);

    for(size_t i = 2; i < steps.size(); i++)
    {
        lcm = std::lcm(lcm, steps[i]);
    }

    return lcm;
}

int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        auto m = aoc::read_file_lines(argv[i])
        .reduce<map>([](const std::string& line, const map& m)->map
        {
            map o(m);
            const std::regex reg("([A-Z])");

            if(m.directions.empty())
            {
                o.directions = line;
            }
            else if(!line.empty())
            {
                const std::regex reg("([A-Z]{3}) = \\(([A-Z]{3}), ([A-Z]{3})\\)");
                std::smatch match;
                
                if(!std::regex_search(line, match, reg))
                {
                    throw std::invalid_argument("unrecognized line format");
                }

                node n{ match[1].str(), match[2].str(), match[3].str(), match[1].str()[2] == 'A', match[1].str()[2] == 'Z' };
                o.lookup[n.name] = n;
            }

            return o;
        });

        std::cout << "Steps (Part 1): " << part_one(m) << std::endl;
        std::cout << "Steps (Part 2): " << part_two(m) << std::endl;
    }

    return EXIT_SUCCESS;
}