#include "BlockType.h"

namespace gui::elements::base {

    const std::unordered_map<BlockType, std::string> BlockTypeNames = {
        {BlockType::Invert, "Invert"s},
        {BlockType::Add, "Add"s},
        {BlockType::Subtract, "Subtract"s},
        {BlockType::Multiply, "Multiply"s},
        {BlockType::Divide, "Divide"s},
        {BlockType::Power, "Power"s},
        {BlockType::Constant, "Constant"s},
        {BlockType::Monitor, "Monitor"s},
    };

    const std::vector<gui::input::InputChoice<BlockType>> BlockInputChoices = [] {
        std::vector<gui::input::InputChoice<BlockType>> temp;

        temp.reserve(BlockTypeNames.size());

        std::for_each(BlockTypeNames.begin(),
                      BlockTypeNames.end(),
                      [&temp](const std::pair<BlockType, std::string>& pair) {
                          temp.push_back(gui::input::InputChoice<BlockType>{
                              .displayName = pair.second, .value = pair.first});
                      });

        return temp;
    }();
}  // namespace gui::elements::base
