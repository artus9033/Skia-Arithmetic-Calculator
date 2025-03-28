#include "BlocksManager.h"

namespace business_logic {

    BlocksManager::BlocksManager(delegate::IWindowDelegate* windowDelegate)
        : calculations::BlocksCalculator(this),
          draggedBlock(nullptr),
          mouseX(0),
          mouseY(0),
          dragOffset({.width = 0, .height = 0}),
          windowDelegate(windowDelegate) {}

    void BlocksManager::handleMouseDown() {
        // do not handle mouse down if we have an active choices input
        if (hasActiveChoicesInput()) {
            logger->info("Skipping mouse down because we have an active choices input");
            return;
        }

        // check for double-click
        auto maybeClickedBlock = getBlockAtMousePos();

        if (std::chrono::steady_clock::now() - doubleClickCtLastMouseClickTime <=
            constants::DOUBLE_CLICK_TIME_THRESHOLD_MS) {
            // double click

            if (maybeClickedBlock) {
                const auto& clickedBlock = maybeClickedBlock.value();

                auto* doubleClickableBlock =
                    dynamic_cast<business_logic::elements::interactions::IDoubleClickable*>(
                        clickedBlock.get());

                // check if the block is IDoubleClickable
                if (doubleClickableBlock == nullptr) {
                    logger->info("Double clicked block {} is not IDoubleClickable",
                                 clickedBlock->getSelfId());
                } else {
                    doubleClickableBlock->onDoubleClick(mouseX, mouseY);
                }
            } else {
                logger->info("Double clicked outside any block");
            }

            // the next double click event should start counting now
            doubleClickCtLastMouseClickTime = std::chrono::steady_clock::time_point();
        } else {
            // single click

            if (maybeClickedBlock) {
                const auto& clickedBlock = maybeClickedBlock.value();

                auto maybeClickedPort =
                    clickedBlock->getPortAtCoordinates({.x = mouseX, .y = mouseY});

                if (maybeClickedPort.value_or(nullptr) != nullptr) {
                    logger->info("Clicked port '{}' on block {}",
                                 maybeClickedPort.value()->name,
                                 clickedBlock->getSelfId());

                    connectPortsInteraction.handleUserInteractedWith(
                        clickedBlock.get(), maybeClickedPort.value(), windowDelegate, this);

                    // this event should not count as a possible double-click interaction part
                    doubleClickCtLastMouseClickTime = std::chrono::steady_clock::time_point();
                } else {
                    logger->info("Clicked block {} for dragging", clickedBlock->getSelfId());
                    draggedBlock = clickedBlock.get();

                    dragOffset = {.width = mouseX - clickedBlock->getCx(),
                                  .height = mouseY - clickedBlock->getCy()};

                    draggedBlock->onDragStart();

                    // update the last mouse click time for double-click
                    doubleClickCtLastMouseClickTime = std::chrono::steady_clock::now();
                }
            } else {
                logger->info("Clicked outside any block");

                // update the last mouse click time for double-click
                doubleClickCtLastMouseClickTime = std::chrono::steady_clock::now();
            }
        }
    }

    void BlocksManager::handleMouseUp() {
        if (draggedBlock != nullptr) {
            logger->info("Finished dragging block {}", draggedBlock->getSelfId());
            draggedBlock->onDragEnd();
            draggedBlock = nullptr;
        }
    }

    void BlocksManager::handleEscapeKeyPress() {
        logger->info("Escape key pressed");

        if (hasActiveChoicesInput()) {
            clearActiveChoicesInput();
        }

        if (connectPortsInteraction.isStarted()) {
            connectPortsInteraction.resetInteraction();
        }
    }

    void BlocksManager::handleNumericKeyPress(int number) {
        if (inputChoiceInteraction.has_value()) {
            if (number >= 1 &&
                static_cast<size_t>(number) <= inputChoiceInteraction->choices.size()) {
                logger->info("Selected choice: {}", number);

                auto choice = inputChoiceInteraction->choices[number - 1];
                onNewBlockChoice(choice.value);
                clearActiveChoicesInput();
            } else {
                logger->error("Invalid choice number: {}", number);
                windowDelegate->showWarning(
                    "Invalid choice",
                    "You entered: " + std::to_string(number) +
                        " which is out of range. Pick a "
                        "number between 1 and " +
                        std::to_string(inputChoiceInteraction->choices.size()));
            }
        }
    }

