#include <iostream>
#include "article_info.hh"

using namespace std;


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cerr<<"Usage: "<<argv[0]<<" <pdf-file>"<<endl;
		return 0;
	}
	
	ArticleInfo article;

	article.extract_pdf(argv[1]);
	cout<<"#"<<article.doi<<"#"<<endl;
	if(article.has_doi)
	{
		article.extract_metadata();
		if(article.has_metadata)
		{
			article.print_info();
		}
		else
		{
			cerr<<"Could not retrieve metadata."<<endl;
		}
	}
	else
		cerr<<"Did not get DOI"<<endl;
	return 0;
}
