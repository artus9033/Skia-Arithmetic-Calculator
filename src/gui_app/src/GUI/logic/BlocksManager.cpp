#include "BlocksManager.h"

namespace gui::logic {
    BlocksManager::BlocksManager(gui::window::delegate::IWindowDelegate* windowDelegate)
        : gui::logic::calculations::BlocksCalculator(this),
          doubleClickCtLastMouseClickTime(0),
          windowDelegate(windowDelegate) {}

    void BlocksManager::handleMouseDown() {
        // do not handle mouse down if we have an active choices input
        if (hasActiveChoicesInput()) {
            logger->info("Skipping mouse down because we have an active choices input");
            return;
        }

        // check for double-click
        auto maybeClickedBlock = getBlockAtMousePos();

        if (time(nullptr) - doubleClickCtLastMouseClickTime <
            constants::DOUBLE_CLICK_TIME_THRESHOLD_SECONDS) {
            // double click

            if (maybeClickedBlock) {
                auto clickedBlock = maybeClickedBlock.value();

                auto doubleClickableBlock =
                    dynamic_cast<gui::elements::base::IDoubleClickable*>(clickedBlock.get());

                // check if the block is IDoubleClickable
                if (doubleClickableBlock) {
                    doubleClickableBlock->onDoubleClick(mouseX, mouseY);
                } else {
                    logger->info("Double clicked block {} is not IDoubleClickable",
                                 clickedBlock->getSelfId());
                }
            } else {
                logger->info("Double clicked outside any block");
            }

            // the next double click event should start counting now
            doubleClickCtLastMouseClickTime = 0;
        } else {
            // single click

            if (maybeClickedBlock) {
                auto clickedBlock = maybeClickedBlock.value();

                auto maybeClickedPort =
                    clickedBlock->getPortAtCoordinates({.x = mouseX, .y = mouseY});

                if (maybeClickedPort.value_or(nullptr) != nullptr) {
                    logger->info("Clicked port '{}' on block {}",
                                 maybeClickedPort.value()->name,
                                 clickedBlock->getSelfId());

                    connectPortsInteraction.handleUserInteractedWith(
                        clickedBlock.get(), maybeClickedPort.value(), windowDelegate, this);

                    // this event should not count as a possible double-click interaction part
                    doubleClickCtLastMouseClickTime = 0;
                } else {
                    logger->info("Clicked block {} for dragging", clickedBlock->getSelfId());
                    draggedBlock = clickedBlock;

                    dragOffset = {.width = mouseX - clickedBlock->getCx(),
                                  .height = mouseY - clickedBlock->getCy()};

                    draggedBlock->onDragStart();

                    // update the last mouse click time for double-click
                    doubleClickCtLastMouseClickTime = time(nullptr);
                }
            } else {
                logger->info("Clicked outside any block");

                // update the last mouse click time for double-click
                doubleClickCtLastMouseClickTime = time(nullptr);
            }
        }
    }

