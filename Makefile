CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Find all .cpp and .h files recursively
CPP_FILES := $(shell find . -name '*.cpp' -o -name '*.hpp')

VulkanEngine: $(CPP_FILES)
	g++ $(CFLAGS) -o VulkanEngine $(CPP_FILES) $(LDFLAGS)

.PHONY: test clean

test: VulkanEngine
	./VulkanEngine

clean:
	rm -f VulkanEngine
