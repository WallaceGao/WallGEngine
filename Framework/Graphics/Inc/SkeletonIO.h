#include "Skeleton.h"

namespace WallG::Graphics::SkeletonIO
{
	void Write(FILE* file, const Skeleton& mesh);
	void Read(FILE* file, Skeleton& mesh);
}