    void BlocksManager::handleMouseUp() {
        if (draggedBlock) {
            logger->info("Finished dragging block {}", draggedBlock->getSelfId());
            draggedBlock->onDragEnd();
            draggedBlock.reset();
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
                gui::window::prompt::MessageBox::showWarning(
                    "Invalid choice",
                    "You entered: " + std::to_string(number) +
                        " which is out of range. Pick a "
                        "number between 1 and " +
                        std::to_string(inputChoiceInteraction->choices.size()),
                    windowDelegate);
            }
        }
    }

    void BlocksManager::handleMouseMove(int x, int y) {
        mouseX = x;
        mouseY = y;

        if (draggedBlock) {
            draggedBlock->onDragProgress(mouseX - dragOffset.width, mouseY - dragOffset.height);
        }
    }

    std::optional<std::shared_ptr<gui::elements::base::BaseBlock>>
    BlocksManager::getBlockAtMousePos() {
        return getBlockAt(mouseX, mouseY);
    }

    std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> BlocksManager::getBlockAt(
        int x, int y) {
        // find the block that is being hovered over; reverse order to render newer blocks on top
        // (as if they had the highest z-index)
        auto blocksReversed = boost::adaptors::reverse(blocks);

        auto maybeHoveredBlockIt =
            std::find_if(blocksReversed.begin(), blocksReversed.end(), [x, y](const auto& block) {
                return block->isHovered(x, y);
            });

        std::optional<std::shared_ptr<gui::elements::base::BaseBlock>> maybeHoveredBlock =
            std::nullopt;

        if (maybeHoveredBlockIt != blocksReversed.end()) {
            maybeHoveredBlock = *maybeHoveredBlockIt;
        }

        return maybeHoveredBlock;
    }

    void BlocksManager::render(SkCanvas* canvas,
                               const geometry::Size2D& size,
                               gui::renderer::delegate::UIRendererDelegate*& uiRendererDelegate) {
        // check if we have an active input choice interaction to render
        if (inputChoiceInteraction.has_value()) {
            std::vector<components::UITextsRow> rows = {
                components::UITextsRow({components::UIText("Choose a block type:",
                                                           components::UIText::Variant::Headline)}),
                components::UITextsRow({
                    components::UIText(
                        "Press a number to choose a block type. Press ESC to cancel.",
                        components::UIText::Variant::MenuCaption),

                }),
                // some extra spacing below the above text
                components::UITextsRow(
                    {components::UIText("", components::UIText::Variant::MenuCaption)})};

            for (const auto& row : inputChoicesUiTextsRows) {
                rows.push_back(row);
            }

            uiRendererDelegate->renderCenteredTextsRows(canvas, size, rows);
        } else {
            // calculate values flow, then render the blocks & friends

            try {
                calculateValuesFlow();
            } catch (const errors::GraphCycleException& e) {
                logger->error("Graph cycle detected: {}", e.what());
            }

            auto maybeHoveredBlock = getBlockAtMousePos();

            if (maybeHoveredBlock.has_value()) {
                hoveredBlock = maybeHoveredBlock.value();  // update the state
            }

            // iterate from oldest to newest (normal order) to render newer ones on top of older
            // ones
            for (const auto& block : blocks) {
                auto isFocused = maybeHoveredBlock.has_value() && (maybeHoveredBlock == block);

                block->render(canvas, mouseX, mouseY, isFocused);
            }

            // render the existing port connections
            for (const auto& [source, destinations] : connectionsRegistry) {
                auto& sourceBlock = source.block;
                auto& sourcePort = source.port;
                auto& sourcePortCoords = sourceBlock->getPortCoordinates(sourcePort);

                for (const auto& dest : destinations) {
                    auto& destBlock = dest.block;
                    auto& destPort = dest.port;

                    auto& destPortCoords = destBlock->getPortCoordinates(destPort);

                    canvas->drawLine(sourcePortCoords.x,
                                     sourcePortCoords.y,
                                     destPortCoords.x,
                                     destPortCoords.y,
                                     connectorPaint);
                }
            }

            // render the current interaction connector line (if applicable)
            maybeRenderDraggedLine(canvas);
        }
    }

    void BlocksManager::maybeRenderDraggedLine(SkCanvas* canvas) {
        // make sure the interaction still references valid objects
        connectPortsInteraction.sanitize();

        if (connectPortsInteraction.isStarted()) {
            auto startSide = connectPortsInteraction.getStartSide();

            auto startSideBlockPtr = startSide.block;
            auto startSidePortPtr = startSide.port;

            if (startSideBlockPtr && startSidePortPtr) {
                auto portCoords = startSideBlockPtr->getPortCoordinates(startSidePortPtr);

                canvas->drawLine(portCoords.x, portCoords.y, mouseX, mouseY, connectorPaint);
            }
        }
    }

    void BlocksManager::onNewBlockChoice(const gui::elements::base::BlockType& blockType) {
        switch (blockType) {
            case gui::elements::base::BlockType::Constant: {
                blocks.push_back(std::make_shared<gui::elements::impl::ConstantBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Add: {
                blocks.push_back(std::make_shared<gui::elements::impl::AddBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Subtract: {
                blocks.push_back(std::make_shared<gui::elements::impl::SubtractBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Multiply: {
                blocks.push_back(std::make_shared<gui::elements::impl::MultiplyBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Divide: {
                blocks.push_back(std::make_shared<gui::elements::impl::DivideBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Power: {
                blocks.push_back(std::make_shared<gui::elements::impl::PowerBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Invert: {
                blocks.push_back(std::make_shared<gui::elements::impl::InvertBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            case gui::elements::base::BlockType::Monitor: {
                blocks.push_back(std::make_shared<gui::elements::impl::MonitorBlock>(
                    mouseX, mouseY, this, this, windowDelegate));
            } break;

            default: {
                auto name = magic_enum::enum_name(blockType);

                logger->error("Unknown user-selected block type: {}", name);

                gui::window::prompt::MessageBox::showWarning(
                    "Invalid choice",
                    "You selected an invalid block type: '" + std::string(name) + "'",
                    windowDelegate);
            } break;
        }
    }

    void BlocksManager::setActiveChoicesInput(
        gui::input::InputChoiceInteraction<gui::elements::base::BlockType>&&
            inputChoiceInteraction) {
        auto choices = inputChoiceInteraction.choices;

        std::vector<std::string> labels;
        labels.reserve(choices.size());
        int index = 0;
        std::for_each(choices.begin(),
                      choices.end(),
                      [&index, &labels](
                          const gui::input::InputChoice<gui::elements::base::BlockType>& choice) {
                          labels.push_back(std::to_string(++index) + " " + choice.displayName);
                      });

        index = 0;  // reset for re-use

        inputChoicesUiTextsRows.reserve(choices.size() / MAX_INPUT_CHOICES_PER_ROW + 1);
        {
            std::vector<components::UIText> rowBuff;
            size_t number = 1;
            for (; number <= choices.size(); number++) {
                rowBuff.push_back(components::UIText(
                    std::to_string(number) + " " + choices[number - 1].displayName,
                    components::UIText::Variant::Choice));

                if (number % MAX_INPUT_CHOICES_PER_ROW == 0) {
                    inputChoicesUiTextsRows.push_back(components::UITextsRow(rowBuff));
                    rowBuff.clear();
                }
            }

            if (!rowBuff.empty()) {
                inputChoicesUiTextsRows.push_back(components::UITextsRow(rowBuff));
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

    bool BlocksManager::hasConnectionBetween(const gui::logic::PortsConnectionSide& source,
                                             const gui::logic::PortsConnectionSide& dest) const {
        gui::logic::PortsConnectionSide variant1 = {.block = source.block, .port = source.port};
        gui::logic::PortsConnectionSide variant2 = {.block = dest.block, .port = dest.port};

        return std::any_of(connectionsRegistry.begin(),
                           connectionsRegistry.end(),
                           [variant1, variant2](const auto& connection) {
                               return connection.first == variant1 &&
                                      connection.second.contains(variant2);
                           });
    }

    bool BlocksManager::isInputConnected(const gui::logic::PortsConnectionSide& side) const {
        return std::any_of(
            connectionsRegistry.begin(), connectionsRegistry.end(), [side](const auto& connection) {
                return connection.first == side || connection.second.contains(side);
            });
    }

    void BlocksManager::onPortsConnected(const gui::logic::PortsConnectionSide& source,
                                         const gui::logic::PortsConnectionSide& dest) {
        connectionsRegistry[source].insert(dest);
    }

    void BlocksManager::onBlockDeleted(const gui::elements::base::BaseBlock* block) {
        // erase entries where the block was the key or the value
        for (auto& [source, destinations] : connectionsRegistry) {
            if (source.block == block) {
                connectionsRegistry.erase(source);
            } else {
                std::erase_if(destinations,
                              [block](const auto& dest) { return dest.block == block; });
            }
        }

        // ensure draggedBlock is not referencing the deleted block
        if (draggedBlock && draggedBlock->getSelfId() == block->getSelfId()) {
            draggedBlock.reset();
        }
    }

    SkPaint BlocksManager::connectorPaint = []() {
        SkPaint paint;

        paint.setColor(colors::PURPLE_BLUE);
        paint.setStrokeWidth(4);
        paint.setAntiAlias(true);

        return paint;
    }();

    void BlocksManager::handleRightClickOnBlock(
        const std::shared_ptr<gui::elements::base::BaseBlock>& block) {
        // check if a port was right-clicked
        auto maybeClickedPort = block->getPortAtCoordinates({.x = mouseX, .y = mouseY});

        if (maybeClickedPort.has_value()) {
            gui::logic::PortsConnectionSide candidate = {.block = block.get(),
                                                         .port = maybeClickedPort.value()};

            // remove all connections from the right-clicked port (if an entry exists, otherwise
            // call does nothing)
            size_t erasedCount = 0;

            for (auto& [source, destinations] : connectionsRegistry) {
                if (source == candidate) {
                    erasedCount += connectionsRegistry.erase(source);
                } else {
                    erasedCount += std::erase_if(
                        destinations, [candidate](const auto& dest) { return dest == candidate; });
                }
            }

            logger->info("Right clicked on port '{}' - removed its {} connections",
                         maybeClickedPort.value()->name,
                         erasedCount);
        } else {
            logger->info("Right clicked on block '{}' - doing nothing", block->getSelfId());
        }
    }

    const std::vector<std::shared_ptr<gui::elements::base::BaseBlock>>& BlocksManager::getBlocks()
        const {
        return blocks;
    }

    const std::unordered_map<gui::logic::PortsConnectionSide,
                             std::unordered_set<gui::logic::PortsConnectionSide>>&
    BlocksManager::getConnectionsRegistry() const {
        return connectionsRegistry;
    }
}  // namespace gui::logic
