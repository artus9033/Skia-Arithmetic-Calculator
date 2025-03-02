#include "SkiaRendererImpl.h"

namespace gui::renderer {
    SkiaRendererImpl::SkiaRendererImpl(gui::window::WindowBase* window,
                                       int winWidth,
                                       int winHeight,
                                       int fbWidth,
                                       int fbHeight,
                                       std::shared_ptr<gui::logic::BlocksManager> blocksManager)
        : IRenderer(window, winWidth, winHeight, fbWidth, fbHeight),
          delegate::UIRendererDelegate(),
          business_logic::Loggable<SkiaRendererImpl>(),
          blocksManager(blocksManager),
          window(window),
          uiRendererDelegatePtr(static_cast<delegate::UIRendererDelegate*>(this)) {
        if (!window) {
            throw std::runtime_error("Invalid window handle provided to SkiaRendererImpl");
        }

        reinitializeSurface();

        if (skSurface == nullptr) {
            throw std::runtime_error(
                "Failed to create Skia surface (SkSurfaces::RenderTarget returned null)");
        }

        /** Text font paint - start */

        // black stroke
        textFontStrokePaint.setColor(colors::TEXT_OUTLINE_COLOR);
        textFontStrokePaint.setStyle(SkPaint::kStroke_Style);
        textFontStrokePaint.setStrokeWidth(4);
        textFontStrokePaint.setAntiAlias(true);

        // white fill
        textFontFillPaint.setColor(colors::TEXT_COLOR);
        textFontFillPaint.setStyle(SkPaint::kFill_Style);
        textFontFillPaint.setAntiAlias(true);

        /** Text font paint - end */

        /** Text font - start */
        sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_CoreText(nullptr);
        auto typeface = fontMgr->legacyMakeTypeface(nullptr, SkFontStyle::Normal());

        inputHeadlineFont.setSize(INPUT_HEADLINE_FONT_SIZE_BASE);
        inputHeadlineFont.setTypeface(typeface);
        inputHeadlineFont.setEmbolden(true);

        inputCaptionFont.setSize(INPUT_CAPTION_FONT_SIZE_BASE);
        inputCaptionFont.setTypeface(typeface);

        inputChoiceFont.setSize(INPUT_CHOICE_FONT_SIZE_BASE);
        inputChoiceFont.setTypeface(typeface);
        /** Text font - end */
    }

    void SkiaRendererImpl::reinitializeSurface() {
        auto windowSize = window->getWindowSize();
        logger->info("Reinitializing Skia surface with width {} and height {}",
                     windowSize.width,
                     windowSize.height);

        sk_sp<const GrGLInterface> interface = GrGLMakeNativeInterface();
        if (!interface) {
            throw std::runtime_error("Failed to create GL interface");
        }

        grContext = GrDirectContexts::MakeGL(interface);

        GrGLFramebufferInfo framebufferInfo;
        framebufferInfo.fFBOID = 0;
        framebufferInfo.fFormat = GL_RGBA8;

        auto framebufferSize = window->getFramebufferSize();
        GrBackendRenderTarget renderTarget = GrBackendRenderTargets::MakeGL(
            framebufferSize.width, framebufferSize.height, 0, 8, framebufferInfo);

        skSurface = SkSurfaces::WrapBackendRenderTarget(grContext.get(),
                                                        renderTarget,
                                                        kBottomLeft_GrSurfaceOrigin,
                                                        kRGBA_8888_SkColorType,
                                                        nullptr,
                                                        nullptr);
    }

    SkiaRendererImpl::~SkiaRendererImpl() {
        skSurface.reset();
        grContext.reset();
    }

    void SkiaRendererImpl::render() {
        SkCanvas* canvas = skSurface->getCanvas();
        canvas->clear(colors::WINDOW_BACKGROUND_COLOR);

        blocksManager->render(canvas, window->getWindowSize(), uiRendererDelegatePtr);

        grContext->flush();
    }

    void SkiaRendererImpl::handleWindowResized([[maybe_unused]] gui::window::WindowBase* window,
                                               double xScale,
                                               double yScale) {
        reinitializeSurface();

        skSurface->getCanvas()->scale(xScale, yScale);

        // recalculate the sizes of fonts
        inputHeadlineFont.setSize(INPUT_HEADLINE_FONT_SIZE_BASE * xScale);
        inputCaptionFont.setSize(INPUT_CAPTION_FONT_SIZE_BASE * xScale);
        inputChoiceFont.setSize(INPUT_CHOICE_FONT_SIZE_BASE * xScale);
    }

