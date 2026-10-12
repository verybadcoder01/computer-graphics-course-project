#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <source_location>
#include "exceptions/window/windowExceptions.h"
#include "window.h"

void Window::GlfwWindowDeleter::operator()(GLFWwindow *window) const noexcept {
    glfwDestroyWindow(window);
}

Window::Window(int width, int height, const std::string &title) {
    if (!glfwInit()) {
        throw WindowCreationException("Failed to initialize GLFW", std::source_location::current());
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));
    if (!handle) {
        glfwTerminate();
        throw WindowCreationException("Failed to create GLFW window",
                                      std::source_location::current());
    }
    glfwMakeContextCurrent(handle.get());
    glfwSwapInterval(1);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        handle.reset();
        glfwTerminate();
        throw WindowCreationException("Failed to initialize GLAD", std::source_location::current());
    }
}

Window::~Window() {
    handle.reset();
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle.get());
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(handle.get());
}

glm::ivec2 Window::getFramebufferSize() const {
    glm::ivec2 size{0};
    glfwGetFramebufferSize(handle.get(), &size.x, &size.y);
    return size;
}

double Window::getTime() const {
    return glfwGetTime();
}
