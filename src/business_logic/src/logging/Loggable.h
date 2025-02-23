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

namespace business_logic {
    /**
     * Class that provides a logger for the derived class; automatically deduces the logger name
     * (tag) from the derived class's name. This name is demangled if GNU libstdc++ is available,
     * otherwise a possibly-mangled fallback is used.
     * @tparam Derived The derived class to deduce the logger name (tag) from
     */
    template <typename Derived>
    class Loggable {
       public:
        Loggable() {
            // get the name of the derived class; may be mangled
            auto name = typeid(Derived).name();

            // if gnu libstdc++ available, demangle it
#ifdef __GNUC__
            int status = 0;
            name = abi::__cxa_demangle(name, NULL, NULL, &status);
#endif

            // extract just the class name to get rid of the namespace & possible template arguments
            // using regex; e.g. 'gui::window::GLFWWindowImpl<gui::renderer::SkiaRendererImpl>' ->
            // 'GLFWWindowImpl'
            std::regex regex(R"([^:<>]+(?=<|$))");
            std::cmatch match;
            if (std::regex_search(name, match, regex)) {
                name = match[0].str().c_str();
            }

            logger = spdlog::stdout_color_mt(name);
        }

       protected:
        std::shared_ptr<spdlog::logger> logger;
    };
}  // namespace business_logic

#endif  // LOGGABLE_H
