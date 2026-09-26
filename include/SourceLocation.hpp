#pragma once // NOLINT(llvm-header-guard)

namespace Helper::Logger
{

/**
 * @struct SourceLocation
 * @brief Captures the source location of a log call site.
 *
 * This lightweight POD structure stores file name, line number,
 * and function name from the call site using compiler macros
 * (__FILE__, __LINE__, __func__).
 *
 * Using a simple struct instead of std::source_location (C++20)
 * to maintain C++17 compatibility.
 */
struct SourceLocation
{
    const char* file { nullptr };
    int line { 0 };
    const char* function { nullptr };

    constexpr SourceLocation() = default;

    constexpr SourceLocation(const char* f, int l, const char* fn)
        : file(f)
        , line(l)
        , function(fn)
    {
    }

    /**
     * @brief Extracts the basename from a full file path.
     *
     * Returns a pointer to the character after the last '/' separator.
     * If no separator is found, returns the original path.
     *
     * @param path Full file path string.
     * @return Pointer to the basename portion of the path.
     */
    static constexpr const char* extractBasename(const char* path)
    {
        if (path == nullptr)
        {
            return "";
        }
        const char* base = path;
        const char* p = path;
        while (*p != '\0')
        {
            if (*p == '/')
            {
                base = p + 1;
            }
            ++p;
        }
        return base;
    }

    /**
     * @brief Returns the basename of the source file.
     *
     * @return Basename string (pointer into the original __FILE__ string).
     */
    [[nodiscard]] constexpr const char* basename() const
    {
        return extractBasename(file);
    }
};

} // namespace Helper::Logger
