#ifndef BUSINESS_LOGIC_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
#define BUSINESS_LOGIC_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H

#include "geometry/Size2D.h"
#include "typenames.h"

/**
 * @brief Delegates exposed by the business_logic module, used for integrating with a GUI / renderer
 * implementation
 */
namespace business_logic::delegate {
    /**
     * @brief The delegate for the window
     *
     * The delegate is used for basic window operations.
     */
    class
        IWindowDelegate {  // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
       public:
        virtual ~IWindowDelegate() = default;

        /**
         * @brief Get the window size
         * @return The window size
         */
        virtual geometry::Size2D getWindowSize() = 0;

        /**
         * @brief Get the framebuffer size
         * @return The framebuffer size
         */
        virtual geometry::Size2D getFramebufferSize() = 0;

        /**
         * @brief Focus the window
         *
         * Helpful e.g. when closing a Qt dialog.
         */
        virtual void focusWindow() = 0;

        /**
         * @brief Show a warning message
         * @param title The title of the warning
         * @param message The message to show
         */
        virtual void showWarning(const std::string& title, const std::string& message) = 0;

        /**
         * @brief Prompt for a floating point input
         * @param title The title of the input
         * @param message The message to show
         * @param defaultValue The default value to use for the input
         * @return The input value or `std::nullopt` if the user cancels the input
         */
        virtual std::optional<FloatingPoint> promptFloatingPointInput(
            const std::string& title,
            const std::string& message,
            const std::optional<FloatingPoint>& defaultValue) = 0;

        /**
         * @brief Prompt for user confirmation
         * @param title The title of the dialog
         * @param message The message to show
         * @return True if the user confirmed, false otherwise
         */
        virtual bool promptConfirmation(const std::string& title, const std::string& message) = 0;
    };
}  // namespace business_logic::delegate

#endif  // BUSINESS_LOGIC_WINDOW_DELEGATE_I_WINDOW_DELEGATE_H
