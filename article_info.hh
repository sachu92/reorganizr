#ifndef ARTICLE_INFO_HH
#define ARTICLE_INFO_HH

#include <string>
#include <vector>

class ArticleInfo
{

public:
	ArticleInfo();
	~ArticleInfo();
	void extract_pdf(char *);

	bool has_doi;
	bool has_metadata;
	// Relevant information
	std::string filename;
	std::string doi;
	std::string uri;
	std::string content;
	std::string title;
	std::vector <std::string> author;
	std::string date;
	std::string publisher;
	std::string volume;
	std::string issue;
	std::string first_page;
};

#endif
