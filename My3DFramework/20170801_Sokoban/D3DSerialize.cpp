#include "stdafx.h"
#include "D3DSerialize.h"
#include <iomanip>

namespace SGA {
	std::ostream & operator<<(std::ostream & os, const D3DXMATRIX & mat)
	{
		for (int iRow = 0; iRow < 4; iRow++) {
			for (int iCol = 0; iCol < 4; iCol++) {
				os << std::fixed << std::setw(9) << std::setprecision(4) << mat(iRow, iCol) << " ";
			}
			os << std::endl;
		}
		return os;
	}
}