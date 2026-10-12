#pragma once

#include <memory>
#include <string>
#include "glm/glm.hpp"

struct GLFWwindow;

class Window {
   public:
    Window(int width, int height, const std::string &title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool shouldClose() const;
    void pollEvents();
    void swapBuffers();

    glm::ivec2 getFramebufferSize() const;
    double getTime() const;

    GLFWwindow *getNativeHandle() const noexcept { return handle.get(); }

   private:
    struct GlfwWindowDeleter {
        void operator()(GLFWwindow *window) const noexcept;
    };
    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> handle;
};
