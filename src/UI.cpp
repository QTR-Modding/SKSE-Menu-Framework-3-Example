#include "UI.h"

void UI::Register() {
    if (!SKSEMenuFramework::IsInstalled()) {
        return;
    }
    Example3::filter = new ImGuiTextFilter();
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
    ImGui::InputScalar("form id", ImGuiDataType_U32, &AddFormId, NULL, NULL, "%08X");

    if (ImGui::Button("Search")) {
        LookupForm();
    }

    if (AddBoundObject) {
        ImGui::Text("How much %s would you like to add?", AddBoundObject->GetName());
        ImGui::SliderInt("number", &Configuration::Example1::Number, 1, 100);
        if (ImGui::Button("Add")) {
            auto player = RE::PlayerCharacter::GetSingleton()->As<RE::TESObjectREFR>();
            player->AddObjectToContainer(AddBoundObject, nullptr, Configuration::Example1::Number, nullptr);
        }
    } else {
        ImGui::Text("Form not found");
    }
}

void __stdcall UI::Example2::Render() {
    if (ImGui::Button("Open Window")) {
        ExampleWindow->IsOpen = true;
    }

    ImGui::InputText("string", Configuration::Example2::Buffer, 256);
    ImGui::ColorEdit4("Color", &Configuration::Example2::Color);
    float samples[100];
    for (int n = 0; n < 100; n++) samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++) ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
}

void __stdcall UI::Example2::RenderWindow() {
    auto viewport = ImGui::GetMainViewport();

    auto center = ImGui::ImVec2Manager::Create();
    ImGui::ImGuiViewportManager::GetCenter(center, viewport);
    ImGui::SetNextWindowPos(*center, ImGuiCond_Appearing, ImVec2{0.5f, 0.5f});
    ImGui::ImVec2Manager::Destroy(center);
    ImGui::SetNextWindowSize(ImVec2{viewport->Size.x * 0.4f, viewport->Size.y * 0.4f}, ImGuiCond_Appearing);
    ImGui::Begin("My First Tool##MenuEntiryFromMod",nullptr, ImGuiWindowFlags_MenuBar); // If two mods have the same window name, and they open at the same time.
                                                                                         // The window content will be merged, is good practice to add ##ModName after the window name.
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W")) {
                ExampleWindow->IsOpen = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    if (ImGui::Button("Close Window")) {
        ExampleWindow->IsOpen = false;
    }
    ImGui::End();
}

void __stdcall UI::Example3::Render() {
    static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable |
                                   ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                   ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;

    filter->Draw();

    if (ImGui::BeginTable("table1", 3, flags)) {
        ImGui::TableSetupColumn("One");
        ImGui::TableSetupColumn("Two");
        ImGui::TableSetupColumn("Three");
        ImGui::TableHeadersRow();

        for (int row = 0; row < 5; row++) {
            ImGui::TableNextRow();
            for (int column = 0; column < 3; column++) {
                ImGui::TableSetColumnIndex(column);
                char buf[32];
                sprintf(buf, "Hello %d,%d", column, row);

                if (filter->PassFilter(buf)) {
                    FontAwesome::PushSolid();
                    ImGui::Text(CheckMark.c_str());
                    FontAwesome::Pop();
                    ImGui::SameLine();
                }

                if (column == 2) {
                    ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
                } else {
                    ImGui::TextUnformatted(buf);
                }
            }
        }
        ImGui::EndTable();
    }
}

void __stdcall UI::Example4::Render() {
    FontAwesome::PushBrands();
    ImGui::Text(TitleText.c_str());
    FontAwesome::Pop();

    FontAwesome::PushSolid();
    ImGui::Button(Button1Text.c_str());
    FontAwesome::Pop();

    ImGui::SameLine();

    FontAwesome::PushRegular();
    ImGui::Button(Button2Text.c_str());
    FontAwesome::Pop();
}

void __stdcall UI::Example5::Render() {
    auto texture = SKSEMenuFramework::LoadTexture("Data\\interface\\unlit-bomb.svg", {100, 100});
    ImGui::Text("Open non pausing menu: ");
    ImGui::SameLine();
    if (ImGui::ImageButton("Open non pausing menu##button", texture, {100, 100})) {
        NonPausingWindow->IsOpen = !NonPausingWindow->IsOpen;
    }
}

void __stdcall UI::Example5::RenderOverlay() {
    if (SKSEMenuFramework::IsAnyBlockingWindowOpened()) {
        return;
    }
    auto drawList = ImGui::GetForegroundDrawList(); 
    //ImVec2 center = ImGui::GetIO()->DisplaySize;
    //center.x *= 0.5;
    //center.y *= 0.5;
    //ImGui::ImDrawListManager::AddCircle(drawList, center, 100, IM_COL32(255, 0, 0, 255), 100, 10);

    const char* text = "Press B to toggle the info window";
    ImVec2 textSize;
    ImGui::CalcTextSize(&textSize, text,0, false, 0);
    ImVec2 textPos = ImVec2(ImGui::GetIO()->DisplaySize.x - textSize.x - 20, 20);  // 10px padding from edges
    ImGui::ImDrawListManager::AddText(drawList, textPos, IM_COL32(255, 255, 255, 255), text);
}

bool __stdcall UI::Example5::OnInput(RE::InputEvent* event) { 
    bool blockUserInput = false;

    if (event->device == RE::INPUT_DEVICE::kKeyboard) {
        if (auto button = event->AsButtonEvent()) {
            if (button->GetIDCode() == RE::BSWin32KeyboardDevice::Key::kB && button->IsDown()) {
                NonPausingWindow->IsOpen = !NonPausingWindow->IsOpen;
                blockUserInput = true;
            }
        }
    }

    return blockUserInput;
}

void __stdcall UI::Example5::RenderWindow() {
    auto viewport = ImGui::GetMainViewport();

    // Position window at top right corner
    ImVec2 windowSize = ImVec2{viewport->Size.x * 0.4f, viewport->Size.y * 0.4f};
    ImVec2 windowPos = ImVec2{viewport->Pos.x + viewport->Size.x - windowSize.x - 20, viewport->Pos.y + 20};

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Appearing, {0,0});
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Appearing);

    ImGui::Begin("My First Overlay Window##MenuEntiryFromMod", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Chinese characters (requires chinese font): %s", u8"测试");
    //You can load only once if you want, but the texture loader creates a cache of your texutre;
    auto texture = SKSEMenuFramework::LoadTexture("Data\\interface\\unlit-bomb.svg", {100, 100});
    auto texture2 = SKSEMenuFramework::LoadTexture("Data\\interface\\screenshot.png");
    ImGui::Text("Image Display: ");
    ImGui::SameLine();
    ImGui::Image(texture, ImVec2(100, 100));
    ImGui::Image(texture2, ImVec2(640, 360));
    ImGui::End();
}
