#define GLFW_INCLUDE_VULKAN
// #include <vulkan/vulkan.h>  // макрос GLFW_INCLUDE_VULKAN уже подключает Vulkan
#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    setlocale(LC_ALL, "RU");

    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return 1;
    }
    
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    // 1) получаем расширения библиотеки GLFW для Vulkan
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstance instance; // объект, который надо создать

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    // 2) устанавливаем расширения
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // создаем объект VkInstance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cout << "Unable to create VKInstance" << std::endl;
        return 1;
    }
    else if (vkCreateInstance(&createInfo, nullptr, &instance) == VK_SUCCESS) { // заменил "else" на "else if", "!=" на "=="
        std::cout << "VKInstance created" << std::endl;
    }
    // удаление VKInstance
    vkDestroyInstance(instance, nullptr);
    
    return 0;
}
