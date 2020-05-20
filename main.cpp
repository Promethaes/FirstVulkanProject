#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>

const auto WINDOW_NAME = "VulkanProjectOne";
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* _window;
	VkInstance instance;

	void createInstance() {
		//create app info
		VkApplicationInfo appInfo{};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= "VulkanProjectOne";
		appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName			= "No Engine";
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion			= VK_API_VERSION_1_0;

		//create info for the instance
		VkInstanceCreateInfo createInfo{};
		createInfo.sType			= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		//find number/names of extensions for glfw
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount	= glfwExtensionCount;
		createInfo.ppEnabledExtensionNames	= glfwExtensions;

		std::cout << "available GLFW extensions:\n";

		for (uint32_t i = 0; i < createInfo.enabledExtensionCount; i++)
			std::cout << "\t" << createInfo.ppEnabledExtensionNames[i] << "\n";

		createInfo.enabledLayerCount = 0;

		//fill vector with extension names from driver
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		std::cout << "available extensions:\n";

		for (auto x : extensions)
			std::cout << "\t" << x.extensionName << "\n";

		//check to see if all required glfw extensions are supported by the driver
		uint32_t searchCounter = 0;
		bool requiredExtensionsEnabled = false;
		for (auto x : extensions) {
			std::string temp1 = x.extensionName;
			std::string temp2 = createInfo.ppEnabledExtensionNames[searchCounter];
			if (temp1.compare(temp2)) {
				searchCounter++;
				if (searchCounter == createInfo.enabledExtensionCount) {
					printf("All required GLFW extensions are enabled.\n");
					requiredExtensionsEnabled = true;
					break;
				}
			}
		}

		if (!requiredExtensionsEnabled)
			printf("Missing GLFW exteionsions!\n");

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan Instance!");
		else
			printf("Vulkan Instance Created.\n");
	}

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_NAME, nullptr, nullptr);
	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(_window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(_window);
		glfwTerminate();
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}