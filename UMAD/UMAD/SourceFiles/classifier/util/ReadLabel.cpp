
#include "../../../HeaderFiles/classifier/util/ReadLabel.h"

/**
 *@ according to thr requirements of our segmentation data
 *@ param &s expression the primary dataset
 *@ param delim: delim expression various punctuation
 */
vector<string> split(string &s,char delim)
{
	stringstream ss(s);
	string str1;
	vector<string> results;
	while (getline(ss,str1,delim))
		results.push_back(str1);
	return results;
}

/**
 *@ store the class label of train data
 *@ param fileName: fileName is the primary data file that store attribute and class label
 *@ param maxDataNum: the total number of the data list
 */
vector<string> CReadLabel::loadLabel(string fileName,int maxDataNum)
{
	string str;
	ifstream in(fileName);
	//vector <string> data;
	vector <string> trainDataLabel;
	int num,dim;

	if(!in)
	{
		cout<<"can't open the file."<<endl;
	}
	in>>dim>>num;
	getline(in,str);

	for(int i=0;i<maxDataNum;i++)
	{
		getline(in,str);
		vector<string> data=split(str,' ');
#ifdef	_WIN32
		string temp=data.back();
#endif
#ifdef __GNUC__
		string temp=data.back().substr(0,data.back().size()-1);
#endif
		trainDataLabel.push_back(temp);
	}
	return trainDataLabel;
}

