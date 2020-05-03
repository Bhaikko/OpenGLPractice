CXX=g++
RM=rm -f
CPPFLAGS=

GlewLibs=-lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl
GlfwLibs=-lGLEW 

SRCS_CPP= \
	Window.cpp \
	Texture.cpp \
	SpotLight.cpp \
	Skybox.cpp \
	ShadowMap.cpp \
	Shader.cpp \
	PointLight.cpp \
	OmniShadowMap.cpp \
	Mesh.cpp \
	Material.cpp \
	Light.cpp \
	DirectionalLight.cpp \
	Camera.cpp \
	main.cpp \

run: 
	$(CXX) $(SRCS_CPP) -o application $(GlewLibs) $(GlfwLibs)
