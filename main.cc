#include <iostream>
#include <cstring>
#include <string>

#include <poppler-document.h>
#include <poppler-page.h>

using namespace std;

string get_doi(char *filename)
{
	string page_content;
	string doi;
	size_t first;
	size_t last;

	poppler::document *pdfdoc = poppler::document::load_from_file(filename);
	for (int i = 0; i < pdfdoc->pages; ++i)
	{
		page_content = pdfdoc->create_page(i)->text().to_latin1();
		first = page_content.find("dx.doi.org");
		if(first != string::npos)
		{
			end = page_content.find(" ", first);
			doi = page_content.substr(first+11, end-start-11);
			break;
		}
		
		first = page_content.find("DOI");
		if(first != string::npos)
		{
			end = page_content.find(" ", first);
			doi = page_content.substr(first+5, end-start-5);
			break;
		}

		first = page_content.find("doi");
		if(first != string::npos)
		{
			end = page_content.find(" ", first);
			doi = page_content.substr(first+4, end-start-4);
			break;
		}
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
