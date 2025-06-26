#define GLAD_STATIC
#include "glad/glad.h"

#define GLFW_STATIC
#include <GLFW/glfw3.h>

#define GLT_STATIC
#define GLT_IMPLEMENTATION
#define GLT_MANUAL_VIEWPORT
#include "GLT/gltext.h"

#include <string_view>
#include <iostream>
#include <utility>

#include "libmem/libmem.hpp"

constexpr std::string_view processName{ "hl2_linux" };
constexpr std::string_view engineModuleName{ "engine.so" };

constexpr std::uintptr_t VIEW_ANGLE_BASE{ 0xB33498 };

int main()
{
    std::optional<libmem::Process> optionalProcess{ libmem::FindProcess(::processName.data()) };

    if (!optionalProcess.has_value())
    {
        std::cerr << "process not found\n";

        return 1;
    }

    const libmem::Process process(std::move(optionalProcess.value()));

    optionalProcess.reset();

    // code under saves a little bit of memory to get it to work
    // add Process(){}; in libmem.hpp in Process struct above Process(const struct lm_process_t *process);

    /*

    libmem::Process process{};

    {   
        const std::optional<libmem::Process> optionalProcess{ libmem::FindProcess(::processName.data()) };

        if (!optionalProcess.has_value())
        {
            std::cerr << "process not found\n";

            return 1;
        }

        process = std::move(optionalProcess.value());
    }

    */

    std::uintptr_t engineModuleBase{};

    {
        const std::optional<libmem::Module> optionalEngineModule{ libmem::FindModule(&process, ::engineModuleName.data()) };

        if (!optionalEngineModule.has_value())
        {
            std::cerr << "engine module not found\n";

            return 1;
        }

        engineModuleBase = std::move(optionalEngineModule.value().base);
    }

    if (!::glfwInit())
    {
        std::cerr << "glfw init failed\n";

        return 1;
    }

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ::glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    ::glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    ::glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    ::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // mouse passthrough not needed for x11

    ::GLFWwindow* window{ ::glfwCreateWindow(75, 50, "window", nullptr, nullptr) };
    if (!window)
    {
        std::cerr << "glfw create window failed\n";

        ::glfwTerminate();

        return 1;
    }

    ::glfwSetWindowPos(window, 0, 0);

    ::glfwMakeContextCurrent(window);
    ::glfwSwapInterval(1);

    if (!::gladLoadGLLoader(reinterpret_cast<::GLADloadproc>(::glfwGetProcAddress)))
	{
        std::cerr << "glad loader failed\n";

        ::glfwDestroyWindow(window);
        ::glfwTerminate();

        return 1;
	}

    {   
        int width, height;
        ::glfwGetFramebufferSize(window, &width, &height);

        ::glViewport(0, 0, width, height);
        ::gltViewport(width, height);
    }

    if (!::gltInit())
    {
        std::cerr << "glt init failed\n";

        ::glfwDestroyWindow(window);
        ::glfwTerminate();

        return 1;
    }

    ::GLTtext* text{ ::gltCreateText() };
    ::gltSetText(text, "fgtfc");

    float yaw{};

    while (!::glfwWindowShouldClose(window))
    {
        if (!libmem::ReadMemory(&process, engineModuleBase + (::VIEW_ANGLE_BASE + 0x4), reinterpret_cast<std::uint8_t*>(&yaw), sizeof(yaw)))
        {
            std::cerr << "reading view angle failed\n";
            
            break;
        }

        if (yaw == 0.f)
        {
            continue;
        }

        ::glfwPollEvents();

        ::glClear(GL_COLOR_BUFFER_BIT);

        ::gltBeginDraw();

        ::gltSetText(text, std::to_string(static_cast<int>(yaw) + 180).c_str());
        ::gltColor(1.f, 1.f, 1.f, 1.f);
        ::gltDrawText2D(text, 0, 0, 2.5f);

        ::gltEndDraw();

        ::glfwSwapBuffers(window);
        
        ::glfwWaitEventsTimeout(0.01);
    }

    ::gltDeleteText(text);
    ::gltTerminate();

    ::glfwDestroyWindow(window);
    ::glfwTerminate();

    return 0;
}