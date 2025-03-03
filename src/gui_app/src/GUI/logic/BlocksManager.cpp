#include "BlocksManager.h"

namespace gui::logic {
    BlocksManager::BlocksManager(gui::window::delegate::IWindowDelegate* windowDelegate)
        : lastMouseClickTime(0), windowDelegate(windowDelegate) {}

    void BlocksManager::handleMouseDown() {
        // do not handle mouse down if we have an active choices input
        if (hasActiveChoicesInput()) {
            logger->info("Skipping mouse down because we have an active choices input");

            return;
        }

        lastMouseClickTime = time(nullptr);

        auto maybeClickedBlock = getBlockAtMousePos();

        if (maybeClickedBlock.has_value()) {
            auto clickedBlock = maybeClickedBlock.value();

            auto maybeClickedPort = clickedBlock->getPortAtCoordinates({.x = mouseX, .y = mouseY});

            if (maybeClickedPort.value_or(nullptr) != nullptr) {
                logger->info("Clicked port '{}' on block {}",
                             maybeClickedPort.value()->name,
                             clickedBlock->getSelfId());

                connectPortsInteraction.handleUserInteractedWith(
                    clickedBlock.get(), maybeClickedPort.value(), windowDelegate, this);
            } else {
                logger->warn("Clicked block {}, but not a port of it", clickedBlock->getSelfId());
            }
        } else {
            logger->info("Clicked outside any block");
        }
    }

    void BlocksManager::handleMouseUp() { lastMouseClickTime = 0; }

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
                MessageBox::showWarning("Invalid choice",
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
            // render the blocks & friends
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
            for (const auto& [source, dest] : connectionsRegistry) {
                auto sourceBlock = source.block;
                auto destBlock = dest.block;
                auto sourcePort = source.port;
                auto destPort = dest.port;

                auto sourcePortCoords = sourceBlock->getPortCoordinates(sourcePort);
                auto destPortCoords = destBlock->getPortCoordinates(destPort);

                canvas->drawLine(sourcePortCoords.x,
                                 sourcePortCoords.y,
                                 destPortCoords.x,
                                 destPortCoords.y,
                                 connectorPaint);
            }

            // render the current interaction connector line (if applicable)
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
                    mouseX, mouseY, this, this, windowDelegate->getWindowSize()));
            } break;

                // case gui::elements::base::BlockType::Add: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::AddBlock>(mouseX, mouseY, this));
                // } break;

                // case gui::elements::base::BlockType::Subtract: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::SubtractBlock>(mouseX, mouseY,
                //         this));
                // } break;

                // case gui::elements::base::BlockType::Multiply: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::MultiplyBlock>(mouseX, mouseY,
                //         this));
                // } break;

                // case gui::elements::base::BlockType::Divide: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::DivideBlock>(mouseX, mouseY,
                //         this));
                // } break;

                // case gui::elements::base::BlockType::Power: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::PowerBlock>(mouseX, mouseY, this));
                // } break;

                // case gui::elements::base::BlockType::Invert: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::InvertBlock>(mouseX, mouseY,
                //         this));
                // } break;

            case gui::elements::base::BlockType::Monitor: {
                blocks.push_back(std::make_shared<gui::elements::impl::MonitorBlock>(
                    mouseX, mouseY, this, this, windowDelegate->getWindowSize()));
            } break;

            default: {
                auto name = magic_enum::enum_name(blockType);

                logger->error("Unknown user-selected block type: {}", name);

                MessageBox::showWarning(
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
        auto maybeConnectionIt = connectionsRegistry.find(source);

        if (maybeConnectionIt != connectionsRegistry.end()) {
            if (maybeConnectionIt->second == dest) {
                return true;
            }
        }

        return false;
    }

    void BlocksManager::onPortsConnected(const gui::logic::PortsConnectionSide& source,
                                         const gui::logic::PortsConnectionSide& dest) {
        connectionsRegistry[source] = dest;
    }

    void BlocksManager::onBlockDeleted(const gui::elements::base::BaseBlock* block) {
        // erase entries where the block was the key or the value
        for (const auto& [source, dest] : connectionsRegistry) {
            if (source.block == block || dest.block == block) {
                connectionsRegistry.erase(source);
            }
        }
    }

    SkPaint BlocksManager::connectorPaint = []() {
        SkPaint paint;

        paint.setColor(colors::PURPLE_BLUE);
        paint.setStrokeWidth(4);
        paint.setAntiAlias(true);

        return paint;
    }();

}  // namespace gui::logic
