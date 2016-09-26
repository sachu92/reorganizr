#include "article_info.hh"

#include <cstring>
#include <iostream>
#include <poppler-document.h>
#include <poppler-page.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>

using namespace std;

std::string htmlText;

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
	this->journal = "";
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
	this->journal = "";
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

void ArticleInfo::extract_metadata()
{
	CURL *curl;
	
	curl_global_init(CURL_GLOBAL_ALL);

	// Init curl session
	curl = curl_easy_init();
	if(curl)
	{
	// set url
	curl_easy_setopt(curl, CURLOPT_URL, this->uri.c_str());
	// verbose output for debug mode
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
	// disable progress bar
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
	//set write call back
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	// follow redirections
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// write content to string
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlText);
	
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	}
	else
	{
		cerr<<"Error initializing cURL."<<std::endl;
		return;
	}

	cout<<"Cleaned up cURL."<<endl;

	htmlDocPtr doc;
	xmlNode *root = NULL;

	LIBXML_TEST_VERSION

	doc = htmlReadMemory(htmlText.c_str(), htmlText.size(), this->uri.c_str(), NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
	if(NULL == doc)
	{
		fprintf(stderr, "Document not parsed.\n");
		return;
	}
	cout<<"Read html."<<endl;
	root = xmlDocGetRootElement(doc);
	
	if(NULL == root)
	{
		fprintf(stderr, "empty document\n");
		xmlFree(doc);
		return;
	}

	this->traverse(root);

	xmlFreeDoc(doc);
	xmlCleanupParser();
	return;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	((string*)stream)->append((char *)ptr, size * nmemb);
	return size*nmemb;
}

void ArticleInfo::traverse(xmlNode *node)
{
	xmlNode *curr = NULL;

	if(NULL == node)
		return;
	for(curr = node; curr; curr = curr->next)
	{
		if(curr->type == XML_ELEMENT_NODE)
		{
			if(xmlStrcmp(curr->name, (xmlChar *)"meta")==0)
			{
				xmlChar *name = xmlGetProp(curr, (xmlChar *)"name");
				xmlChar *content = xmlGetProp(curr, (xmlChar *)"content");
				if(xmlStrcmp(name, (xmlChar *)"citation_author")==0)
				{
					this->author.push_back((char *)content);
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_title")==0)
				{
					this->title = (char *)content;
					this->has_metadata = true;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_date")==0)
				{
					this->date = (char *)content;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_journal_title")==0)
				{
					this->journal = (char *)content;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_publisher")==0)
				{
					this->publisher = (char *)content;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_volume")==0)
				{
					this->volume = (char *)content;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_issue")==0)
				{
					this->issue = (char *)content;
				}
				else if(xmlStrcmp(name, (xmlChar *)"citation_firstpage")==0)
				{
					this->first_page = (char *)content;
				}
				xmlFree(name);
				xmlFree(content);
			}
		}
		traverse(curr->children);
	}
}


void ArticleInfo::print_info()
{
	cout<<"Title: "<<this->title<<endl;
	cout<<"Author(s):"<<endl;
	for(int i = 0;i < this->author.size();i++)
	{
		cout<<this->author[i]<<endl;
	}
	cout<<"Date: "<<this->date<<endl;
	cout<<"Journal: "<<this->journal<<endl;
	cout<<"Publisher: "<<this->publisher<<endl;
	cout<<"Volume: "<<this->volume<<endl;
	cout<<"Issue: "<<this->issue<<endl;
	cout<<"First page: "<<this->first_page<<endl;
}
