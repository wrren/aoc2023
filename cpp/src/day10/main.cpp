#include <aoc/input.h>
#include <iostream>
#include <set>
#include <numeric>

enum direction
{
    LEFT, RIGHT, UP, DOWN, ANY
};

direction opposite(direction in)
{
    switch(in)
    {
        case LEFT:  return RIGHT;
        case RIGHT: return LEFT;
        case UP:    return DOWN;
        case DOWN:  return UP;
        default:
            return ANY;
    }
}

struct route
{
    direction from;
    direction to;
};

struct node
{
    size_t              x;
    size_t              y;
    char                type;
    std::vector<route>  routes;
    size_t              distance    = std::numeric_limits<size_t>::max();
    bool                edge        = false;

    node() = default;

    node(char c, size_t x, size_t y) :
    x(x),
    y(y),
    type(c)
    {
        switch(type)
        {
            case 'J':
                routes = {{ LEFT, UP }, { UP, LEFT }};
            break;
            case 'L':
                routes = {{RIGHT, UP}, {UP, RIGHT}};
            break;
            case '7':
                routes = {{LEFT, DOWN}, {DOWN, LEFT}};
            break;
            case 'F':
                routes = {{RIGHT, DOWN}, {DOWN, RIGHT}};
            break;
            case '|':
                routes = {{UP, DOWN}, {DOWN, UP}};
            break;
            case '-':
                routes = {{LEFT, RIGHT}, {RIGHT, LEFT}};
            break;
            default:
                routes = {{ANY, RIGHT}, {ANY, LEFT}, {ANY, UP}, {ANY, DOWN}};
        }
    }

    bool is_pipe() const
    {
        return type != '.';
    }

    bool operator<(const node& other) const
    {
        return other.x == x ? y < other.y : x < other.x;
    }

    bool operator>(const node& other) const
    {
        return !(*this < other);
    }

    bool operator==(const node& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct map
{
    std::vector<std::vector<node>> nodes;

    const node& at(size_t x, size_t y) const
    {
        return nodes[y][x];
    }

    bool is_edge_node(const node& n) const
    {
        if(nodes.empty())
        {
            return false;
        }
        return n.x == 0 || n.y == 0 || n.x == (nodes[0].size() - 1) || n.y == (nodes.size() - 1);
    }

    node& at(const node& start, direction dir)
    {
        auto x = start.x;
        auto y = start.y;
        switch(dir)
        {
            case UP:    
                if(y == 0)
                {
                    throw std::invalid_argument("invalid direction specified");
                }
                return nodes[y-1][x];
            case DOWN:  
                if(y == (nodes.size() - 1))
                {
                    throw std::invalid_argument("invalid direction specified");
                }
                return nodes[y+1][x];
            case LEFT:  
                if(x == 0)
                {
                    throw std::invalid_argument("invalid direction specified");
                }
                return nodes[y][x-1];
            case RIGHT:
                if(x == (nodes[y].size() - 1))
                {
                    throw std::invalid_argument("invalid direction specified");
                }
                return nodes[y][x+1];
            default:
                throw std::invalid_argument("invalid direction specified");
        }
    }

    node* find_unevaluated(const std::set<node*> open, const std::set<node*> closed)
    {
        for(auto & l : nodes)
        {
            for (auto& n : l)
            {
                if(n.is_pipe() == false && open.find(&n) == open.end() && closed.find(&n) == closed.end())
                {
                    return &n;
                }
            }
        }

        return nullptr;
    }

    size_t flood()
    {
        std::set<node*> open, closed, evaluating, covered;
        node* start = find_unevaluated(open, closed);

        while(start != nullptr)
        {
            evaluating.clear();
            evaluating.insert(start);
            covered.clear();
            bool is_closed = true;
            while(!evaluating.empty())
            {
                auto current = *evaluating.begin();
                evaluating.erase(evaluating.begin());
                covered.insert(current);

                if(is_edge_node(*current))
                {
                    is_closed = false;
                }

                for(auto& r : current->routes)
                {
                    try
                    {
                        auto& n = at(*current, r.to);

                        if(!n.is_pipe() && covered.find(&n) == covered.end())
                        {
                            evaluating.insert(&n);
                        }
                    }
                    catch(std::invalid_argument& e)
                    {}
                }
            }

            if(is_closed)
            {
                for(auto e : covered)
                {
                    closed.insert(e);
                }
            }
            else
            {
                for(auto e : covered)
                {
                    open.insert(e);
                }
            }

            start = find_unevaluated(open, closed);
        }

        for(auto& l : nodes)
        {
            std::cout << std::endl;

            for(auto& n : l)
            {
                if(n.is_pipe())
                {
                    std::cout << n.type;
                }
                else
                {
                    if(open.find(&n) != open.end())
                    {
                        std::cout << 'O';
                    }
                    else if(closed.find(&n) != closed.end())
                    {
                        std::cout << 'I';
                    }
                    else
                    {
                        std::cout << n.type;
                    }
                }
            }
        }
        
        std::cout << std::endl;

        return closed.size();
    }

    bool find(char p, node& o) const
    {
        for(size_t i = 0; i < nodes.size(); i++)
        {
            auto& line = nodes[i];

            for(size_t j = 0; j < line.size(); j++)
            {
                if(line[j].type == p)
                {
                    o = line[j];
                    return true;
                }
            }
        }

        return false;
    }
};

struct path
{
    std::set<node>  travelled;
    node            location;
    direction       from;

    path() = default;

    path(node l, direction f) :
    location(l),
    from(f)
    {}

    path(const path& other) :
    travelled(other.travelled),
    location(other.location),
    from(other.from)
    {}

    bool next(map& m, size_t& max_distance)
    {
        for(auto& r : location.routes)
        {
            if(r.from == from || from == ANY)
            {
                try
                {
                    auto& n = m.at(location, r.to);

                    if(n.is_pipe() && travelled.find(n) == travelled.end())
                    {
                        n.distance = std::min(travelled.size() + 1, n.distance);

                        from            = opposite(r.to);
                        location        = n;
                        travelled.insert(n);
                    }
                    else if(n.type == 'S')
                    {
                        max_distance = std::max(max_distance, travelled.size());
                        return false;
                    }
                }
                catch(std::invalid_argument& e)
                {}
            }
        }

        return true;
    }

    bool operator<(const path& other) const
    {
        return travelled < other.travelled;
    }
    bool operator>(const path& other) const
    {
        return travelled > other.travelled;
    }
    bool operator==(const path& other) const
    {
        return travelled == other.travelled;
    }
};



int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        map m;
        auto success = aoc::read_file_lines(argv[i])
        .reduce<map>([](const std::string& line, map& out)->bool
        {
            auto y = out.nodes.size();
            std::vector<node> nodes;

            for(size_t x = 0; x < line.size(); x++)
            {
                nodes.push_back(node(line[x], x, y));
            }
            
            out.nodes.push_back(nodes);
            
            return true;
        }, m);

        path start(node(), ANY);

        if(success && m.find('S', start.location))
        {
            start.location.distance = 0;
            start.travelled.insert(start.location);
            size_t max_distance = 0;

            while(start.next(m, max_distance))
            {
                
            }

            std::cout << "Max Distance (Part 1): " << max_distance / 2 << std::endl;
            std::cout << "Enclosed Tiles (Part 2): " << m.flood() << std::endl;
        }
        else
        {
            std::cerr << "failed to read file " << argv[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}