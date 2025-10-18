module;
#include <GLFW/glfw3.h>

export module glfw3;

export  {
  void use_glfw_wayland() {
    glfwInitHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

  }
    using ::glfwCreateWindow;
    using ::glfwDefaultWindowHints;
    using ::glfwDestroyWindow;
    using ::glfwGetKey;
    using ::glfwGetTime;
    using ::glfwGetWindowAttrib;
    using ::glfwGetWindowMonitor;
    using ::glfwGetWindowPos;
    using ::glfwSetFramebufferSizeCallback;
    using ::glfwGetWindowSize;
    using ::glfwInit;
    using ::glfwMakeContextCurrent;
    using ::glfwPollEvents;
    using ::glfwSetErrorCallback;
    using ::glfwSetKeyCallback;
    using ::glfwSetWindowShouldClose;
    using ::glfwSwapBuffers;
    using ::glfwSwapInterval;
    using ::glfwTerminate;
    using ::glfwWindowHint;
    using ::GLFWwindow;
}
