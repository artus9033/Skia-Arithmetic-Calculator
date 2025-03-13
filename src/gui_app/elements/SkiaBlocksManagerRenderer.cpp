#include "SkiaBlocksManagerRenderer.h"

namespace gui::elements {
    void SkiaBlocksManagerRenderer::maybeRenderDraggedLine(SkCanvas* canvas) {
        // make sure the interaction still references valid objects
        connectPortsInteraction.sanitize();

        if (connectPortsInteraction.isStarted()) {
            auto startSide = connectPortsInteraction.getStartSide();

            const auto* startSideBlockPtr = startSide.block;
            auto startSidePortPtr = startSide.port;

            if (startSideBlockPtr && startSidePortPtr) {
                auto portCoords = startSideBlockPtr->getPortCoordinates(startSidePortPtr);

                canvas->drawLine(portCoords.x, portCoords.y, mouseX, mouseY, connectorPaint);
            }
        }
    }

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

                blockRenderer.render(block.get(), canvas, mouseX, mouseY, isFocused);
            }

            // render the existing port connections
            for (const auto& [source, destinations] : connectionsRegistry) {
                auto& sourceBlock = source.block;
                auto& sourcePort = source.port;
                auto& sourcePortCoords = sourceBlock->getPortCoordinates(sourcePort);

                for (const auto& dest : destinations) {
                    auto& destBlock = dest.block;
                    auto& destPort = dest.port;

                    auto isPartOfCycle =
                        maybeGraphCycle.has_value() &&
                        maybeGraphCycle->contains({.block = destBlock, .port = destPort});

                    auto& destPortCoords = destBlock->getPortCoordinates(destPort);

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
}  // namespace gui::elements
