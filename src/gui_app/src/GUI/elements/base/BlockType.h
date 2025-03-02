#ifndef GUI_ELEMENTS_BASE_BLOCK_TYPE_H
#define GUI_ELEMENTS_BASE_BLOCK_TYPE_H

#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

#include "GUI/input/InputChoice.h"

namespace gui::elements::base {
    using namespace std::string_literals;

    enum class BlockType { Constant, Subtract, Divide, Multiply, Add, Power, Invert, Monitor };

    extern const std::unordered_map<BlockType, std::string> BlockTypeNames;

    extern const std::vector<gui::input::InputChoice<BlockType>> BlockInputChoices;
}  // namespace gui::elements::base

#endif  // GUI_ELEMENTS_BASE_BLOCK_TYPE_H
