#pragma once

#include "adapters/gui/guiAdapter.h"

class ImGuiAdapter : public IGuiAdapter {
   public:
    ImGuiAdapter() = default;
    ~ImGuiAdapter() override;

    ImGuiAdapter(const ImGuiAdapter &) = delete;
    ImGuiAdapter &operator=(const ImGuiAdapter &) = delete;

    void init(Window &window) override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    glm::vec2 getDisplaySize() const override;

    bool beginPanel(const std::string &title, const GuiRect &rect) override;
    void endPanel() override;

    void sameLine() override;
    void separator() override;
    glm::vec2 getContentRegionAvail() const override;
    void beginDisabled(bool disabled) override;
    void endDisabled() override;

    void text(const std::string &value) override;
    void textColored(const glm::vec4 &color, const std::string &value) override;
    bool button(const std::string &label) override;
    bool selectable(const std::string &label, bool selected) override;
    bool checkbox(const std::string &label, bool &value) override;
    bool combo(const std::string &label,
               int &current,
               const std::vector<std::string> &items) override;
    bool dragFloat(const std::string &label,
                   float &value,
                   float speed,
                   float min,
                   float max) override;
    bool dragFloat3(const std::string &label, glm::vec3 &value, float speed) override;
    bool dragInt(const std::string &label, int &value, float speed, int min, int max) override;
    bool colorEdit3(const std::string &label, glm::vec3 &value) override;
    bool inputText(const std::string &label, std::string &value) override;
    bool collapsingHeader(const std::string &label) override;

    bool isItemHovered() const override;
    bool isItemActivated() const override;
    bool isItemDeactivatedAfterEdit() const override;
    GuiRect getItemRect() const override;

    void image(GuiTextureId texture, const glm::vec2 &size, bool flipVertically) override;

    InputState getInput() const override;

   private:
    bool initialized = false;
};
