#pragma once
struct cVariant
{
	enum Types {
		VAR_TYPE_INT32,
		VAR_TYPE_FLOAT,
		VAR_TYPE_BOOL
	};

	explicit cVariant(int32_t data) {
		type = VAR_TYPE_INT32;
		AsInt32 = data;
	}

	explicit cVariant(bool data) {
		type = VAR_TYPE_BOOL;
		AsBool = data;
	}

	explicit cVariant(float data) {
		type = VAR_TYPE_FLOAT;
		AsFloat32 = data;
	}

	Types type;
	union {
		int32_t		AsInt32;
		float		AsFloat32;
		bool		AsBool;
	};
};

