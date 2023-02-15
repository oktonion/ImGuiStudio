#include <ImGuiStudio.h>
#include "../backends/glfw/include/GLFW/glfw3.h"


#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#undef GL_VERSION_1_1
#include <ImGui/backends/imgui_impl_opengl3_loader.h>

#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>


namespace ImGuiStudioBackend
{
    enum {
        WindowPosX = 0,
        WindowPosY = 0,
        WindowWidth = 1280,
        WindowHeight = 720
    };
    GLFWwindow* NativeWindow()
    {
        struct lambdas
        {
            static GLFWwindow* glfwWindow()
            {
                {
                    struct glfw
                    {
                        static void ErrorCallbackThrow(int error, const char* description)
                        {
                            std::string msg = "glfw error:";
                            msg = msg +
                                "'" + description + "' (" + std::to_string(error) + ")";
                            throw(std::logic_error(msg.c_str()));
                        }
                    };
                    glfwSetErrorCallback(glfw::ErrorCallbackThrow);
                }

                if (!glfwInit())
                    throw std::logic_error("glfw error: glfwInit() failed");

                
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                glfwWindowHint(GLFW_DECORATED, 0);
                glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

                return glfwCreateWindow(WindowWidth, WindowHeight, "", nullptr, nullptr);
            }
        };

        static GLFWwindow* result = lambdas::glfwWindow();

        return result;
    }

    enum Style{
        Default_Style,
        Win98_Style,
        Win10_Style
    };

    void Init(int, char**, Style style = Default_Style)
    {
        //#ifndef _DEBUG
        //    ShowWindow( GetConsoleWindow( ), 0 );
        //#endif // !_DEBUG

        if (!NativeWindow())
            throw(std::logic_error("Can't create native glfw window!"));

        glfwMakeContextCurrent(NativeWindow());

        glfwSwapInterval(1);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
        bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
        bool err = glewInit() != GLEW_OK;
#else
        bool err = imgl3wInit() != 0;
#endif
        //if ( err )
        //{
        //    fprintf( stderr, "Failed to initialize OpenGL loader!\n" );
        //    return;
        //}

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigWindowsMoveFromTitleBarOnly = true;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(NativeWindow(), true);

#ifdef _OPENGL2
        ImGui_ImplOpenGL2_Init();
#else
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
#endif
        {
            struct glfw
            {
                static void ErrorCallbackNoThrow(int error, const char* description)
                {
                    std::cout << "glfw error: '" << description << "(" << error << ")" << std::endl;
                }
            };
            glfwSetErrorCallback(glfw::ErrorCallbackNoThrow);
        }

        if (style == Win10_Style)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->Clear();
            ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
            if (font != NULL) {
                io.FontDefault = font;
            }
            else {
                io.Fonts->AddFontDefault();
            }
            io.Fonts->Build();

            ImGuiStyle* style = &ImGui::GetStyle();
            int hspacing = 8;
            int vspacing = 6;
            style->DisplaySafeAreaPadding = ImVec2(0, 0);
            style->WindowPadding = ImVec2(hspacing / 2, vspacing);
            style->FramePadding = ImVec2(hspacing, vspacing);
            style->ItemSpacing = ImVec2(hspacing, vspacing);
            style->ItemInnerSpacing = ImVec2(hspacing, vspacing);
            style->IndentSpacing = 20.0f;

            style->WindowRounding = 0.0f;
            style->FrameRounding = 0.0f;

            style->WindowBorderSize = 0.0f;
            style->FrameBorderSize = 1.0f;
            style->PopupBorderSize = 1.0f;

            style->ScrollbarSize = 20.0f;
            style->ScrollbarRounding = 0.0f;
            style->GrabMinSize = 5.0f;
            style->GrabRounding = 0.0f;

            ImVec4 white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            ImVec4 transparent = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            ImVec4 dark = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
            ImVec4 darker = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);

            ImVec4 background = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
            ImVec4 text = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            ImVec4 border = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            ImVec4 grab = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
            ImVec4 header = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            ImVec4 active = ImVec4(0.00f, 0.47f, 0.84f, 1.00f);
            ImVec4 hover = ImVec4(0.00f, 0.47f, 0.84f, 0.20f);

            style->Colors[ImGuiCol_Text] = text;
            style->Colors[ImGuiCol_WindowBg] = background;
            style->Colors[ImGuiCol_ChildBg] = background;
            style->Colors[ImGuiCol_PopupBg] = white;

            style->Colors[ImGuiCol_Border] = border;
            style->Colors[ImGuiCol_BorderShadow] = transparent;

            style->Colors[ImGuiCol_Button] = header;
            style->Colors[ImGuiCol_ButtonHovered] = hover;
            style->Colors[ImGuiCol_ButtonActive] = active;

