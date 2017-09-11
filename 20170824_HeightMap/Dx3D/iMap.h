#pragma once
class iMap
{
public:
	iMap(void);
	virtual ~iMap(void);

	virtual bool GetHeight(OUT float& y, IN float x, IN float z) = 0;
};