    void SkiaRendererImpl::renderCenteredTextsRows(
        SkCanvas* canvas,
        const geometry::Size2D& size,
        const std::vector<components::UITextsRow>& rows) {
        // calculate a list of heights of rows (each height is the max height of a text in the row)
        auto rowsHeights = std::vector<SkScalar>(rows.size());
        std::transform(
            rows.begin(),
            rows.end(),
            rowsHeights.begin(),
            [this](const components::UITextsRow& row) {
                SkScalar maxHeight = 0.0f;
                for (const auto& text : row.getUiTexts()) {
                    maxHeight =
                        std::max(maxHeight, getFontForLabelType(text.getVariant()).getSize());
                }

                return maxHeight *
                       (1 + CENTERED_TEXT_ROWS_MARGIN_VERTICAL_NORM_PERCENT);  // apply a margin
                                                                               // between rows
            });

        auto sumOfHeights =
            std::accumulate(rowsHeights.begin(), rowsHeights.end(), static_cast<SkScalar>(0.0f));

        auto screenCenterX = static_cast<SkScalar>(size.width) / 2.0f;
        auto screenCenterY = static_cast<SkScalar>(size.height) / 2.0f;

        // Y start, will be updated as we render each row
        auto renderY = screenCenterY -
                       sumOfHeights * (1.0f + CENTERED_TEXT_ROWS_MARGIN_VERTICAL_NORM_PERCENT) /
                           2.0f;  // apply a margin
                                  // between rows

        int rowIndex = 0;
        for (const auto& row : rows) {
            // calculate a list of widths of rows (each width is the sum of the widths of the texts
            // in the row)
            auto textsWidths = std::vector<SkScalar>(row.getUiTexts().size());
            std::transform(row.getUiTexts().begin(),
                           row.getUiTexts().end(),
                           textsWidths.begin(),
                           [this](const components::UIText& uiText) {
                               return getFontForLabelType(uiText.getVariant())
                                   .measureText(uiText.getText().c_str(),
                                                uiText.getText().length(),
                                                SkTextEncoding::kUTF8);
                           });

            const auto sumOfWidths = std::accumulate(
                textsWidths.begin(), textsWidths.end(), static_cast<SkScalar>(0.0f));

            // X start, will be updated as we render each text
            auto renderX =
                screenCenterX - sumOfWidths *
                                    (1.0f + CENTERED_TEXT_ROWS_MARGIN_HORIZONTAL_NORM_PERCENT) /
                                    2.0f;  // apply a margin
                                           // between texts

            auto halfMarginY =
                rowsHeights[rowIndex] * CENTERED_TEXT_ROWS_MARGIN_VERTICAL_NORM_PERCENT / 2.0f;

            // apply 1st half of the margin between rows
            renderY += halfMarginY;

            // render the texts in this row
            int textIndex = 0;
            for (const auto& uiText : row.getUiTexts()) {
                auto font = getFontForLabelType(uiText.getVariant());
                auto text = uiText.getText();

                auto halfMarginX = textsWidths[textIndex] *
                                   CENTERED_TEXT_ROWS_MARGIN_HORIZONTAL_NORM_PERCENT / 2.0f;
                // apply 1st half of the margin before the element
                renderX += halfMarginX;

                // render stroke
                canvas->drawString(text.c_str(), renderX, renderY, font, textFontStrokePaint);

                // render fill on top of stroke
                canvas->drawString(text.c_str(), renderX, renderY, font, textFontFillPaint);

                // apply 2nd half of the margin after the element
                renderX += textsWidths[textIndex] + halfMarginX;

                textIndex++;
            }

            // apply 2nd half of the margin between rows
            renderY += rowsHeights[rowIndex] + halfMarginY;

            rowIndex++;
        }
    }

    const SkFont& SkiaRendererImpl::getFontForLabelType(
        const components::UIText::Variant& variant) const {
        switch (variant) {
            case components::UIText::Variant::Headline:
                return inputHeadlineFont;

            case components::UIText::Variant::Caption:
                return inputCaptionFont;

            case components::UIText::Variant::Choice:
                return inputChoiceFont;
        }
    }
}  // namespace gui::renderer
