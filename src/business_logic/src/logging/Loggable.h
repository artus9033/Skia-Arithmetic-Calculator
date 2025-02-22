#ifndef LOGGABLE_H
#define LOGGABLE_H

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace business_logic {
    class Loggable {
       public:
        Loggable(std::string logTag);

       protected:
        std::shared_ptr<spdlog::logger> logger;
    };
}  // namespace business_logic

#endif  // LOGGABLE_H
