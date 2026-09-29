CC = gcc
CFLAGS = -Wall -O2
PKG_CONFIG ?= pkgconf

GLEW_CFLAGS = $(shell $(PKG_CONFIG) glew --cflags)
GLFW_CFLAGS = $(shell $(PKG_CONFIG) glfw3 --cflags)
GLEW_LIBS = $(shell $(PKG_CONFIG) glew --libs)
GLFW_LIBS = $(shell $(PKG_CONFIG) glfw3 --libs)

ifeq ($(OS),Windows_NT)
EXE = .exe
OPENGL_LIBS = -lopengl32
else
EXE =
OPENGL_LIBS = -lGL
endif

SRC = src/main.c \
      src/math/mat4.c \
      src/graphics/camera.c \
      src/geometry/obj_loader.c \
      src/texture/ppm_loader.c \
      src/render/shader_program.c \
      src/render/render_mesh.c \
      src/render/render_texture.c \
      src/render/image_texture.c \
      src/render/skybox.c

HEADERS = $(wildcard src/*.h src/*/*.h)

.PHONY: all tests clean
all: cg1$(EXE)

ifeq ($(OS),Windows_NT)
.PHONY: cg1
cg1: cg1.exe
endif

cg1$(EXE): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(GLEW_CFLAGS) $(GLFW_CFLAGS) $(GLEW_LIBS) $(GLFW_LIBS) $(OPENGL_LIBS) -lm

test_mat4$(EXE): src/tests/test_mat4.c src/tests/test_utils.c src/math/mat4.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ src/tests/test_mat4.c src/tests/test_utils.c src/math/mat4.c -lm

test_obj_loader$(EXE): src/tests/test_obj_loader.c src/geometry/obj_loader.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ src/tests/test_obj_loader.c src/geometry/obj_loader.c

test_ppm_loader$(EXE): src/tests/test_ppm_loader.c src/texture/ppm_loader.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ src/tests/test_ppm_loader.c src/texture/ppm_loader.c

tests: test_mat4$(EXE) test_obj_loader$(EXE) test_ppm_loader$(EXE)

clean:
ifeq ($(OS),Windows_NT)
	-cmd /c del /q cg1.exe test_mat4.exe test_obj_loader.exe test_ppm_loader.exe
else
	rm -f cg1 test_mat4 test_obj_loader test_ppm_loader
endif
