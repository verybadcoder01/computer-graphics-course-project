#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "glm/glm.hpp"

class Window;

struct GuiRect {
    glm::vec2 pos{0};
    glm::vec2 size{0};
};

enum class MouseButton { Left = 0, Right, Middle, Count };
enum class Key { W = 0, A, S, D, Z, Y, Delete, Space, F5, Count };

struct InputState {
    glm::vec2 mousePos{0};
    glm::vec2 mouseDelta{0};
    float wheel = 0;
    std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseDown{};
    std::array<bool, static_cast<size_t>(MouseButton::Count)> mouseClicked{};
    std::array<bool, static_cast<size_t>(Key::Count)> keyDown{};
    std::array<bool, static_cast<size_t>(Key::Count)> keyPressed{};
    bool ctrl = false;
    bool shift = false;
    bool wantsTextInput = false;

    bool isDown(MouseButton b) const { return mouseDown[static_cast<size_t>(b)]; }
    bool isClicked(MouseButton b) const { return mouseClicked[static_cast<size_t>(b)]; }
    bool isDown(Key k) const { return keyDown[static_cast<size_t>(k)]; }
    bool isPressed(Key k) const { return keyPressed[static_cast<size_t>(k)]; }
};

using GuiTextureId = uint64_t;

// Widget-level immediate-mode GUI interface. Labels double as widget ids; a "##suffix" makes a
// label unique without changing the visible text. All sizes and positions are in GUI (logical)
// pixels.
class IGuiAdapter {
   public:
    virtual ~IGuiAdapter() = default;

    virtual void init(Window &window) = 0;
    virtual void shutdown() = 0;
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual glm::vec2 getDisplaySize() const = 0;

    virtual bool beginPanel(const std::string &title, const GuiRect &rect) = 0;
    virtual void endPanel() = 0;

    virtual void sameLine() = 0;
    virtual void separator() = 0;
    virtual glm::vec2 getContentRegionAvail() const = 0;
    virtual void beginDisabled(bool disabled) = 0;
    virtual void endDisabled() = 0;

    virtual void text(const std::string &value) = 0;
    virtual void textColored(const glm::vec4 &color, const std::string &value) = 0;
    virtual bool button(const std::string &label) = 0;
    virtual bool selectable(const std::string &label, bool selected) = 0;
    virtual bool checkbox(const std::string &label, bool &value) = 0;
    virtual bool combo(const std::string &label,
                       int &current,
                       const std::vector<std::string> &items) = 0;
    virtual bool dragFloat(const std::string &label,
                           float &value,
                           float speed = 0.1f,
                           float min = 0.0f,
                           float max = 0.0f) = 0;
    virtual bool dragFloat3(const std::string &label, glm::vec3 &value, float speed = 0.1f) = 0;
    virtual bool dragInt(const std::string &label,
                         int &value,
                         float speed = 1.0f,
                         int min = 0,
                         int max = 0) = 0;
    virtual bool colorEdit3(const std::string &label, glm::vec3 &value) = 0;
    virtual bool inputText(const std::string &label, std::string &value) = 0;
    virtual bool collapsingHeader(const std::string &label) = 0;

    virtual bool isItemHovered() const = 0;
    virtual bool isItemActivated() const = 0;
    virtual bool isItemDeactivatedAfterEdit() const = 0;
    virtual GuiRect getItemRect() const = 0;

    virtual void image(GuiTextureId texture, const glm::vec2 &size, bool flipVertically) = 0;

    virtual InputState getInput() const = 0;
};
