#include <aoc/input.h>
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>

struct sequence
{
    std::vector<int64_t>        steps;
    sequence*                   parent;
    std::shared_ptr<sequence>   child;

    std::shared_ptr<sequence> next()
    {
        auto n = std::make_shared<sequence>();

        for(size_t i = 1; i < steps.size(); i++)
        {
            n->steps.push_back(steps[i] - steps[i-1]);
        }

        child = n;
        n->parent = this;

        return n;
    }

    sequence& append()
    {
        steps.push_back(steps.back());
        return *this;
    }

    sequence& predict_back(const sequence& child)
    {
        auto back = steps.back();
        steps.push_back(back + child.steps.back());
        return *this;
    }

    sequence& predict_front(const sequence& child)
    {
        auto front = steps.front();
        
        steps.insert(steps.begin(), front - child.steps.front());
        return *this;
    }

    bool is_zeroes() const
    {
        for(auto& e : steps)
        {
            if(e != 0)
            {
                return false;
            }
        }
        return true;
    }
};

int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        auto sequences = aoc::read_file_lines(argv[i])
        .map<sequence>([](const std::string& line){ 
            return sequence{ aoc::split_transform<int64_t>(line, " ", [](const std::string& in){ return std::stoll(in); }) }; 
        });

        int64_t part1_sum = 0;
        int64_t part2_sum = 0;

        for(auto& s : sequences)
        {
            auto child = s.next().get();

            while(child->is_zeroes() == false)
            {
                child = child->next().get();
            }

            auto parent = child->parent;

            while(parent != nullptr)
            {
                parent->predict_back(*child);
                parent->predict_front(*child);
                child   = parent;
                parent  = child->parent;
            }

            part1_sum += s.steps.back();
            part2_sum += s.steps.front();
        }

        std::cout << "Sum (Part 1): " << part1_sum << std::endl;
        std::cout << "Sum (Part 2): " << part2_sum << std::endl;
    }

    return EXIT_SUCCESS;
}