# pragma once

# include <Siv3D.hpp>

namespace Debug
{
	/// @brief whether the debug print is enabled
	constexpr bool Enabled = SIV3D_BUILD_PRIVATE_DEFINITION_DEBUG();

	/// @brief print the value to the screen
	/// @param value the value to print
	/// @param always if false, the value is printed only when F3 key is down
	/// @return whether the value is printed
	template <typename T> inline bool Print(const T& value, const bool always = true)
	{
		if constexpr (not Enabled) return false;
		if (not always && not KeyF3.down()) return false;
		s3d::Print << U"{} ({})\t[DEBUG] {}"_fmt(DateTime::Now().format(U"HH:mm:ss.SS"), Scene::FrameCount(), Format(value));
		return true;
	}

	inline bool Log(const FilePath& file, uint32 line, const String& text, const bool always = true)
	{
		return Print(U"({}:{})\t{}"_fmt(FileSystem::RelativePath(file), line, text), always);
	}
}

# if SIV3D_BUILD_PRIVATE_DEFINITION_DEBUG()
#   define DEBUG_DETAIL_LOG(FMT, ...) \
	(Debug::Log(s3d::Unicode::Widen(__FILE__), (__LINE__), s3d::detail::FormatHelper{ (FMT) }(__VA_ARGS__)))
# else
#    define DEBUG_DETAIL_LOG(FMT, ...) ((FMT), (__VA_ARGS__), false)
# endif

/// @brief print the formatted string to the screen only in debug mode
/// @param FMT the format string
/// @param ... the arguments to format
/// @return whether the value is printed
# define DEBUG_PRINT(FMT, ...) (DEBUG_DETAIL_LOG((FMT), __VA_ARGS__))

/// @brief print the value with the name to the screen only in debug mode
/// @param VALUE the value to print
/// @return whether the value is printed
# define DEBUG_DUMP(VALUE) (DEBUG_PRINT(U"{} : {}", s3d::Unicode::Widen(#VALUE), s3d::Format(VALUE)))

/// @brief print the formatted string to the screen only in debug mode when F3 key is down
/// @param FMT the format string
/// @param ... the arguments to format
/// @return whether the value is printed
# define DEBUG_PRINT_F3(FMT, ...) (s3d::KeyF3.down() && (DEBUG_PRINT((FMT), __VA_ARGS__)))

/// @brief print the value with the name to the screen only in debug mode when F3 key is down
/// @param VALUE the value to print
/// @return whether the value is printed
# define DEBUG_DUMP_F3(VALUE) (s3d::KeyF3.down() && (DEBUG_DUMP(VALUE)))
