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
        /**
         * The name of this logger
         */
        const char* name;

        Loggable() {
            // get the name of the given class; may be mangled
            // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
            name = typeid(Clazz).name();

            // if gnu libstdc++ available, demangle it
#ifdef __GNUC__
            int status = 0;
            name = abi::__cxa_demangle(name, nullptr, nullptr, &status);
#endif

            // extract just the class name to get rid of the namespace & possible template arguments
            // using regex; e.g.
            // 'business_logic::window::GLFWWindowImpl<business_logic::renderer::SkiaRendererImpl>'
            // -> 'GLFWWindowImpl'
            const std::regex regex(R"([^:<>]+(?=<|$))");
            std::cmatch match;
            if (std::regex_search(name, match, regex)) {
                name = match[0].str().c_str();
            }

            logger = spdlog::get(name);
            if (!logger) {
                logger = spdlog::stdout_color_mt(name);
            }
        }

       protected:
        std::shared_ptr<spdlog::logger> logger;
    };
}  // namespace business_logic

#endif  // LOGGABLE_H
