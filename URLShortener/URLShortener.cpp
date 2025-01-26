#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cstdio>
#include <format>
#include <regex>

struct shortenedURLStruct
{
	std::string original_url;
	std::string shortened_url;
	//std::unordered_map<std::string, std::string> url_container;
} ;

class ShortenedURL
{
public:
	shortenedURLStruct url_container;
	ShortenedURL(std::string url)
	{
		url_container.original_url = url;
		url_container.shortened_url = URLShortener(url);
	}

	//operator std::string() const { return std::format("Original URL: {0}\tUnique URL: {1}", url_container.original_url, url_container.shortened_url); }

	std::string to_string()
	{
		return std::format("Original URL: {0}\tUnique URL: {1}", url_container.original_url, url_container.shortened_url);
	}

	bool operator==(const ShortenedURL& other) const
	{
		return url_container.original_url == other.url_container.original_url &&
			url_container.shortened_url == other.url_container.shortened_url;
	}


private:
	std::string URLShortener(std::string url)
	{
		//std::string url = "https://www.sap.com/hungary/index.html?url_id=auto_hp_redirect_hungary";

		size_t hash = std::hash<std::string>{}(url);
		std::string unique_url = std::string("https://myshort.url/").append(std::to_string(hash));
		
		return unique_url;
	}
};

namespace std 
{
	template <>
	struct hash<ShortenedURL>
	{
		size_t operator()(const ShortenedURL& su) const
		{
			return hash<std::string>()(su.url_container.original_url) ^ hash<std::string>()(su.url_container.shortened_url);
		}
	};
}

int main()
{
	const std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");

	std::unordered_set<ShortenedURL> urls;
	std::string input;

	while (true)
	{
		std::cout << "URL: ";
		std::cin >> input;
		if (strcmp(input.c_str(), "print_all") == 0)
		{
			std::printf("List of URLs:\n");
			for (auto u : urls)
			{
				std::printf("%s\n", u.to_string().c_str());
			}
		}
		else
		{
			if (urls.find(ShortenedURL(input)) == urls.end())
			{
				if (std::regex_match(input, pattern))
				{
					urls.insert(ShortenedURL(input));
					std::printf("The URL was shortened successfully!\n");
				}
				else
				{
					std::printf("URL is invalid!\n");
				}

			}
			else
			{
				std::printf("URL already exists!\n");
			}
		}
	}
	
	return 0;
}
