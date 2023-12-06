#include <aoc/input.h>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <cmath>

struct race
{
    double time;
    double distance;

    size_t win_permutations() const
    {
        auto can_win = [this](double hold)->bool
        {
            return (hold * (time - hold)) > distance;
        };
        
        double min_hold = std::numeric_limits<double>::max();
        double max_hold = 0;
        std::deque<std::pair<double, double>> evaluate;
        evaluate.push_back(std::pair<double, double>(0, time));

        while (!evaluate.empty())
        {
            auto next = evaluate.back();
            evaluate.pop_back();

            auto pivot = std::ceil((next.second - next.first) / 2) + next.first;
            if (pivot != next.first && pivot != next.second)
            {
                auto wins = can_win(pivot);

                if (wins && pivot > max_hold)
                {
                    max_hold = pivot;
                }
                
                if (wins && pivot < min_hold)
                {
                    min_hold = pivot;
                }

                if (wins || can_win(next.first))
                {
                    evaluate.push_back(std::pair<double, double>(next.first, pivot));
                }
                if (wins || can_win(next.second))
                {
                    evaluate.push_back(std::pair<double, double>(pivot, next.second));
                }
            }
        }

        return static_cast<size_t>((max_hold - min_hold) + 1);
    }
};

typedef std::vector<race> race_list;

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        race_list races;

        if (aoc::read_file_lines<race_list>(argv[i], [](const std::string& line, race_list& list)->bool
            {
                auto segments = aoc::split(line, ":");
                if (segments.size() != 2)
                {
                    return false;
                }

                auto values = aoc::split_transform<double>(segments[1], " ", [](const std::string& value) { return std::stod(value);  });

                if (list.size() < values.size())
                {
                    list.resize(values.size());
                }

                if (segments[0] == "Time")
                {
                    for (size_t i = 0; i < values.size(); i++)
                    {
                        list[i].time = values[i];
                    }
                }
                else if (segments[0] == "Distance")
                {
                    for (size_t i = 0; i < values.size(); i++)
                    {
                        list[i].distance = values[i];
                    }
                }
                else
                {
                    return false;
                }

                return true;
            }, races))
        {
            size_t permutation_multiple = 1;

            for (auto& race : races)
            {
                permutation_multiple *= race.win_permutations();
            }

            std::cout << "Win Permutation Multiple (Part 1): " << permutation_multiple << std::endl;

            race part_two;

            std::stringstream times, distances;

            for (auto& race : races)
            {
                times << race.time;
                distances << race.distance;
            }

            times >> part_two.time;
            distances >> part_two.distance;

            std::cout << "Win Permutation Multiple (Part 2): " << part_two.win_permutations() << std::endl;
        }
        else
        {
            std::cerr << "Failed to open or parse file " << argv[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
