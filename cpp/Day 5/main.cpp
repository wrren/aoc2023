#include <aoc/input.h>
#include <vector>
#include <map>
#include <iostream>
#include <regex>
#include <set>

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
    size_t start;
    size_t end;

    bool overlaps(size_t in) const
    {
        return in >= start && in < end;
    }

    bool overlaps(const range& other) const
    {
        return (other.start >= start && other.start < end) || (other.end >= start && other.end < end);
    }

    range constrain(const range& other) const
    {
        return range{ std::max(start, other.start), std::min(end, other.end) };
    }

    bool operator<(const range& other) const
    {
        return start < other.start;
    }
};

struct transform
{
    range from;
    range to;

    size_t apply(size_t in) const
    {
	    if(from.overlaps(in))
	    {
            in += (to.start - from.start);
	    }

        return in;
    }

    range apply(const range& other) const
    {
        range constrained = from.constrain(other);

        constrained.start += (to.start - from.start);
        constrained.end += (to.start - from.start);

        return constrained;
    }
};

struct resource_map
{
    resource_type           from;
    resource_type           to;
    std::vector<transform>  transforms;
};

struct almanac
{
    std::vector<size_t>         seeds;
    std::vector<resource_map>   maps;
};

size_t map_seed(const almanac& almanac, size_t seed)
{
    size_t current_value = seed;

    for (auto& map : almanac.maps)
    {
        for (const auto& transform : map.transforms)
        {
            if(transform.from.overlaps(current_value))
            {
                current_value = transform.apply(current_value);
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
        location = std::min(location, map_seed(almanac, seed));
    }

    return location;
}

void traverse(std::vector<range> range_queue, std::vector<resource_map>::const_iterator current_map, std::vector<resource_map>::const_iterator end, range& best_range)
{
    if(range_queue.empty())
    {
        return;
    }

    if(current_map == end)
    {
	    for(auto& range : range_queue)
	    {
		    if(range < best_range)
		    {
                best_range = range;
		    }
	    }

        return;
    }

    std::vector<range> new_queue;

    while(!range_queue.empty())
    {
        auto range = range_queue.back();
        range_queue.pop_back();

        for (auto& transform : current_map->transforms)
        {
            if (transform.from.overlaps(range))
            {
                auto new_range = transform.apply(range);
                new_queue.push_back(new_range);
            }
        }
    }

    for(auto& range : range_queue)
    {
        for(auto& transform : current_map->transforms)
        {
	        if(transform.from.overlaps(range))
	        {
                auto new_range = transform.apply(range);
                new_queue.push_back(new_range);
	        }
        }
    }

    traverse(new_queue, ++current_map, end, best_range);
}


size_t part_two(const almanac& almanac)
{
    std::vector <range> range_queue;
    range best_range{ std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };

    for(size_t i = 0; i < almanac.seeds.size(); i += 2)
    {
        range_queue.push_back({ almanac.seeds[i], almanac.seeds[i] + almanac.seeds[i + 1] });
    }

    traverse(range_queue, almanac.maps.begin(), almanac.maps.end(), best_range);
    
    return best_range.start;
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
                        transform new_transform{
                            { parsed_range[1], parsed_range[1] + parsed_range[2] },
                            { parsed_range[0], parsed_range[0] + parsed_range[2] },
                        };

                        current_almanac.maps[current_almanac.maps.size() - 1].transforms.push_back(new_transform);
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            }, current_almanac))
        {
            
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
