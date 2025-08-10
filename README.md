# Расширения в Vulkan

Расширения добавляют в Vulkan дополнительную функциональность (например, интеграцию с оконными системами). Они представляют собой официально утверждённые модули, расширяющие базовый API.

---

## Ключевые элементы

### Структура расширения
```c
typedef struct VkExtensionProperties {
    char   extensionName[VK_MAX_EXTENSION_NAME_SIZE]; // Название расширения
    uint32_t specVersion;                             // Версия
} VkExtensionProperties;
```

### Получение списка расширений
```cpp
uint32_t count;
vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);

std::vector<VkExtensionProperties> extensions(count);
vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());

for (auto& ext : extensions) {
    std::cout << ext.extensionName << std::endl;
}
```

---

## Активация расширений (рабочий пример)
```cpp
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // 1. Инициализация GLFW (обязательно!)
    if (!glfwInit()) {
        std::cerr << "Ошибка инициализации GLFW" << std::endl;
        return 1;
    }

    // 2. Получение расширений для оконной системы
    uint32_t extensionCount;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    // 3. Настройка создания инстанса
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    // 4. Создание инстанса
    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Не удалось создать Vulkan instance" << std::endl;
        glfwTerminate();  // Важно: очистка GLFW при ошибке
        return 1;
    }

    std::cout << "Vulkan instance успешно создан" << std::endl;

    // 5. Очистка ресурсов
    vkDestroyInstance(instance, nullptr);
    glfwTerminate();  // Обязательная очистка GLFW

    return 0;
}
```

---

## Критические исправления
1. **Инициализация GLFW**  
   Без `glfwInit()` вызов `glfwGetRequiredInstanceExtensions` приведёт к неопределённому поведению

2. **Устранение двойного вызова**  
   В оригинальном примере:
   ```cpp
   else(vkCreateInstance(...) != VK_SUCCESS) // Синтаксическая ошибка + повторный вызов
   ```
   Исправлено на единственный вызов с правильной обработкой ошибок

3. **Гарантированная очистка ресурсов**  
   Добавлены:
   - `glfwTerminate()` после `vkDestroyInstance`
   - Очистка GLFW при возникновении ошибки

4. **Корректная обработка ошибок**  
   Использован `std::cerr` для вывода ошибок и возврат кода ошибки `1`

---

## Важно
1. Расширения **должны быть явно активированы** при создании `VkInstance`
2. Для работы с окнами **обязательно**:
   - Инициализировать GLFW
   - Получить оконные расширения через `glfwGetRequiredInstanceExtensions`
3. При ошибке создания инстанса:
   - Не забывайте вызывать `glfwTerminate()`
   - Не пытайтесь уничтожить несуществующий инстанс

> Пример METANIT содержал критические ошибки: отсутствие инициализации GLFW и некорректную обработку создания инстанса. Данный код гарантирует корректную работу с расширениями.
