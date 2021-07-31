#include "AnimationClip.h"
#include "Animation.h"

namespace WallG::Graphics
{
	class AnimationIO
	{
	public:
		static void Write(FILE* file, const std::vector<std::unique_ptr<AnimationClip>>& animationClip);
		static void Read(FILE* file, std::vector<std::unique_ptr<AnimationClip>>& animationClip);
		static void WriteAnimation(FILE* file, const Animation& animation);
		static void ReadAnimation(FILE* file, Animation& animation);
	};
}
