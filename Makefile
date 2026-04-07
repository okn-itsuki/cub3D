NAME := cub3D

DEFAULT_SRC_DIR := src
BONUS_SRC_DIR := bonus_src
DEFAULT_INCLUDE_DIR := include
BONUS_INCLUDE_DIR := include_bonus
OBJ_ROOT := obj
LIBFT_DIR := libft

CC := cc
RM := rm -f
RMDIR := rm -rf
UNAME_S := $(shell uname -s)

BUILD_MODE := mandatory
ifneq ($(filter bonus,$(MAKECMDGOALS)),)
BUILD_MODE := bonus
endif
ifeq ($(origin BONUS),command line)
ifneq ($(strip $(BONUS)),)
BUILD_MODE := bonus
endif
endif

DEBUG_BUILD :=
ifneq ($(filter debug,$(MAKECMDGOALS)),)
DEBUG_BUILD := 1
endif
ifeq ($(origin DEBUG),command line)
ifneq ($(strip $(DEBUG)),)
DEBUG_BUILD := 1
endif
endif

ifeq ($(BUILD_MODE),bonus)
SRC_DIR := $(BONUS_SRC_DIR)
INCLUDE_DIR := $(BONUS_INCLUDE_DIR)
else
SRC_DIR := $(DEFAULT_SRC_DIR)
INCLUDE_DIR := $(DEFAULT_INCLUDE_DIR)
endif

BUILD_TAG := $(BUILD_MODE)
ifneq ($(strip $(DEBUG_BUILD)),)
BUILD_TAG := $(BUILD_TAG)_debug
endif

OBJ_DIR := $(OBJ_ROOT)/$(BUILD_TAG)
MODE_BINARY := $(OBJ_DIR)/$(NAME)

SRC := $(shell find $(SRC_DIR) -type f -name '*.c' 2>/dev/null | sort)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -I$(SRC_DIR)
CFLAGS := -Wall -Wextra -Werror
DEPFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

ifneq ($(strip $(DEBUG_BUILD)),)
CFLAGS += -g3 -O0
endif

ifeq ($(UNAME_S),Darwin)
MLX_DIR := minilibx_opengl_20191021
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

all: link

bonus: link

debug: link

link: $(MODE_BINARY)
	cp -f $(MODE_BINARY) $(NAME)

$(MODE_BINARY): $(MLX) $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

ifneq ($(MLX_REPO),)
$(MLX_DIR):
	git clone $(MLX_REPO) $(MLX_DIR)
endif

$(MLX): | $(MLX_DIR)
	$(MAKE) -C $(MLX_DIR)

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
endif

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi
	$(RMDIR) $(OBJ_ROOT)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) fclean; fi
	$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all bonus debug link clean fclean re
