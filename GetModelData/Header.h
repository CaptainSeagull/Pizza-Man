#include <wtypes.h>

namespace ModelData
{
	extern "C" { __declspec(dllexport) void GetModelData(const char* file, float &sizeX, float &sizeY, float &sizeZ, float &positionX, float &positionY, float &positionZ); }
}