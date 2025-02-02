
NAME = FractalRenderer.exe

# execute in build directory
# cmake ..
# cmake --build .
all:
	cd build && cmake .. && cmake --build . && mv $(NAME) ..
	./$(NAME)

clean:
	rm -rf build
	rm -f $(NAME)

re: clean all
