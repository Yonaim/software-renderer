all:
	g++ -std=c++20 main.cpp src/*/*.cpp -I./include -o renderer.out

clean:

fclean:

re: 
	make fclean
	make all

.PHONY: all clean fclean re bonus
