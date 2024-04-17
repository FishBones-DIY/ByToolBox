#include <algorithm>

#include "../public/log.h"

#include "string_utils.h"



namespace string_utils
{
	std::string trim(const std::string& s, char c /*= ' '*/)
	{
		std::string::size_type a = s.find_first_not_of(c);
		std::string::size_type z = s.find_last_not_of(c);
		if (a == std::string::npos) {
			a = 0;
		}

		if (z == std::string::npos) {
			z = s.size();
		}

		return s.substr(a, z + 1 - a);
	}

	bool startsWithW(std::wstring s, std::wstring sub)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](auto c) { return std::toupper(c); });
		std::transform(sub.begin(), sub.end(), sub.begin(),
			[](auto c) { return std::toupper(c); });
		return s.find(sub) == 0 ? true : false;
	}

}; // namespace reg_utils