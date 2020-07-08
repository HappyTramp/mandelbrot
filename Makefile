RM = rm -f

NAME = mandel

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

CC = gcc
OFLAG = -O3
CCFLAGS = -I$(INC_DIR) -Wall -Wextra -Wpedantic $(OFLAG) \
		  $(shell pkg-config --cflags sdl2 glew)
LDFLAGS = $(shell pkg-config --libs sdl2 glew)

INC = $(shell find $(INC_DIR) -type f -name '*.h')
SRC = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: prebuild $(NAME)

prebuild:
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	$(CC) $(CCFLAGS) -c -o $@ $<

debug: OFLAG = -g
debug: all

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

windows:
	gcc -O3 src\*.c -I inc -lSDL2 -lSDL2main -lglew32 -lopengl32

.PHONY: all debug clean fclean re windows
