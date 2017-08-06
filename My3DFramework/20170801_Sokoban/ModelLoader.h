#pragma once
#include "MeshModel.h"
namespace SGA {
	namespace ModelLoad {
		class I3DModelLoader
		{
		public:
			I3DModelLoader() {}
			virtual ~I3DModelLoader() {}
			virtual void load3DModel(const char* modelFile, MeshModel& meshModel) = 0;
		};
	}
}