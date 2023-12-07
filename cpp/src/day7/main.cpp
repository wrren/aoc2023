#include <aoc/input.h>
#include <cstdlib>
#include <map>
#include <iostream>

const char order[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

struct card
{
    char    c;
    size_t  strength;
    size_t  count;
    bool    is_wildcard;

    card() = default;

    card(char c) :
    is_wildcard(c == 'J')
    {
        for(size_t i = 0; i < sizeof(order) / sizeof(order[0]); i++)
        {
            if(c == order[i])
            {
                this->strength  = i + 1;
                this->c         = c;
                this->count     = 1;
            }
        }
    }
};

struct hand
{
    enum class hand_type
    {
        high_card,
        one_pair,
        two_pair,
        three_of_a_kind,
        full_house,
        four_of_a_kind,
        five_of_a_kind
    };

    enum class system
    {
        part1,
        part2
    };

    std::map<char, card>    cards;
    std::vector<card>       card_order;
    std::string             card_string;
    hand_type               type;
    system                  strength_system;

    hand(const std::string& card_string) :
    card_string(card_string)
    {
        card_order.reserve(5);
        
        for(auto c : card_string)
        {
            auto entry = cards.find(c);
            if(entry == cards.end())
            {
                cards[c] = card(c);
            }
            else
            {
                entry->second.count++;
            }
            
            card_order.push_back(c);
        }
    }

    static hand_type determine_strength_part1(const std::map<char, card> cards)
    {
        hand_type type = hand_type::high_card;
        auto found_pairs        = 0;
        bool found_three        = false;

        for(auto& c : cards)
        {
            switch(c.second.count)
            {
                case 5:
                    type = hand_type::five_of_a_kind;
                    break;
                case 4:
                    type = hand_type::four_of_a_kind;
                    break;
                case 3:
                    found_three = true;
                    break;
                case 2:
                    found_pairs++;
                    break;
            }
        }

        if(found_three && found_pairs == 1)
        {
            type = hand_type::full_house;
        }
        else if(found_three)
        {
            type = hand_type::three_of_a_kind;
        }
        else if(found_pairs == 2)
        {
            type = hand_type::two_pair;
        }
        else if(found_pairs == 1)
        {
            type = hand_type::one_pair;
        }

        return type;
    }

    static hand_type determine_strength_part2(const std::map<char, card> cards)
    {
        auto wildcards = cards.find('J');
        auto wildcard_count = 0;

        if(wildcards != cards.end())
        {
            wildcard_count = wildcards->second.count;
        }

        hand_type type = hand_type::high_card;
        auto found_pairs        = 0;
        bool found_three        = false;

        for(auto& c : cards)
        {
            switch(c.second.count)
            {
                case 5:
                    return hand_type::five_of_a_kind;
                    break;
                case 4:
                    if(!c.second.is_wildcard && wildcard_count == 1)
                    {
                        return hand_type::five_of_a_kind;
                    }
                    else if(!c.second.is_wildcard)
                    {
                        return hand_type::four_of_a_kind;
                    }
                    break;
                case 3:
                    if(!c.second.is_wildcard && wildcard_count == 2)
                    {
                        return hand_type::five_of_a_kind;
                    }
                    else if(!c.second.is_wildcard && wildcard_count == 1)
                    {
                        return hand_type::four_of_a_kind;
                    }
                    else if(!c.second.is_wildcard)
                    {
                        found_three = true;
                    }
                    break;
                case 2:
                    if(!c.second.is_wildcard && wildcard_count == 3)
                    {
                        return hand_type::five_of_a_kind;
                    }
                    else if(!c.second.is_wildcard && wildcard_count == 2)
                    {
                        return hand_type::four_of_a_kind;
                    }
                    else if(!c.second.is_wildcard && wildcard_count == 1)
                    {
                        wildcard_count = 0;
                        found_three = true;
                    }
                    else if(!c.second.is_wildcard)
                    {
                        found_pairs++;
                    }
                    break;
            }
        }

        if(found_three && found_pairs == 1)
        {
            type = hand_type::full_house;
        }
        else if(found_three)
        {
            type = hand_type::three_of_a_kind;
        }
        else if(found_pairs == 2)
        {
            type = hand_type::two_pair;
        }
        else if(found_pairs == 2 && wildcard_count == 1)
        {
            type = hand_type::full_house;
        }
        else if(found_pairs == 1)
        {
            type = hand_type::one_pair;
        }

        if(type == hand_type::high_card)
        {
            switch(wildcard_count)
            {
                case 4: return hand_type::five_of_a_kind;
                case 3: return hand_type::four_of_a_kind;
                case 2: return hand_type::three_of_a_kind;
                case 1: return hand_type::one_pair;
            }
        }

        return type;
    }

    size_t wildcard_count() const
    {
        auto wildcards = cards.find('J');

        if(wildcards != cards.end())
        {
            return wildcards->second.count;
        }

        return 0;
    }

    bool operator<(const hand& other) const
    {
        if(strength_system == system::part1)
        {
            if(other.type == type)
            {
                for(size_t i = 0; i < 5; i++)
                {
                    auto our_strength   = card_order[i].strength;
                    auto their_strength = other.card_order[i].strength;

                    if(our_strength < their_strength)
                    {
                        return true;
                    }
                    else if(our_strength > their_strength)
                    {
                        return false;
                    }
                }
            }
            else
            {
                return static_cast<int>(type) < static_cast<int>(other.type);
            }
        }
        else
        {
            if(other.type == type)
            {
                for(size_t i = 0; i < 5; i++)
                {
                    auto our_strength   = card_order[i].c == 'J' ? 0 : card_order[i].strength;
                    auto their_strength = other.card_order[i].c == 'J' ? 0 : other.card_order[i].strength;

                    if(our_strength < their_strength)
                    {
                        return true;
                    }
                    else if(our_strength > their_strength)
                    {
                        return false;
                    }
                }
            }
            else
            {
                return static_cast<int>(type) < static_cast<int>(other.type);
            }
        }
        

        return false;
    }
};

struct bet
{
    hand    the_hand;
    size_t  amount;

    bet(const std::string& cards_string, size_t amount) :
    the_hand(cards_string),
    amount(amount)
    {}

    bool operator<(const bet& other) const
    {
        return the_hand < other.the_hand;
    }
};

bool parse_line(const std::string& line, std::vector<bet>& bets)
{
    auto components = aoc::split(line, " ");
    bet new_bet(components[0], std::stoll(components[1]));

    bets.push_back(new_bet);

    return true;
}

int main(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        std::vector<bet> bets;

        if(aoc::read_file_lines<std::vector<bet>>(argv[i], &parse_line, bets))
        {
            for(auto& bet : bets)
            {
                bet.the_hand.type = hand::determine_strength_part1(bet.the_hand.cards);
                bet.the_hand.strength_system = hand::system::part1;
            }

            std::sort(bets.begin(), bets.end());
            size_t total_winnings = 0;

            for(size_t i = 0; i < bets.size(); i++)
            {
                total_winnings += bets[i].amount * (i + 1);
            }

            std::cout << "Total Winnings (Part 1): " << total_winnings << std::endl;

            for(auto& bet : bets)
            {
                bet.the_hand.type = hand::determine_strength_part2(bet.the_hand.cards);
                bet.the_hand.strength_system = hand::system::part2;
            }       

            std::sort(bets.begin(), bets.end());
            total_winnings = 0;

            for(size_t i = 0; i < bets.size(); i++)
            {
                total_winnings += bets[i].amount * (i + 1);
            }

            std::cout << "Total Winnings (Part 2): " << total_winnings << std::endl;     
        }
        else
        {
            std::cerr << "failed to read input file " << argv[i] << std::endl;
        }
    }

    return EXIT_SUCCESS;
}