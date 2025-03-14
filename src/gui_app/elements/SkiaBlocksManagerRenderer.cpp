#include "SkiaBlocksManagerRenderer.h"

namespace gui::elements {
    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    void SkiaBlocksManagerRenderer::maybeRenderDraggedLine(SkCanvas* canvas) {
        // make sure the interaction still references valid objects
        connectPortsInteraction.sanitize();

        if (connectPortsInteraction.isStarted()) {
            // NOLINTNEXTLINE(bugprone-unchecked-optional-access)
            auto startSide = connectPortsInteraction.getStartSide().value();

            const auto* startSideBlockPtr = startSide.block;
            const auto* startSidePortPtr = startSide.port;

            if ((startSideBlockPtr != nullptr) && (startSidePortPtr != nullptr)) {
                auto portCoords = startSideBlockPtr->getPortCoordinates(startSidePortPtr);

                canvas->drawLine(portCoords.x, portCoords.y, mouseX, mouseY, connectorPaint);
            }
        }
    }
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)

    SkPaint SkiaBlocksManagerRenderer::connectorPaint = []() {
        SkPaint paint;

        paint.setColor(colors::PURPLE_BLUE);
        paint.setStrokeWidth(4);
        paint.setAntiAlias(true);

        return paint;
    }();

    SkPaint SkiaBlocksManagerRenderer::cycleConnectorPaint = []() {
        SkPaint paint;

        paint.setColor(colors::RED);
        paint.setStrokeWidth(4);
        paint.setAntiAlias(true);

        return paint;
    }();

    // NOLINTBEGIN(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    void SkiaBlocksManagerRenderer::render(
        SkCanvas* canvas,
        const business_logic::geometry::Size2D& size,
        gui::renderer::delegate::UIRendererDelegate<SkCanvas>*& uiRendererDelegate) {
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

            std::copy(inputChoicesUiTextsRows.begin(),
                      inputChoicesUiTextsRows.end(),
                      std::back_inserter(rows));

            uiRendererDelegate->renderCenteredTextsRows(canvas, size, rows);
        } else {
            // calculate values flow, then render the blocks & friends

            // first, set values to NaNs on all input ports
            // this ensures that the values are not carried over from the previous calculation
            // for ports that may have become disconnected
            for (auto& block : blocks) {
                for (const auto& port : block->getInputPorts()) {
                    block->setPortValue(&port, std::numeric_limits<FloatingPoint>::quiet_NaN());
                }
            }

            // then, make values flow
            try {
                calculateValuesFlow();

                // reset the cycle path
                maybeGraphCycle = std::nullopt;
            } catch (const business_logic::errors::GraphCycleException& e) {
                logger->error("Graph cycle detected: {}", e.what());

                maybeGraphCycle = e.getCyclePath();
            }

            auto maybeHoveredBlock = getBlockAtMousePos();

            // iterate from oldest to newest (normal order) to render newer ones on top of older
            // ones
            for (const auto& block : blocks) {
                auto isFocused = maybeHoveredBlock.has_value() && (maybeHoveredBlock == block);

                SkiaBaseBlockRenderer::render(block.get(), canvas, mouseX, mouseY, isFocused);
            }

            // render the existing port connections
            for (const auto& [source, destinations] : connectionsRegistry) {
                const auto& sourceBlock = source.block;
                const auto& sourcePort = source.port;
                const auto& sourcePortCoords = sourceBlock->getPortCoordinates(sourcePort);

                for (const auto& dest : destinations) {
                    const auto& destBlock = dest.block;
                    const auto& destPort = dest.port;

                    auto isPartOfCycle =
                        maybeGraphCycle.has_value() &&
                        maybeGraphCycle->contains({.block = destBlock, .port = destPort});

                    const auto& destPortCoords = destBlock->getPortCoordinates(destPort);

                    canvas->drawLine(sourcePortCoords.x,
                                     sourcePortCoords.y,
                                     destPortCoords.x,
                                     destPortCoords.y,
                                     isPartOfCycle ? cycleConnectorPaint : connectorPaint);
                }
            }

            // render the current interaction connector line (if applicable)
            maybeRenderDraggedLine(canvas);
        }
    }
    // NOLINTEND(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
}  // namespace gui::elements
