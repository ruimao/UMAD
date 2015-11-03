#include "clienttransport.h"

void
s_pipfun(int signo)
{
	cerr << "signo:" << signo << ", the server does not listen on the target port." << endl;
	return;
}

//send raw data set to every server
void sendrawdatatoserver(int *serverlist, int servernum, char *filename, int size, char *datatype, int fragmentlength)
{
		if (!strcmp(datatype, "vector"))
		{
			CDoubleVector::sendrawdata(serverlist, servernum, filename, size);	
		}
		else if(!strcmp(datatype,"spectra"))
		{
			//CTandemSpectra::sendrawdata(serverlist, servernum, filename, size, fragmentlength);
			return ;
		}
		else if(!strcmp(datatype,"string"))
		{
			CStringObject::sendrawdata(serverlist, servernum, filename, size);
		}
		else if(!strcmp(datatype,"rna"))
		{
			CRNA::sendrawdata(serverlist, servernum, filename, size, fragmentlength);
		}
		else if(!strcmp(datatype,"image"))
		{
			CImage::sendrawdata(serverlist, servernum, filename, size);
		}
		else if(!strcmp(datatype,"dna"))
		{
			CDNA::sendrawdata(serverlist, servernum, filename, size, fragmentlength);
		}
		else if(!strcmp(datatype,"peptide"))
		{
			CPeptide::sendrawdata(serverlist, servernum, filename, size, fragmentlength);
		}
}

//send query data set to every server
void sendquerydatatoserver(int *serverlist, int servernum, char *filename, int size, char *datatype, int fragmentlength)
{
		if (!strcmp(datatype, "vector"))
		{
			CDoubleVector::sendquerydata(serverlist, servernum, filename, size);	
		}
		else if(!strcmp(datatype,"spectra"))
		{
			//CTandemSpectra::sendquerydata(serverlist, servernum, filename, size, fragmentlength);
			return ;
		}
		else if(!strcmp(datatype,"string"))
		{
			CStringObject::sendquerydata(serverlist, servernum, filename, size);
		}
		else if(!strcmp(datatype,"rna"))
		{
			CRNA::sendquerydata(serverlist, servernum, filename, size, fragmentlength);
		}
		else if(!strcmp(datatype,"image"))
		{
			CImage::sendquerydata(serverlist, servernum, filename, size);
		}
		else if(!strcmp(datatype,"dna"))
		{
			CDNA::sendquerydata(serverlist, servernum, filename, size, fragmentlength);
		}
		else if(!strcmp(datatype,"peptide"))
		{
			CPeptide::sendquerydata(serverlist, servernum, filename, size, fragmentlength);
		}
}
