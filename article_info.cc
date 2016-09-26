#include "article_info.hh"

#include <poppler-document.h>
#include <poppler-page.h>

using namespace std;

ArticleInfo::ArticleInfo()
{
	this->has_doi = false;
	this->has_metadata = false;
	this->filename = "";
	this->doi = "";
	this->uri = "";
	this->content = "";
	this->title = "";
	this->author.clear();
	this->date = "";
	this->publisher = "";
	this->volume = "";
	this->issue = "";
	this-> first_page = "";
}

ArticleInfo::~ArticleInfo()
{
	this->filename = "";
	this->doi = "";
	this->uri = "";
	this->content = "";
	this->title = "";
	this->author.clear();
	this->date = "";
	this->publisher = "";
	this->volume = "";
	this->issue = "";
	this-> first_page = "";
}

void ArticleInfo::extract_pdf(char *fileloc)
{
	int i, j;
	size_t first;
	size_t last;
	string page_content;

	poppler::document *pdfdoc = poppler::document::load_from_file(fileloc);
	for (i = 0; i < pdfdoc->pages(); ++i)
	{
		page_content = pdfdoc->create_page(i)->text().to_latin1();
		// Remove special characters from the content
		for(j = 0;j < page_content.length();j++)
		{
			if(page_content[j] < 32 || page_content[j] > 126)
				page_content[j] = ' ';
		}
		this->content.append(page_content);
		this->content.append(" ");
		if(!this->has_doi)
		{
			first = page_content.find("dx.doi.org");
			if(first != string::npos)
			{
				last = page_content.find(" ", first);
				this->doi = page_content.substr(first+11, last-first-11);
				this->uri = doi.insert(0, "http://dx.doi.org/");
				this->has_doi = true;
				continue;
			}

			first = page_content.find("DOI");
			if(first != string::npos)
			{
				last = page_content.find(" ", first+5);
				this->doi = page_content.substr(first+5, last-first-5);
				this->uri = doi.insert(0, "http://dx.doi.org/");
				this->has_doi = true;
				continue;
			}

			first = page_content.find("doi");
			if(first != string::npos)
			{
				last = page_content.find(" ", first);
				this->doi = page_content.substr(first+4, last-first-4);
				this->uri = doi.insert(0, "http://dx.doi.org/");
				this->has_doi = true;
				continue;
			}

			first = page_content.find("arXiv");
			if(first != string::npos)
			{
				last = page_content.find(" ", first);
				this->doi = page_content.substr(first+6, last-first-6);
				this->uri = doi.insert(0, "http://arxiv.org/abs/");
				this->has_doi = true;
				continue;
			}
		}
	}
	// Sanitize DOI and URI
	for(i = 0;i < this->doi.length();i++)
	{
		if(this->doi[i] == '\n')
			this->doi.erase(i,1);
	}
	for(i = 0;i < this->uri.length();i++)
	{
		if(this->uri[i] == '\n')
			this->uri.erase(i,1);
	}
}


