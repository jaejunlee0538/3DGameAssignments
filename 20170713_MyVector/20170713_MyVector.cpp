// 20170713_MyVector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "cVector3.h"

int main()
{
	cVector3 vec1(1, 2, 3), vec2(3,4,5);
	vec1.Print();
	vec2.Print();

	(vec1 + vec2).Print();
	(vec1 - vec2).Print();
	(vec1 * 2.0).Print();
	std::cout << "Length : " << vec1.Length() << std::endl;
	std::cout << "LengthSq : " << vec1.LengthSq() << std::endl;

	vec1.Normalize().Print();
	std::cout << "cross 결과 확인 : " << cVector3::Dot(cVector3::Cross(vec1, vec2), vec1) << std::endl;
	std::cout << "cross 결과 확인 : " << cVector3::Dot(cVector3::Cross(vec2, vec1), vec2) << std::endl;

    return 0;
}

