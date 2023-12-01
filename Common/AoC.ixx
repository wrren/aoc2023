export module AoC;

import std;

export template<typename Acc> bool ReadLines(const std::filesystem::path& Path, std::function<bool(const std::string&, Acc&)> Fun, Acc& Accumulator = {})
{
	auto Stream = std::ifstream(Path);

	if (!Stream.is_open())
	{
		return false;
	}

	std::string Line;

	while (getline(Stream, Line))
	{
		if (Fun(Line, Accumulator) == false)
		{
			return false;
		}
	}

	return true;
}