#ifndef UITEXT_H
#define UITEXT_H

#include <cstdint>
#include <string>

/**
 * @brief UI primitives and components to be rendered as part of the UI
 */
namespace business_logic::components {
    /**
     * @brief The UI renderable text primitive
     */
    class UIText {
       public:
        /**
         * @brief The visual display variant of the text
         */
        enum class Variant : std::uint8_t { Headline, MenuCaption, Choice, Caption };

        /**
         * @brief Constructor
         * @param text The text to display
         * @param variant The visual display variant of the text
         */
        UIText(const std::string& text, const Variant& variant);

        virtual ~UIText() = default;

        // disable copy semantics
        UIText(const UIText&) = delete;
        UIText& operator=(const UIText&) = delete;

        // disable move semantics
        UIText(UIText&&) = delete;
        UIText& operator=(UIText&&) = delete;

        /**
         * @brief Gets the text
         * @return The text
         */
        const std::string& getText() const { return text; }

        /**
         * @brief Gets the visual display variant of the text
         * @return The visual display variant of the text
         */
        const Variant& getVariant() const { return variant; }

       private:
        /**
         * @brief The text to display
         */
        std::string text;

        /**
         * @brief The visual display variant of the text
         */
        Variant variant;
    };

}  // namespace business_logic::components
#endif  // UITEXT_H
