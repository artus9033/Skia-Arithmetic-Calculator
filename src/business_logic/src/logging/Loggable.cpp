#include "logging/Loggable.h"

namespace business_logic {
    Loggable::Loggable(std::string logTag) { logger = spdlog::stdout_color_mt(logTag); }
}  // namespace business_logic
