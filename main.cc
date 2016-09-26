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
	return 0;
}
