#include "UI.h"

void UI::Register() {
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    Example3::filter = new ImGuiMCP::ImGuiTextFilter();
    Configuration::Example2::Buffer[0] = '\0';
    SKSEMenuFramework::SetSection("SKSE Menu Framework Compiled Example");
    SKSEMenuFramework::AddSectionItem("Font Awesome", Example4::Render);
    SKSEMenuFramework::AddSectionItem("Add Item", Example1::Render);
    SKSEMenuFramework::AddSectionItem("Folder Example/Example 2", Example2::Render);
    UI::Example2::ExampleWindow = SKSEMenuFramework::AddWindow(Example2::RenderWindow, true);
    SKSEMenuFramework::AddSectionItem("Folder Example/Example 3", Example3::Render);

    SKSEMenuFramework::AddSectionItem("Example 5", Example5::Render);
    SKSEMenuFramework::AddHudElement(Example5::RenderOverlay);
    SKSEMenuFramework::AddInputEvent(Example5::OnInput);
    UI::Example5::NonPausingWindow = SKSEMenuFramework::AddWindow(Example5::RenderWindow, false);
}

void UI::Example1::LookupForm() {
    auto addForm = RE::TESForm::LookupByID(AddFormId);
    if (addForm) {
        AddBoundObject = addForm->As<RE::TESBoundObject>();
    } else {
        AddBoundObject = nullptr;
    }
}

void __stdcall UI::Example1::Render() {
    ImGuiMCP::InputScalar("form id", ImGuiMCP::ImGuiDataType_U32, &AddFormId, NULL, NULL, "%08X");

    if (ImGuiMCP::Button("Search")) {
        LookupForm();
    }

    if (AddBoundObject) {
        ImGuiMCP::Text("How much %s would you like to add?", AddBoundObject->GetName());
        ImGuiMCP::SliderInt("number", &Configuration::Example1::Number, 1, 100);
        if (ImGuiMCP::Button("Add")) {
            auto player = RE::PlayerCharacter::GetSingleton()->As<RE::TESObjectREFR>();
            player->AddObjectToContainer(AddBoundObject, nullptr, Configuration::Example1::Number, nullptr);
        }
    } else {
        ImGuiMCP::Text("Form not found");
    }
}

void __stdcall UI::Example2::Render() {
    if (ImGuiMCP::Button("Open Window")) {
        ExampleWindow->IsOpen = true;
    }

    ImGuiMCP::InputText("string", Configuration::Example2::Buffer, 256);
    ImGuiMCP::ColorEdit4("Color", &Configuration::Example2::Color);
    float samples[100];
    for (int n = 0; n < 100; n++) samples[n] = sinf(n * 0.2f + ImGuiMCP::GetTime() * 1.5f);
    ImGuiMCP::PlotLines("Samples", samples, 100);
    ImGuiMCP::TextColored(ImGuiMCP::ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGuiMCP::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++) ImGuiMCP::Text("%04d: Some text", n);
    ImGuiMCP::EndChild();
}

void __stdcall UI::Example2::RenderWindow() {
    auto viewport = ImGuiMCP::GetMainViewport();

    auto center = ImGuiMCP::ImVec2Manager::Create();
    ImGuiMCP::ImGuiViewportManager::GetCenter(center, viewport);
    ImGuiMCP::SetNextWindowPos(*center, ImGuiMCP::ImGuiCond_Appearing, ImGuiMCP::ImVec2{0.5f, 0.5f});
    ImGuiMCP::ImVec2Manager::Destroy(center);
    ImGuiMCP::SetNextWindowSize(ImGuiMCP::ImVec2{viewport->Size.x * 0.4f, viewport->Size.y * 0.4f}, ImGuiMCP::ImGuiCond_Appearing);
    ImGuiMCP::Begin("My First Tool##MenuEntiryFromMod", nullptr,
                    ImGuiMCP::ImGuiWindowFlags_MenuBar);  // If two mods have the same window name, and they open at the
                                                          // same
                                                          // time.
                                                          // The window content will be merged, is good practice to add ##ModName after the window name.
    if (ImGuiMCP::BeginMenuBar()) {
        if (ImGuiMCP::BeginMenu("File")) {
            if (ImGuiMCP::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
            }
            if (ImGuiMCP::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
            }
            if (ImGuiMCP::MenuItem("Close", "Ctrl+W")) {
                ExampleWindow->IsOpen = false;
            }
            ImGuiMCP::EndMenu();
        }
        ImGuiMCP::EndMenuBar();
    }
    if (ImGuiMCP::Button("Close Window")) {
        ExampleWindow->IsOpen = false;
    }
    ImGuiMCP::End();
}

