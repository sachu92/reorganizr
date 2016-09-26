#ifndef ARTICLE_INFO_HH
#define ARTICLE_INFO_HH

#include <string>
#include <vector>

#include <libxml/HTMLparser.h>


extern std::string htmlText;
static size_t write_data(void *, size_t, size_t, void*);

class ArticleInfo
{

public:
	ArticleInfo();
	~ArticleInfo();
	void extract_pdf(char *);
  void extract_metadata();
	void traverse(xmlNode*);
	void print_info();

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
	std::string journal;
	std::string publisher;
	std::string volume;
	std::string issue;
	std::string first_page;
};

#endif
