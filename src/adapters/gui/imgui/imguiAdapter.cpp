#include "imguiAdapter.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <algorithm>
#include <array>
#include "app/window/window.h"

namespace {

ImVec2 toImVec(const glm::vec2 &v) {
    return ImVec2(v.x, v.y);
}

glm::vec2 toGlm(const ImVec2 &v) {
    return glm::vec2(v.x, v.y);
}

constexpr std::array<ImGuiKey, static_cast<size_t>(Key::Count)> keyMap = {
    ImGuiKey_W, ImGuiKey_A,      ImGuiKey_S,     ImGuiKey_D, ImGuiKey_Z,
    ImGuiKey_Y, ImGuiKey_Delete, ImGuiKey_Space, ImGuiKey_F5};

constexpr size_t textBufferSize = 256;

}  // namespace

ImGuiAdapter::~ImGuiAdapter() {
    shutdown();
}

void ImGuiAdapter::init(Window &window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getNativeHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    initialized = true;
}

void ImGuiAdapter::shutdown() {
    if (!initialized) {
        return;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
}

void ImGuiAdapter::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiAdapter::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 ImGuiAdapter::getDisplaySize() const {
    return toGlm(ImGui::GetIO().DisplaySize);
}

bool ImGuiAdapter::beginPanel(const std::string &title, const GuiRect &rect) {
    ImGui::SetNextWindowPos(toImVec(rect.pos));
    ImGui::SetNextWindowSize(toImVec(rect.size));
    constexpr ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;
    return ImGui::Begin(title.c_str(), nullptr, flags);
}

void ImGuiAdapter::endPanel() {
    ImGui::End();
}

void ImGuiAdapter::sameLine() {
    ImGui::SameLine();
}

void ImGuiAdapter::separator() {
    ImGui::Separator();
}

glm::vec2 ImGuiAdapter::getContentRegionAvail() const {
    return toGlm(ImGui::GetContentRegionAvail());
}

void ImGuiAdapter::beginDisabled(bool disabled) {
    ImGui::BeginDisabled(disabled);
}

void ImGuiAdapter::endDisabled() {
    ImGui::EndDisabled();
}

void ImGuiAdapter::text(const std::string &value) {
    ImGui::TextUnformatted(value.c_str());
}

void ImGuiAdapter::textColored(const glm::vec4 &color, const std::string &value) {
    ImGui::TextColored(ImVec4(color.r, color.g, color.b, color.a), "%s", value.c_str());
}

bool ImGuiAdapter::button(const std::string &label) {
    return ImGui::Button(label.c_str());
}

bool ImGuiAdapter::selectable(const std::string &label, bool selected) {
    return ImGui::Selectable(label.c_str(), selected);
}

bool ImGuiAdapter::checkbox(const std::string &label, bool &value) {
    return ImGui::Checkbox(label.c_str(), &value);
}

bool ImGuiAdapter::combo(const std::string &label,
                         int &current,
                         const std::vector<std::string> &items) {
    std::vector<const char *> itemPtrs;
    itemPtrs.reserve(items.size());
    for (const auto &item : items) {
        itemPtrs.push_back(item.c_str());
    }
    return ImGui::Combo(label.c_str(), &current, itemPtrs.data(),
                        static_cast<int>(itemPtrs.size()));
}

bool ImGuiAdapter::dragFloat(const std::string &label,
                             float &value,
                             float speed,
                             float min,
                             float max) {
    return ImGui::DragFloat(label.c_str(), &value, speed, min, max);
}

bool ImGuiAdapter::dragFloat3(const std::string &label, glm::vec3 &value, float speed) {
    return ImGui::DragFloat3(label.c_str(), &value.x, speed);
}

bool ImGuiAdapter::dragInt(const std::string &label, int &value, float speed, int min, int max) {
    return ImGui::DragInt(label.c_str(), &value, speed, min, max);
}

bool ImGuiAdapter::colorEdit3(const std::string &label, glm::vec3 &value) {
    return ImGui::ColorEdit3(label.c_str(), &value.x);
}

bool ImGuiAdapter::inputText(const std::string &label, std::string &value) {
    std::array<char, textBufferSize> buffer{};
    const size_t length = std::min(value.size(), buffer.size() - 1);
    std::copy_n(value.begin(), length, buffer.begin());
    if (ImGui::InputText(label.c_str(), buffer.data(), buffer.size())) {
        value = buffer.data();
        return true;
    }
    return false;
}

bool ImGuiAdapter::collapsingHeader(const std::string &label) {
    return ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
}

bool ImGuiAdapter::isItemHovered() const {
    return ImGui::IsItemHovered();
}

bool ImGuiAdapter::isItemActivated() const {
    return ImGui::IsItemActivated();
}

bool ImGuiAdapter::isItemDeactivatedAfterEdit() const {
    return ImGui::IsItemDeactivatedAfterEdit();
}

GuiRect ImGuiAdapter::getItemRect() const {
    return GuiRect{.pos = toGlm(ImGui::GetItemRectMin()), .size = toGlm(ImGui::GetItemRectSize())};
}

void ImGuiAdapter::image(GuiTextureId texture, const glm::vec2 &size, bool flipVertically) {
    const ImVec2 uv0 = flipVertically ? ImVec2(0, 1) : ImVec2(0, 0);
    const ImVec2 uv1 = flipVertically ? ImVec2(1, 0) : ImVec2(1, 1);
    ImGui::Image(ImTextureRef(static_cast<ImTextureID>(texture)), toImVec(size), uv0, uv1);
}

InputState ImGuiAdapter::getInput() const {
    const ImGuiIO &io = ImGui::GetIO();
    InputState input;
    if (ImGui::IsMousePosValid()) {
        input.mousePos = toGlm(io.MousePos);
    }
    input.mouseDelta = toGlm(io.MouseDelta);
    input.wheel = io.MouseWheel;
    for (size_t i = 0; i < static_cast<size_t>(MouseButton::Count); ++i) {
        input.mouseDown[i] = ImGui::IsMouseDown(static_cast<ImGuiMouseButton>(i));
        input.mouseClicked[i] = ImGui::IsMouseClicked(static_cast<ImGuiMouseButton>(i));
    }
    for (size_t i = 0; i < keyMap.size(); ++i) {
        input.keyDown[i] = ImGui::IsKeyDown(keyMap[i]);
        input.keyPressed[i] = ImGui::IsKeyPressed(keyMap[i], false);
    }
    input.ctrl = io.KeyCtrl;
    input.shift = io.KeyShift;
    input.wantsTextInput = io.WantTextInput;
    return input;
}