            style->Colors[ImGuiCol_FrameBg] = white;
            style->Colors[ImGuiCol_FrameBgHovered] = hover;
            style->Colors[ImGuiCol_FrameBgActive] = active;

            style->Colors[ImGuiCol_MenuBarBg] = header;
            style->Colors[ImGuiCol_Header] = header;
            style->Colors[ImGuiCol_HeaderHovered] = hover;
            style->Colors[ImGuiCol_HeaderActive] = active;

            style->Colors[ImGuiCol_CheckMark] = text;
            style->Colors[ImGuiCol_SliderGrab] = grab;
            style->Colors[ImGuiCol_SliderGrabActive] = darker;


            style->Colors[ImGuiCol_ScrollbarBg] = header;
            style->Colors[ImGuiCol_ScrollbarGrab] = grab;
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = dark;
            style->Colors[ImGuiCol_ScrollbarGrabActive] = darker;
        }
        else if (style == Win98_Style)
        {
            ImGuiStyle* style = &ImGui::GetStyle();
            style->FrameBorderSize = 1.0f;
            style->FramePadding = ImVec2(4.0f, 4.0f);
            style->WindowMenuButtonPosition = ImGuiDir_Right;
            style->ScrollbarSize = 16.0f;

            ImVec4* colors = style->Colors;

            colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            colors[ImGuiCol_WindowBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_ChildBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_PopupBg] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
            colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
            colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
            colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.50f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
            colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
            colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
            colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
            colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
            colors[ImGuiCol_SliderGrabActive] = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
            colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
            colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
            colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
            colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
            colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
            colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
            colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
            colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.80f, 0.80f, 0.56f);
            colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_Tab] = ImVec4(0.76f, 0.80f, 0.84f, 0.95f);
            colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_TabActive] = ImVec4(0.60f, 0.73f, 0.88f, 0.95f);
            colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.92f, 0.94f, 0.95f);
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.74f, 0.82f, 0.91f, 1.00f);
            colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
            colors[ImGuiCol_DragDropTarget] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);


            // Fonts + Icons
            ImGuiIO& io = ImGui::GetIO();
            for (int i = 0; i < io.Fonts->ConfigData.size(); i++) {
                if (strcmp(io.Fonts->ConfigData[i].Name, "MS Sans Serif")) {
                    return;
                }
            }
            ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesDefault());
            io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\micross.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesDefault());

            // Run-length encoding of some icons
            // In retrorespect I should have used an icon map like FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS
            unsigned char minimize[] = { 86,6,6,6,0 };
            unsigned char close[] = { 14,2,4,2,5,2,2,2,7,4,9,2,9,4,7,2,2,2,5,2,4,2,0 };
            unsigned char* run_length[] = { minimize, close };

            int rect_ids[IM_ARRAYSIZE(run_length)];
            for (int i = 0; i < IM_ARRAYSIZE(run_length); i++) {
                rect_ids[i] = io.Fonts->AddCustomRectFontGlyph(font, 214 + i, 12, 9, 13 + 1);
            }

            io.Fonts->Build();

            unsigned char* tex_pixels = NULL;
            int tex_width, tex_height;
            io.Fonts->GetTexDataAsRGBA32(&tex_pixels, &tex_width, &tex_height);

            for (int i = 0; i < IM_ARRAYSIZE(run_length); i++) {

                int rect_id = rect_ids[i];
                if (const ImFontAtlasCustomRect* rect = io.Fonts->GetCustomRectByIndex(rect_id)) {
                    bool black = false;
                    unsigned char* run = run_length[i];
                    int run_size = 0;
                    bool done = false;

                    for (int y = 0; !done && y < rect->Height; y++)
                    {
                        ImU32* p = (ImU32*)tex_pixels + (rect->Y + y) * tex_width + (rect->X);
                        for (int x = rect->Width; !done && x > 0; x--) {
                            if (black) *p = IM_COL32(255, 0, 0, 255);
                            p++;
                            run_size++;
                            if (run_size == *run) {
                                run_size = 0;
                                run++;
                                black = !black;
                                done = (*run == 0);
                            }
                        }
                    }
                }
            }
        }
    }

    void Start()
    {
        ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, .0f);
        ImGuiStudio::Init();

        {
            struct ButtonInfo :
                GIDE::Toolbox::Component
            {
                virtual std::string name() const { return "Button"; }
                virtual std::string group() const { return "Dear ImGui"; }
                virtual std::string description() const { return "button"; }
                virtual bool is_containter() const { return false; }
                virtual const Properties& properties() const
                {
                    struct lambdas {
                        static Properties properties()
                        {
                            Properties result;
                            result["width"] = GIDE::Properties::FloatingPoint;
                            result["height"] = GIDE::Properties::FloatingPoint;
                            return result;
                        }
                    };
                    static Properties result = lambdas::properties();
                    return result;
                }
            };

            struct SmallButtonInfo :
                GIDE::Toolbox::Component
            {
                virtual std::string name() const { return "SmallButton"; }
                virtual std::string group() const { return "Dear ImGui"; }
                virtual std::string description() const { return "button with FramePadding=(0,0) to easily embed within text"; }
                virtual bool is_containter() const { return false; }
                virtual const Properties& properties() const
                {
                    struct lambdas {
                        static Properties properties()
                        {
                            Properties result;
                            result["width"] = GIDE::Properties::FloatingPoint;
                            result["height"] = GIDE::Properties::FloatingPoint;
                            return result;
                        }
                    };
                    static Properties result = lambdas::properties();
                    return result;
                }
            };

            struct CheckboxInfo :
                GIDE::Toolbox::Component
            {
                virtual std::string name() const { return "Checkbox"; }
                virtual std::string group() const { return "Dear ImGui"; }
                virtual std::string description() const { return ""; }
                virtual bool is_containter() const { return false; }
                virtual const Properties& properties() const
                {
                    struct lambdas {
                        static Properties properties()
                        {
                            Properties result;
                            result["width"] = GIDE::Properties::FloatingPoint;
                            result["height"] = GIDE::Properties::FloatingPoint;
                            return result;
                        }
                    };
                    static Properties result = lambdas::properties();
                    return result;
                }
            };

            ImGuiStudio::Designer().toolbox().add(*new ButtonInfo());
            ImGuiStudio::Designer().toolbox().add(*new SmallButtonInfo());
            ImGuiStudio::Designer().toolbox().add(*new CheckboxInfo());
        }
        bool opened = true;
        int display_w = 0, display_h = 0;
        int display_x = 0, display_y = 0;
        enum { imgui_main_window_border_x = 20, imgui_main_window_border_y = 20 };

        glfwGetWindowPos(NativeWindow(), &display_x, &display_y);
        glfwGetWindowSize(NativeWindow(), &display_w, &display_h);

        ImGui::SetNextWindowSize(ImVec2(display_w - imgui_main_window_border_x, display_h - imgui_main_window_border_y)); // ensures ImGui fits the GLFW window
        ImGui::SetNextWindowPos(ImVec2(imgui_main_window_border_x, imgui_main_window_border_y));

        while (!glfwWindowShouldClose(NativeWindow()))
        {
            glfwPollEvents();

#ifdef _OPENGL2
            ImGui_ImplOpenGL2_NewFrame();
#else
            ImGui_ImplOpenGL3_NewFrame();
#endif

            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            
            //glfwGetFramebufferSize(NativeWindow(), &display_w, &display_h);

            

            ImGuiStudio::MainWindow::Begin(&opened);
            
            auto winSize = ImGui::GetWindowSize();
            auto winPos = ImGui::GetWindowPos();

            if (winSize.x != (display_w - imgui_main_window_border_x * 2) || winSize.y != (display_h - imgui_main_window_border_y * 2))
            {
                display_w = winSize.x, display_h = winSize.y;
                glfwSetWindowSize(NativeWindow(), display_w + imgui_main_window_border_x*2, display_h + imgui_main_window_border_y*2);
            }

            if (winPos.x != imgui_main_window_border_x || winPos.y != imgui_main_window_border_y)
            {
                glfwGetWindowPos(NativeWindow(), &display_x, &display_y);
                display_x += (winPos.x - imgui_main_window_border_x), display_y += (winPos.y - imgui_main_window_border_y);
                glfwSetWindowPos(NativeWindow(), display_x, display_y);
                ImGui::SetWindowPos(ImVec2(imgui_main_window_border_x, imgui_main_window_border_y));
            }

            if (!opened)
                return;

            ImGuiStudio::Designer().step();
            
            ImGuiStudio::MainWindow::End();

            ImGui::Render();

            


            if (glViewport)
                glViewport(0, 0, display_w, display_h);

            if (glClearColor)
                glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            if (glClear)
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef _OPENGL2
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#else
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

            // glfwMakeContextCurrent( m_glfwindow );
            glfwSwapBuffers(NativeWindow());
        }
    }

    void Stop() 
    {

        ImGuiStudio::Free();

#ifdef _OPENGL2
        ImGui_ImplOpenGL2_Shutdown();
#else
        ImGui_ImplOpenGL3_Shutdown();
#endif

        ImGui_ImplGlfw_Shutdown();

        ImGui::DestroyContext();

        glfwDestroyWindow(NativeWindow());

        glfwTerminate();
    }
} // namespace ImGuiStudioBackend


int main(int argc, char** argv)
{
    try 
    {
        ImGuiStudioBackend::Init(argc, argv);
        ImGuiStudioBackend::Start();
    }
    catch (const std::exception& e)
    {
        std::cout << "Dear ImGui Studio application has experienced unexpected exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Dear ImGui Studio application has experienced unknown exception" << std::endl;
    }
    ImGuiStudioBackend::Stop();
    return 0;
}
