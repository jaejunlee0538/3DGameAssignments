#pragma once
class iMap
{
public:
	iMap(void);
	virtual ~iMap(void);

	virtual bool GetHeight(IN float x, OUT float& y, IN float z) = 0;
};

