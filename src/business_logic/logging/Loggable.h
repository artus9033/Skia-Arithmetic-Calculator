#ifndef LOGGABLE_H
#define LOGGABLE_H

// GCC-specific demangling API
#ifdef __GNUC__
#include <cxxabi.h>

#include <cstdlib>
#endif

#include <regex>
#include <string>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#ifdef _WIN32
#include <dbghelp.h>
#include <windows.h>

#pragma comment(lib, "dbghelp.lib")
#endif

/**
 * @brief The business logic module
 */
namespace business_logic {
    /**
     * @brief Class that provides a logger for the given class; automatically deduces the logger.
     *
     * Class that provides a logger for the given class; automatically deduces the logger name
     * (tag) from the given class's name. This name is demangled if GNU libstdc++ is available,
     * otherwise a possibly-mangled fallback is used.
     * @tparam Clazz The given class to deduce the logger name (tag) from
     */
    template <typename Clazz>
    class Loggable {
       public:
        Loggable() {
            // get the name of the given class; may be mangled
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            const auto *name = typeid(Clazz).name();

            // if gnu libstdc++ available, demangle it
#ifdef __GNUC__
            int status = 0;
            name = abi::__cxa_demangle(name, nullptr, nullptr, &status);
#endif

            std::string classNameStr(name);
#ifdef _WIN32
            char undecoratedName[1024];
            UnDecorateSymbolName(name, undecoratedName, sizeof(undecoratedName), UNDNAME_COMPLETE);

            classNameStr = std::string(undecoratedName);
#endif

            // extract just the class name to get rid of the namespace & possible template arguments
            // using regex; e.g.
            // 'business_logic::window::GLFWQtWindowImpl<business_logic::renderer::SkiaRendererImpl>'
            // -> 'GLFWQtWindowImpl'
            const std::regex regex(R"([^:<>]+(?=<|$))");
            std::cmatch match;
            if (std::regex_search(name, match, regex)) {
                classNameStr = match[0].str();
            }

            logger = spdlog::get(classNameStr);
            if (!logger) {
                logger = spdlog::stdout_color_st(classNameStr);
            }
        }

       protected:
        std::shared_ptr<spdlog::logger> logger;
    };
}  // namespace business_logic

#endif  // LOGGABLE_H
