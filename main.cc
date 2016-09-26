#include <iostream>
#include <cstring>
#include <string>

#include <poppler-document.h>
#include <poppler-page.h>

using namespace std;

string get_doi(char *filename)
{
	int i;
	string page_content;
	string doi;
	size_t first;
	size_t last;

	poppler::document *pdfdoc = poppler::document::load_from_file(filename);
	for (i = 0; i < 2; ++i)
	{
		page_content = pdfdoc->create_page(i)->text().to_latin1();
		cout<<"$$$$$$$$$$$$"<<endl<<page_content<<endl;
		first = page_content.find("dx.doi.org");
		if(first != string::npos)
		{
			last = page_content.find(" ", first);
			doi = page_content.substr(first+11, last-first-11);
			doi = doi.insert(0, "http://dx.doi.org/");
			break;
		}
		
		first = page_content.find("DOI");
		if(first != string::npos)
		{
			last = page_content.find(" ", first+5);
			doi = page_content.substr(first+5, last-first-5);
			doi = doi.insert(0, "http://dx.doi.org/");
			break;
		}

		first = page_content.find("doi");
		if(first != string::npos)
		{
			last = page_content.find(" ", first);
			doi = page_content.substr(first+4, last-first-4);
			doi = doi.insert(0, "http://dx.doi.org/");
			break;
		}

		first = page_content.find("arXiv");
		if(first != string::npos)
		{
			last = page_content.find(" ", first);
			doi = page_content.substr(first+6, last-first-6);
			doi = doi.insert(0, "http://arxiv.org/abs/");
			break;
		}
	}
	for(i = 0;i < doi.length();i++)
	{
		if(doi[i] == '\n')
			doi.erase(i,1);
	}
	return doi;
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cerr<<"Usage: "<<argv[0]<<" <pdf-file>"<<endl;
		return 0;
	}

	cout<<"#"<<get_doi(argv[1])<<"#"<<endl;
	return 0;
}
