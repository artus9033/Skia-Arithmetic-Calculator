#ifndef IWindow_H
#define IWindow_H

namespace gui::window {
    class IWindow {
       public:
        virtual ~IWindow() = default;

        virtual void run() = 0;

        virtual bool shouldClose() const = 0;
    };
}  // namespace gui::window

#endif  // IWindow_H
