#include "servertransport.h"


void 
s_fun(int signo)
{
	cerr << "signo:" << signo << endl;
	alarm(5);
	return;
}


void recvrawdatafromclient(int connfd, string &dataFileName, int dim, string &datatype, int finalSize)
{
		if (!datatype.compare("vector"))
		{
				CDoubleVector::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("image"))
		{
				CImage::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("string"))
		{
				CStringObject::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("dna"))
		{
				CDNA::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("rna"))
		{
				CRNA::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("peptide"))
		{
				CPeptide::recvrawdata(connfd, dataFileName, dim, finalSize);
		}
		else if (!datatype.compare("spectra"))
		{
				//CTandemSpectra::recvrawdata(connfd, dataFileName, dim, finalSize);
				return;
		}
}

void recvquerydatafromclient(int connfd, string &dataFileName, int dim, string &datatype, int lastQuery)
{
		if (!datatype.compare("vector"))
		{
				CDoubleVector::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("image"))
		{
				CImage::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("string"))
		{
				CStringObject::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("dna"))
		{
				CDNA::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("rna"))
		{
				CRNA::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("peptide"))
		{
				CPeptide::recvquerydata(connfd, dataFileName, dim, lastQuery);
		}
		else if (!datatype.compare("spectra"))
		{
				//CTandemSpectra::recvquerydata(connfd, dataFileName, dim, lastQuery);
				return;
		}
}
