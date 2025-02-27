#include "BlocksManager.h"

#include "MessageBox.h"

namespace gui::logic {

    BlocksManager::BlocksManager(gui::window::delegate::IWindowDelegate* windowDelegate)
        : lastMouseClickTime(0), windowDelegate(windowDelegate) {}

    void BlocksManager::handleMouseDown() { lastMouseClickTime = time(nullptr); }

    void BlocksManager::handleMouseUp() { lastMouseClickTime = 0; }

    void BlocksManager::handleEscapeKeyPress() {
        logger->info("Escape key pressed");

        clearActiveChoicesInput();
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
                MessageBox::showInfo("Invalid choice",
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
            std::vector<gui::renderer::components::UITextsRow> rows = {
                gui::renderer::components::UITextsRow({gui::renderer::components::UIText(
                    "Choose a block type:", gui::renderer::components::UIText::Variant::Headline)}),
                gui::renderer::components::UITextsRow({
                    gui::renderer::components::UIText(
                        "Press a number to choose a block type. Press ESC to cancel.",
                        gui::renderer::components::UIText::Variant::Caption),

                }),
                // some extra spacing below the above text
                gui::renderer::components::UITextsRow({gui::renderer::components::UIText(
                    "", gui::renderer::components::UIText::Variant::Caption)})};

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

                block->render(canvas, isFocused);
            }

            maybeRenderDraggedLine(canvas);
        }
    }

    void BlocksManager::maybeRenderDraggedLine(SkCanvas* canvas) const {
        if (lastMouseClickTime > 0) {
            SkPaint paint;
            paint.setColor(SK_ColorGREEN);
            canvas->drawLine(0, 0, mouseX, mouseY, paint);
        }
    }

    void BlocksManager::onNewBlockChoice(const gui::elements::base::BlockType& blockType) {
        switch (blockType) {
            case gui::elements::base::BlockType::Constant: {
                blocks.push_back(std::make_shared<gui::elements::impl::ConstantBlock>(
                    mouseX, mouseY, this, windowDelegate->getWindowSize()));
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

                // case gui::elements::base::BlockType::Monitor: {
                //     blocks.push_back(
                //         std::make_shared<gui::elements::impl::MonitorBlock>(mouseX, mouseY,
                //         this));
                // } break;

            default: {
                auto name = magic_enum::enum_name(blockType);

                logger->error("Unknown user-selected block type: {}", name);

                MessageBox::showInfo(
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
            std::vector<gui::renderer::components::UIText> rowBuff;
            size_t number = 1;
            for (; number <= choices.size(); number++) {
                rowBuff.push_back(gui::renderer::components::UIText(
                    std::to_string(number) + " " + choices[number - 1].displayName,
                    gui::renderer::components::UIText::Variant::Choice));

                if (number % MAX_INPUT_CHOICES_PER_ROW == 0) {
                    inputChoicesUiTextsRows.push_back(
                        gui::renderer::components::UITextsRow(rowBuff));
                    rowBuff.clear();
                }
            }

            if (!rowBuff.empty()) {
                inputChoicesUiTextsRows.push_back(gui::renderer::components::UITextsRow(rowBuff));
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

}  // namespace gui::logic
