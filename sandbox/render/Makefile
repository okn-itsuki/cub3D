NAME := render_sandbox

ROOT_DIR := ../..
DEFAULT_SRC_DIR := $(ROOT_DIR)/src
BONUS_SRC_DIR := $(ROOT_DIR)/bonus_src
DEFAULT_INCLUDE_DIR := $(ROOT_DIR)/include
BONUS_INCLUDE_DIR := $(ROOT_DIR)/include_bonus
LIBFT_DIR := $(ROOT_DIR)/libft
OBJ_ROOT := obj

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

DASHBOARD_BUILD :=
ifneq ($(filter debug,$(MAKECMDGOALS)),)
DASHBOARD_BUILD := 1
endif
ifeq ($(origin DASHBOARD),command line)
ifneq ($(strip $(DASHBOARD)),)
DASHBOARD_BUILD := 1
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

SANDBOX_SRC := $(shell find . -type f -name '*.c' 2>/dev/null | sort)
APP_SRC := $(shell find $(SRC_DIR) -type f -name '*.c' \
	! -name 'main.c' ! -name 'main_bonus.c' 2>/dev/null | sort)
OBJ := $(patsubst ./%.c,$(OBJ_DIR)/sandbox/%.o,$(SANDBOX_SRC)) \
	$(patsubst $(ROOT_DIR)/%.c,$(OBJ_DIR)/%.o,$(APP_SRC))
DEP := $(OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -I$(SRC_DIR)
CFLAGS := -Wall -Wextra -Werror
DEPFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

ifeq ($(BUILD_MODE),bonus)
CPPFLAGS += -DBONUS_MODE
endif

ifneq ($(strip $(DEBUG_BUILD)),)
CFLAGS += -g3 -O0
endif

ifneq ($(strip $(DASHBOARD_BUILD)),)
CPPFLAGS += -DDEBUG_DASHBOARD
endif

ifeq ($(UNAME_S),Darwin)
MLX_DIR := $(ROOT_DIR)/minilibx_opengl_20191021
LDLIBS += -lmlx -framework OpenGL -framework AppKit
else ifeq ($(UNAME_S),Linux)
MLX_DIR := $(ROOT_DIR)/minilibx-linux
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

$(OBJ_DIR)/sandbox/%.o: ./%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(ROOT_DIR)/%.c
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
	$(RMDIR) $(OBJ_ROOT)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all bonus debug link clean fclean re
