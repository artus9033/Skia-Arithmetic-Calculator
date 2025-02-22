#ifndef IRenderer_H
#define IRenderer_H

#include "GUI/window/IWindow.h"

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
        explicit IRenderer([[maybe_unused]] gui::window::IWindow* window,
                           [[maybe_unused]] int width,
                           [[maybe_unused]] int height) {};

        virtual ~IRenderer() = default;

        virtual void render() = 0;

        virtual void handleWindowResized(int width, int height) = 0;

        IRenderer(const IRenderer&) = delete;
        IRenderer& operator=(const IRenderer&) = delete;
    };
}  // namespace gui::renderer

#endif  // IRenderer_H