void __stdcall UI::Example3::Render() {
    static ImGuiMCP::ImGuiTableFlags flags = ImGuiMCP::ImGuiTableFlags_Resizable | ImGuiMCP::ImGuiTableFlags_Reorderable | ImGuiMCP::ImGuiTableFlags_Hideable | ImGuiMCP::ImGuiTableFlags_RowBg | ImGuiMCP::ImGuiTableFlags_BordersOuter |
                                             ImGuiMCP::ImGuiTableFlags_BordersV | ImGuiMCP::ImGuiTableFlags_NoBordersInBody | ImGuiMCP::ImGuiTableFlags_ScrollY;

    filter->Draw();

    if (ImGuiMCP::BeginTable("table1", 3, flags)) {
        ImGuiMCP::TableSetupColumn("One");
        ImGuiMCP::TableSetupColumn("Two");
        ImGuiMCP::TableSetupColumn("Three");
        ImGuiMCP::TableHeadersRow();

        for (int row = 0; row < 5; row++) {
            ImGuiMCP::TableNextRow();
            for (int column = 0; column < 3; column++) {
                ImGuiMCP::TableSetColumnIndex(column);
                char buf[32];
                sprintf(buf, "Hello %d,%d", column, row);

                if (filter->PassFilter(buf)) {
                    FontAwesome::PushSolid();
                    ImGuiMCP::Text(CheckMark.c_str());
                    FontAwesome::Pop();
                    ImGuiMCP::SameLine();
                }

                if (column == 2) {
                    ImGuiMCP::Button(buf, ImGuiMCP::ImVec2(-FLT_MIN, 0.0f));
                } else {
                    ImGuiMCP::TextUnformatted(buf);
                }
            }
        }
        ImGuiMCP::EndTable();
    }
}

void __stdcall UI::Example4::Render() {
    FontAwesome::PushBrands();
    ImGuiMCP::Text(TitleText.c_str());
    FontAwesome::Pop();

    FontAwesome::PushSolid();
    ImGuiMCP::Button(Button1Text.c_str());
    FontAwesome::Pop();

    ImGuiMCP::SameLine();

    FontAwesome::PushRegular();
    ImGuiMCP::Button(Button2Text.c_str());
    FontAwesome::Pop();
}

void __stdcall UI::Example5::Render() {
    auto texture = SKSEMenuFramework::LoadTexture("Data\\interface\\unlit-bomb.svg", {100, 100});
    ImGuiMCP::Text("Open non pausing menu: ");
    ImGuiMCP::SameLine();
    if (ImGuiMCP::ImageButton("Open non pausing menu##button", texture, {100, 100})) {
        NonPausingWindow->IsOpen = !NonPausingWindow->IsOpen;
    }
}

void __stdcall UI::Example5::RenderOverlay() {
    if (SKSEMenuFramework::IsAnyBlockingWindowOpened()) {
        return;
    }
    auto drawList = ImGuiMCP::GetForegroundDrawList();
    // ImVec2 center = ImGui::GetIO()->DisplaySize;
    // center.x *= 0.5;
    // center.y *= 0.5;
    // ImGui::ImDrawListManager::AddCircle(drawList, center, 100, IM_COL32(255, 0, 0, 255), 100, 10);

    const char* text = "Press B to toggle the info window";
    ImGuiMCP::ImVec2 textSize;
    ImGuiMCP::CalcTextSize(&textSize, text, 0, false, 0);
    ImGuiMCP::ImVec2 textPos = ImGuiMCP::ImVec2(ImGuiMCP::GetIO()->DisplaySize.x - textSize.x - 20, 20);  // 10px padding from edges
    ImGuiMCP::ImDrawListManager::AddText(drawList, textPos, IM_COL32(255, 255, 255, 255), text);
}

bool __stdcall UI::Example5::OnInput(RE::InputEvent* event) {
    bool blockThisUserInput = false;

    if (event->device == RE::INPUT_DEVICE::kKeyboard) {
        if (auto button = event->AsButtonEvent()) {
            if (button->GetIDCode() == RE::BSWin32KeyboardDevice::Key::kB && button->IsDown()) {
                NonPausingWindow->IsOpen = !NonPausingWindow->IsOpen;
                blockThisUserInput = true;
            }
        }
    }

    return blockThisUserInput;
}

void __stdcall UI::Example5::RenderWindow() {
    auto viewport = ImGuiMCP::GetMainViewport();

    // Position window at top right corner
    ImGuiMCP::ImVec2 windowSize = ImGuiMCP::ImVec2{viewport->Size.x * 0.4f, viewport->Size.y * 0.4f};
    ImGuiMCP::ImVec2 windowPos = ImGuiMCP::ImVec2{viewport->Pos.x + viewport->Size.x - windowSize.x - 20, viewport->Pos.y + 20};

    ImGuiMCP::SetNextWindowPos(windowPos, ImGuiMCP::ImGuiCond_Appearing, {0, 0});
    ImGuiMCP::SetNextWindowSize(windowSize, ImGuiMCP::ImGuiCond_Appearing);

    ImGuiMCP::Begin("My First Overlay Window##MenuEntiryFromMod", nullptr, ImGuiMCP::ImGuiWindowFlags_NoCollapse);
    ImGuiMCP::Text("Chinese characters (requires chinese font): %s", u8"测试");
    // You can load only once if you want, but the texture loader creates a cache of your texutre;
    auto texture = SKSEMenuFramework::LoadTexture("Data\\interface\\unlit-bomb.svg", {100, 100});
    auto texture2 = SKSEMenuFramework::LoadTexture("Data\\interface\\screenshot.png");
    ImGuiMCP::Text("Image Display: ");
    ImGuiMCP::SameLine();
    ImGuiMCP::Image(texture, ImGuiMCP::ImVec2(100, 100));
    ImGuiMCP::Image(texture2, ImGuiMCP::ImVec2(640, 360));
    ImGuiMCP::End();
}
