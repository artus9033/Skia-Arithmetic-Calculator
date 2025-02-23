#ifndef MISC_H
#define MISC_H

#include <format>
#include <string>

namespace business_logic {
    /**
     * @brief Convert a pointer to a string in format "0x..." containing its hexadecimal address
     * @param value The pointer to convert
     * @tparam T The type of the pointer
     * @return The string
     */
    template <typename T>
    inline std::string stringifyAddressOf(const T* value) {
        return std::format("0x{:x}", reinterpret_cast<std::intptr_t>(value));
    }
}  // namespace business_logic

#endif  // MISC_H
