CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanEngine: *.cpp *.hpp
	g++ $(CFLAGS) -o VulkanEngine *.cpp $(LDFLAGS)

.PHONY: test clean

test: VulkanEngine
	./VulkanEngine

clean:
	rm -f VulkanEngine