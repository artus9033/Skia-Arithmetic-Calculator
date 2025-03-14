#ifndef BUSINESS_LOGIC_ELEMENTS_BASE_BLOCK_TYPE_H
#define BUSINESS_LOGIC_ELEMENTS_BASE_BLOCK_TYPE_H

#include <cstdint>

namespace business_logic::elements::blocks {
    /**
     * @brief The available block types
     */
    enum class BlockType : std::uint8_t {
        Constant,
        Subtract,
        Divide,
        Multiply,
        Add,
        Power,
        Invert,
        Monitor
    };
}  // namespace business_logic::elements::blocks

#endif  // BUSINESS_LOGIC_ELEMENTS_BASE_BLOCK_TYPE_H
