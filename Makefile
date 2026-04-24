NAME := cub3D

INCLUDE_DIR := include
OBJ_DIR := obj
LIBFT_DIR := libft

CC := cc
RM := rm -f
RMDIR := rm -rf

SRC := \
	src/main.c \
	src/config_destroy.c \
	src/map_access.c \
	src/parse/init_config.c \
	src/parse/parse.c \
	src/parse/parse_elements.c \
	src/parse/parse_color.c \
	src/parse/parse_map.c \
	src/parse/parse_texture.c \
	src/parse/parse_utils.c \
	src/parse/read_file.c \
	src/parse/split_lines.c \
	src/parse/system_err_mes.c
OBJ := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -Isrc
CFLAGS := -Wall -Wextra -Werror
DEPFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

ifneq ($(strip $(DEBUG)),)
CFLAGS += -g3 -O0
endif

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
LIBFT := $(LIBFT_DIR)/libft.a
CPPFLAGS += -I$(LIBFT_DIR)
LDFLAGS += -L$(LIBFT_DIR)
LDLIBS += -lft
endif

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
endif

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) fclean; fi
	$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
