#include <aoc/input.h>
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <cmath>

struct card
{
    size_t id;
    size_t count;
    size_t winning_numbers;
};

typedef std::vector<card> card_list;

size_t part_one(const card_list& cards)
{
    size_t points = 0;

    for (auto& card : cards)
    {
        if (card.winning_numbers > 0)
        {
            points += static_cast<size_t>(std::pow(2, card.winning_numbers - 1));
        }
    }

    return points;
}

size_t part_two(card_list& cards)
{
    size_t card_count = 0;

    for (size_t i = 0; i < cards.size(); i++)
    {
        auto& card = cards[i];

        while (card.count > 0)
        {
            card.count--;
            card_count++;
            size_t increment = i + 1 + card.winning_numbers;

            for (size_t j = i + 1; j < increment; j++)
            {
                cards[j].count++;
            }
        }
    }

    return card_count;
}

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
    {
        std::filesystem::path path = argv[i];
        card_list cards;
        
        size_t points = 0;

        if (aoc::read_file_lines<card_list>(path, [](const std::string& line, card_list& cards)->bool
        {
            auto colon = line.find(":");
            if (colon == std::string::npos)
            {
                return false;
            }

            auto pipe = line.find("|", colon + 1);
            if (pipe == std::string::npos)
            {
                return false;
            }

            auto transform = [](const std::string& s) { return std::stol(s); };

            auto winners = aoc::split_transform<long>(line.substr(colon + 1, pipe - colon - 1), " ", transform);
            auto choices = aoc::split_transform<long>(line.substr(pipe + 1), " ", transform);

            auto winners_set = std::set<long>(winners.begin(), winners.end());
            auto choices_set = std::set<long>(choices.begin(), choices.end());

            std::vector<long> intersection;
            std::set_intersection(winners_set.begin(), winners_set.end(), choices_set.begin(), choices_set.end(), std::back_inserter(intersection));

            cards.push_back({
                cards.size() + 1,
                1,
                intersection.size()
            });

            return true;
        }, cards))
        {
            size_t points       = part_one(cards);
            size_t card_count   = part_two(cards);

            std::cout << "Points: " << points << std::endl;
            std::cout << "Cards: " << card_count << std::endl;
        }
        else
        {
            std::cerr << "Error while reading file " << path << std::endl;
        }
    }
}