    void BlocksManager::handleMouseMove(int x, int y) {
        mouseX = x;
        mouseY = y;

        if (draggedBlock != nullptr) {
            draggedBlock->onDragProgress(mouseX - dragOffset.width, mouseY - dragOffset.height);
        }
    }

    std::optional<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>
    BlocksManager::getBlockAtMousePos() {
        return getBlockAt(mouseX, mouseY);
    }

    std::optional<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>
    BlocksManager::getBlockAt(int x, int y) {
        // find the block that is being hovered over; reverse order to render newer blocks on top
        // (as if they had the highest z-index)
        auto maybeHoveredBlockIt =
            std::find_if(blocks.rbegin(), blocks.rend(), [x, y](const auto& block) {
                return block->isHovered(x, y);
            });

        std::optional<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>
            maybeHoveredBlock = std::nullopt;

        if (maybeHoveredBlockIt != blocks.rend()) {
            maybeHoveredBlock = *maybeHoveredBlockIt;
        }

        return maybeHoveredBlock;
    }

    void BlocksManager::onNewBlockChoice(
        const business_logic::elements::blocks::BlockType& blockType) {
        switch (blockType) {
            case business_logic::elements::blocks::BlockType::Constant: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::ConstantBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Add: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::AddBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Subtract: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::SubtractBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Multiply: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::MultiplyBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Divide: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::DivideBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Power: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::PowerBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Invert: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::InvertBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case business_logic::elements::blocks::BlockType::Monitor: {
                blocks.push_back(std::make_shared<business_logic::elements::blocks::MonitorBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            default: {
                auto name = magic_enum::enum_name(blockType);

                logger->error("Unknown user-selected block type: {}", name);

                windowDelegate->showWarning(
                    "Invalid choice",
                    "You selected an invalid block type: '" + std::string(name) + "'");
            } break;
        }
    }

    void BlocksManager::setActiveChoicesInput(
        input::InputChoiceInteraction<business_logic::elements::blocks::BlockType>&&
            inputChoiceInteraction) {
        auto choices = inputChoiceInteraction.choices;

        std::vector<std::string> labels;
        labels.reserve(choices.size());
        int index = 0;
        std::for_each(
            choices.begin(),
            choices.end(),
            [&index, &labels](
                const input::InputChoice<business_logic::elements::blocks::BlockType>& choice) {
                labels.push_back(std::to_string(++index) + " " + choice.displayName);
            });

        index = 0;  // reset for re-use

        inputChoicesUiTextsRows.reserve((choices.size() / constants::MAX_INPUT_CHOICES_PER_ROW) +
                                        1);
        {
            std::vector<components::UIText> rowBuff;
            size_t number = 1;
            for (; number <= choices.size(); number++) {
                rowBuff.emplace_back(std::to_string(number) + " " + choices[number - 1].displayName,
                                     components::UIText::Variant::Choice);

                if (number % constants::MAX_INPUT_CHOICES_PER_ROW == 0) {
                    inputChoicesUiTextsRows.emplace_back(rowBuff);
                    rowBuff.clear();
                }
            }

            if (!rowBuff.empty()) {
                inputChoicesUiTextsRows.emplace_back(rowBuff);
            }

            logger->info("Prepared {} input choices for rendering", number);
        }

        this->inputChoiceInteraction.emplace(std::move(inputChoiceInteraction));
    }

    bool BlocksManager::hasActiveChoicesInput() const { return inputChoiceInteraction.has_value(); }

    void BlocksManager::clearActiveChoicesInput() {
        logger->info("Clearing active choices input");

        inputChoiceInteraction.reset();

        inputChoicesUiTextsRows.clear();
        inputChoicesUiTextsRows.shrink_to_fit();
    }

    bool BlocksManager::hasConnectionBetween(
        const elements::structures::BlocksConnectionSide& source,
        const elements::structures::BlocksConnectionSide& dest) const {
        elements::structures::BlocksConnectionSide variant1 = {.block = source.block,
                                                               .port = source.port};
        elements::structures::BlocksConnectionSide variant2 = {.block = dest.block,
                                                               .port = dest.port};

        return std::any_of(connectionsRegistry.begin(),
                           connectionsRegistry.end(),
                           [variant1, variant2](const auto& connection) {
                               return connection.first == variant1 &&
                                      connection.second.contains(variant2);
                           });
    }

    bool BlocksManager::isInputConnected(
        const elements::structures::BlocksConnectionSide& side) const {
        return std::any_of(
            connectionsRegistry.begin(), connectionsRegistry.end(), [side](const auto& connection) {
                return connection.first == side || connection.second.contains(side);
            });
    }

    void BlocksManager::onPortsConnected(const elements::structures::BlocksConnectionSide& source,
                                         const elements::structures::BlocksConnectionSide& dest) {
        connectionsRegistry[source].insert(dest);
    }

    void BlocksManager::onBlockDeleted(const business_logic::elements::blocks::BaseBlock* block) {
        // IMPORTANT: this method is called from base (BaseBlock) destructor, so the child
        // class (block implementation) is already destroyed; therefore, its methods are no use
        // here and will result in errors

        // erase entries where the block was the key or the value
        for (auto& [source, destinations] : connectionsRegistry) {
            if (source.block == block) {
                // the deleted block is the source of the connection
                connectionsRegistry.erase(source);
            } else {
                // the deleted block is the destination of the connection
                std::erase_if(destinations,
                              [block](const auto& dest) { return dest.block == block; });
            }
        }

        // ensure draggedBlock is not referencing the deleted block
        if (draggedBlock == block) {
            draggedBlock = nullptr;
        }
    }

    void BlocksManager::handleRightClickOnBlock(
        const std::shared_ptr<business_logic::elements::blocks::BaseBlock>& block) {
        // check if a port was right-clicked
        auto maybeClickedPort = block->getPortAtCoordinates({.x = mouseX, .y = mouseY});

        if (maybeClickedPort.has_value()) {
            elements::structures::BlocksConnectionSide sideToDelete = {
                .block = block.get(), .port = maybeClickedPort.value()};

            // remove all connections from the right-clicked port (if an entry exists, otherwise
            // call does nothing)
            size_t erasedCount = 0;

            for (auto& [source, destinations] : connectionsRegistry) {
                if (source == sideToDelete) {
                    // the right-clicked port is the source of the connection
                    erasedCount += connectionsRegistry.erase(source);
                } else {
                    // the right-clicked port is the destination of the connection
                    erasedCount += std::erase_if(destinations, [sideToDelete](const auto& dest) {
                        return dest == sideToDelete;
                    });
                }
            }

            logger->info("Right clicked on port '{}' - removed its {} connections",
                         maybeClickedPort.value()->name,
                         erasedCount);
        } else {
            logger->info("Right clicked on block '{}' - asking for confirmation about removal",
                         block->getSelfId());

            if (windowDelegate->promptConfirmation(
                    "Remove Block",
                    "Are you sure you want to remove block '" +
                        std::string(magic_enum::enum_name(block->getBlockType())) + "'?")) {
                logger->info("User confirmed removal of block '{}'", block->getSelfId());

                blocks.erase(std::remove(blocks.begin(), blocks.end(), block), blocks.end());

                if (connectPortsInteraction.isStarted()) {
                    connectPortsInteraction.resetInteraction();
                }
            } else {
                logger->info("User cancelled removal of block '{}'", block->getSelfId());
            }
        }
    }

    const std::vector<std::shared_ptr<business_logic::elements::blocks::BaseBlock>>&
    BlocksManager::getBlocks() const {
        return blocks;
    }

    const std::unordered_map<elements::structures::BlocksConnectionSide,
                             std::unordered_set<elements::structures::BlocksConnectionSide>>&
    BlocksManager::getConnectionsRegistry() const {
        return connectionsRegistry;
    }
}  // namespace business_logic
