#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <regex>
#include <climits>

enum class block_colour
{
	red,
	green,
	blue
};

typedef struct _block_set
{
	block_colour	colour;
	size_t			count;
} block_set;

typedef struct _game
{
	size_t					id;
	std::vector<block_set>	sets;
} game;

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path path(argv[i]);

		if (!std::filesystem::exists(path))
		{
			std::cerr << "path " << path << " does not exist." << std::endl;
			continue;
		}

		std::ifstream stream(path);
		if (!stream.is_open())
		{
			std::cerr << "file " << path << " could not be opened." << std::endl;
			continue;
		}

		std::string line;

		std::vector<game> games;

		std::regex game_regex("Game ([0-9]+):");
		std::regex set_regex("([0-9]+) ([a-z]+),?");

		size_t id_sum = 0;
		size_t power_sum = 0;

		while (getline(stream, line))
		{
			std::smatch game_match;

			if (!std::regex_search(line, game_match, game_regex)|| game_match.size() != 2)
			{
				std::cerr << "line " << line << " does not match expected format." << std::endl;
				return EXIT_FAILURE;
			}

			size_t game_id = std::atol(game_match[1].str().c_str());

			game this_game{ game_id };

			auto colon = line.find(":");

			auto set_strings = line.substr(colon + 1);

			size_t offset = 0;

			while (offset != std::string::npos)
			{
				size_t new_offset = set_strings.find(";", offset + 1);
				auto set_string = set_strings.substr(offset + 1, new_offset - (offset + 1));

				std::smatch set_match;

				while (std::regex_search(set_string, set_match, set_regex) && set_match.size() == 3)
				{
					block_set bset;

					bset.count	= std::atol(set_match[1].str().c_str());
					auto colour = set_match[2];

					if (colour == "red")
					{
						bset.colour = block_colour::red;
					}
					else if (colour == "green")
					{
						bset.colour = block_colour::green;
					}
					else if (colour == "blue")
					{
						bset.colour = block_colour::blue;
					}
					else
					{
						std::cerr << "unrecognized block colour " << colour << " in line " << line << std::endl;
						return EXIT_FAILURE;
					}

					this_game.sets.push_back(bset);

					set_string	= set_match.suffix();
				}

				offset = new_offset;
			}

			bool can_satisfy = true;

			block_set red_min{ block_colour::red, 0 };
			block_set blue_min{ block_colour::red, 0 };
			block_set green_min{ block_colour::red, 0 };

			for (auto& set : this_game.sets)
			{
				if (set.colour == block_colour::blue)
				{
					if (set.count > 14)
					{
						can_satisfy = false;
					}
					if (set.count > blue_min.count)
					{
						blue_min.count = set.count;
					}
				}
				else if (set.colour == block_colour::red)
				{
					if (set.count > 12)
					{
						can_satisfy = false;
					}
					if (set.count > red_min.count)
					{
						red_min.count = set.count;
					}
				}
				else if (set.colour == block_colour::green)
				{
					if (set.count > 13)
					{
						can_satisfy = false;
					}
					if (set.count > green_min.count)
					{
						green_min.count = set.count;
					}
				}
			}

			if (can_satisfy)
			{
				id_sum += this_game.id;
			}

			size_t power = 1;

			if (red_min.count > 0)
			{
				power *= red_min.count;
			}
			if (blue_min.count >  0)
			{
				power *= blue_min.count;
			}
			if (green_min.count > 0)
			{
				power *= green_min.count;
			}

			power_sum += power;
		}

		std::cout << "ID Sum: " << id_sum << std::endl;
		std::cout << "Power Sum: " << power_sum << std::endl;
	}

	return EXIT_SUCCESS;
}