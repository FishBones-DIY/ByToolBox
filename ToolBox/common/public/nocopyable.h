#pragma once

namespace TOOLBOX {
	class NoCopyable
	{
	protected:
		NoCopyable() = default;
		~NoCopyable() = default;

		NoCopyable(const NoCopyable&) = delete;
		NoCopyable& operator=(const NoCopyable) = delete;
	};
}  // namespace TOOLBOX
