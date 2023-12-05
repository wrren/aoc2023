#include <aoc/input.h>
#include <vector>
#include <map>
#include <iostream>
#include <regex>
#include <climits>

enum class resource_type
{
    seed,
    soil,
    fertilizer,
    water,
    light,
    temperature,
    humidity,
    location
};

struct range
{
    size_t destination;
    size_t source;
    size_t size;
};

struct resource_map
{
    resource_type       from;
    resource_type       to;
    std::vector<range>  ranges;
};

struct almanac
{
    std::vector<size_t>         seeds;
    std::vector<resource_map>   maps;
};

size_t map_seed(const almanac& almanac, size_t seed, resource_type resource)
{
    auto from_resource = resource_type::seed;

    size_t current_value = seed;

    for (auto& map : almanac.maps)
    {
        for (const auto& range : map.ranges)
        {
            if (current_value >= range.source && current_value < (range.source + range.size))
            {
                current_value += (range.destination - range.source);
                break;
            }
        }
    }

    return current_value;
}

size_t part_one(const almanac& almanac)
{
    size_t location = std::numeric_limits<size_t>::max();

    for (auto seed : almanac.seeds)
    {
        location = std::min(location, map_seed(almanac, seed, resource_type::location));
    }

    return location;
}

size_t local_minimum(const almanac& almanac, size_t start, size_t end)
{
    size_t location = std::numeric_limits<size_t>::max();

    while (true)
    {
        auto low    = map_seed(almanac, start, resource_type::location);
        auto high   = map_seed(almanac, end, resource_type::location);

        if (low < high)
        {
            if (low > location)
            {
                break;
            }
            end         = start + (end - start) / 2;;
            location    = low;
        }
        else if (high < low)
        {
            if (high > location)
            {
                break;
            }
            start       = start + (end - start) / 2;
            location    = low;
        }
        else if (low < location)
        {
            start = start + (end - start) / 2;
            location = low;
        }
        else if (start == end)
        {
            break;
        }
    }

    return location;
}

std::vector<range> find_path(const almanac& almanac, const range& start, std::vector<resource_type>::const_iterator begin, std::vector<resource_type>::const_iterator end)
{
    std::vector<range> path;



    return path;
}

size_t part_two(const almanac& almanac)
{
    size_t location = std::numeric_limits<size_t>::max();

    std::vector<range> path;

    for (auto& range : almanac.maps.rbegin()->ranges)
    {
        for (auto it = almanac.maps.rbegin() + 1; it != almanac.maps.rend(); ++it)
        {
            for (auto& r : it->ranges)
            {
                if((r.source <= range.source && r.destination >= range.source) || (r.destination >= range.destination && r.destination >= range.source)
            }
        }

        if (path.size() != almanac.maps.size())
        {
            path.clear();
        }
        else
        {
            break;
        }
    }

    
    
    return location;
}

int main(int argc, char** argv)
{
    std::map<std::string, resource_type> resource_type_map({
        {"seed",            resource_type::seed},
        {"soil",            resource_type::soil},
        {"fertilizer",      resource_type::fertilizer},
        {"water",           resource_type::water},
        {"light",           resource_type::light},
        {"temperature",     resource_type::temperature},
        {"humidity",        resource_type::humidity},
        {"location",        resource_type::location}
    });

    for (int i = 1; i < argc; i++)
    {
        almanac current_almanac;

        if (aoc::read_file_lines<almanac>(argv[i], [resource_type_map](const std::string& line, almanac& current_almanac)->bool
            {
                const std::regex map_regex("([a-z]+)-to-([a-z]+) map:");
                std::smatch map_match;

                if (current_almanac.seeds.size() == 0)
                {
                    const std::regex seeds_regex("seeds: ([0-9\\s]+)");
                    std::smatch seeds_match;
                    if (!std::regex_search(line, seeds_match, seeds_regex) || seeds_match.size() != 2)
                    {
                        return false;
                    }

                    current_almanac.seeds = aoc::split_transform<size_t>(seeds_match[1], " ", [](const std::string& seed_number)->size_t { return std::stoll(seed_number); });
                }
                else if(std::regex_search(line, map_match, map_regex) && map_match.size() == 3)
                {
                    auto from_resource = resource_type_map.find(map_match[1]);
                    auto to_resource    = resource_type_map.find(map_match[2]);
                    if (from_resource == resource_type_map.end() || to_resource == resource_type_map.end())
                    {
                        return false;
                    }

                    resource_map new_map{ from_resource->second, to_resource->second };
                    current_almanac.maps.push_back(new_map);
                }
                else if(line != "")
                {
                    auto parsed_range = aoc::split_transform<size_t>(line, " ", [](const std::string& seed_number)->size_t { return std::stoll(seed_number); });

                    if (parsed_range.size() == 3)
                    {
                        current_almanac.maps[current_almanac.maps.size() - 1].ranges.push_back({parsed_range[0], parsed_range[1], parsed_range[2]});
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            }, current_almanac))
        {
            for (auto& map : current_almanac.maps)
            {
                auto& ranges = map.ranges;

                std::sort(ranges.begin(), ranges.end(), [](const range& a, const range& b)->bool {
                    return a.destination < b.destination;
                });
            }

            std::cout << "Lowest Location (Part 1): " << part_one(current_almanac) << std::endl;
            std::cout << "Lowest Location (Part 2): " << part_two(current_almanac) << std::endl;
        }
        else
        {
            std::cerr << "failed to read file at path " << argv[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
