#include "stdafx.h"
#include "cMapLoader.h"


cMapLoader::cMapLoader()
{
}

cMapLoader::~cMapLoader()
{
}

void cMapLoader::Load(IN char * szFullPath, OUT vector<vector<int>>& obj)
{
	vector<int> num;

	FILE* fp = 0;
	fopen_s(&fp, szFullPath, "r");

	while (!feof(fp))
	{

		char c = fgetc(fp);
		if (c == '\n') 
		{
			obj.push_back(num);
			num.clear();
		}
		else
		{
			int kindBox = atoi(&c);
			num.push_back(kindBox);
		}
		
	}
	//이거왜하나더들어가는거임?
	num.pop_back();
	obj.push_back(num);
	num.clear();

	fclose(fp);
	
}
