#include "../../includes/include.hpp"

template <typename T>
size_t	file_cursor_pos(T &filestream)
{
	filestream.clear();
	return ((size_t)filestream.tellg());
}
