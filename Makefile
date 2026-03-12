NAME := cub3D

SRC_DIR := src
INCLUDE_DIR := include
OBJ_DIR := obj
LIBFT_DIR := libft

CC := cc
RM := rm -f
RMDIR := rm -rf
UNAME_S := $(shell uname -s)

SRC := $(shell find $(SRC_DIR) -type f -name '*.c' 2>/dev/null)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -I$(SRC_DIR)
CFLAGS := -Wall -Wextra -Werror
DEPFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

ifneq ($(strip $(DEBUG)),)
CFLAGS += -g3 -O0
endif

ifeq ($(UNAME_S),Darwin)
MLX_DIR := minilibx-macos
MLX_REPO := https://github.com/dannywillems/minilibx-mac-osx.git
LDLIBS += -lmlx -framework OpenGL -framework AppKit
else ifeq ($(UNAME_S),Linux)
MLX_DIR := minilibx-linux
MLX_REPO := https://github.com/42Paris/minilibx-linux.git
LDLIBS += -lmlx -lXext -lX11 -lm -lz
else
$(error Unsupported platform: $(UNAME_S))
endif

MLX := $(MLX_DIR)/libmlx.a
CPPFLAGS += -I$(MLX_DIR)
LDFLAGS += -L$(MLX_DIR)

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
LIBFT := $(LIBFT_DIR)/libft.a
CPPFLAGS += -I$(LIBFT_DIR)
LDFLAGS += -L$(LIBFT_DIR)
LDLIBS += -lft
endif

all: $(NAME)

$(NAME): $(MLX) $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(MLX_DIR):
	git clone $(MLX_REPO) $(MLX_DIR)

$(MLX): | $(MLX_DIR)
	$(MAKE) -C $(MLX_DIR)

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
endif

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) fclean; fi
	$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
