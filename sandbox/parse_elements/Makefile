NAME = parse_elements_test

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -I../../include -I../../libft \
	-Dparse_get_line_kind=parse_get_line_type

LIBFT_DIR = ../../libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
	../../src/parse/parse_elements.c \
	../../src/parse/parse_utils.c \
	../../src/parse/init_config.c \
	../../src/config_destroy.c

OBJ_DIR = .obj
OBJ = $(OBJ_DIR)/main.o \
	$(OBJ_DIR)/parse_elements.o \
	$(OBJ_DIR)/parse_utils.o \
	$(OBJ_DIR)/init_config.o \
	$(OBJ_DIR)/config_destroy.o

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/parse_elements.o: ../../src/parse/parse_elements.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/parse_utils.o: ../../src/parse/parse_utils.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/init_config.o: ../../src/parse/init_config.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/config_destroy.o: ../../src/config_destroy.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	rmdir $(OBJ_DIR) 2>/dev/null || true
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
