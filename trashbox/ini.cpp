#include "ini.h"

#include <sstream>
#include <iostream>

/*
 * guess that file like
 * [section]
 * key =
 * value
 * is invalid
 */


std::ostream& operator<<(std::ostream& out, const Ini::Section& section)
{
	std::string sep = ":";
	std::string end = "\n";
	for (const auto&[key, value] : section)
	{
		out << key << sep << value << end;
	}
	return out;
}

namespace Ini
{
	Section& Document::AddSection(std::string name)
	{
		return sections.insert({move(name), {}}).first->second;
	}

	const Section& Document::GetSection(const std::string& name) const
	{
		return sections.at(name);
	}

	size_t Document::SectionCount() const
	{
		return sections.size();
	}

	Document Load(istream& input)
	{
		Document doc;
		Section* current_section = nullptr;

		for (string line; getline(input, line);)
		{
			if (line.empty()) continue;

			if (line[0] == '[')
			{
				string section_name;
				int i = 1;
				while (line[i] != ']')
				{
					section_name += line[i++];
				}
				current_section = &doc.AddSection(section_name);
			}
			else
			{
				string key;
				string value;
				bool wasEqualSign = false;
				for (auto c : line)
				{
					if (c == '=')
					{
						wasEqualSign = true;
					}
					else if (wasEqualSign)
					{
						value += c;
					}
					else
					{
						key += c;
					}
				}
				current_section->insert({move(key), move(value)});
			}
		}
		return doc;
	}
}

