#ifndef IRenderer_H
#define IRenderer_H

#include "GUI/window/WindowBase.h"

namespace gui::renderer {
    /**
     * @brief Interface (*) for all renderers
     *
     * (*) since virtual constructors are not allowed in interfaces, unfortunately this
     * class-interface's constructor is not virtual, therefore strictly technically speaking,
     * preventing this class from being a pure interface.
     */
    class IRenderer {
       public:
        // since virtual constructors are not allowed in interfaces, unfortunately this
        // constructor is not virtual
        explicit IRenderer([[maybe_unused]] gui::window::WindowBase* window,
                           [[maybe_unused]] int winWidth,
                           [[maybe_unused]] int winHeight,
                           [[maybe_unused]] int fbWidth,
                           [[maybe_unused]] int fbHeight) {};

        virtual ~IRenderer() = default;

        /**
         * @brief Renders the current frame
         */
        virtual void render() = 0;

        /**
         * @brief Handles window resize events
         * @param window The window
         * @param xScale New window x scale
         * @param yScale New window y scale
         */
        virtual void handleWindowResized(gui::window::WindowBase* window,
                                         double xScale,
                                         double yScale) = 0;

        IRenderer(const IRenderer&) = delete;
        IRenderer& operator=(const IRenderer&) = delete;
    };
}  // namespace gui::renderer

#endif  // IRenderer_H
