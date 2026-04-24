NAME := DEBUG_parse_test

ROOT_DIR := ../..
INCLUDE_DIR := $(ROOT_DIR)/include
LIBFT_DIR := $(ROOT_DIR)/libft
OBJ_DIR := obj
PARSE_SRC_DIR := $(ROOT_DIR)/src

CC := cc
RM := rm -f
RMDIR := rm -rf

SRC := $(shell find ./parse_test -type f -name '*.c' 2>/dev/null)
PARSE_SRC := \
	$(PARSE_SRC_DIR)/config_destroy.c \
	$(PARSE_SRC_DIR)/map_access.c \
	$(PARSE_SRC_DIR)/parse/init_config.c \
	$(PARSE_SRC_DIR)/parse/parse.c \
	$(PARSE_SRC_DIR)/parse/parse_color.c \
	$(PARSE_SRC_DIR)/parse/parse_elements.c \
	$(PARSE_SRC_DIR)/parse/parse_map.c \
	$(PARSE_SRC_DIR)/parse/parse_texture.c \
	$(PARSE_SRC_DIR)/parse/parse_utils.c \
	$(PARSE_SRC_DIR)/parse/read_file.c \
	$(PARSE_SRC_DIR)/parse/split_lines.c \
	$(PARSE_SRC_DIR)/parse/system_err_mes.c
OBJ := $(patsubst ./%.c,$(OBJ_DIR)/%.o,$(SRC))
PARSE_OBJ := $(patsubst $(PARSE_SRC_DIR)/%.c,$(OBJ_DIR)/root_src/%.o,$(PARSE_SRC))
DEP := $(OBJ:.o=.d)
DEP += $(PARSE_OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -I./parse_test
CFLAGS := -Wall -Wextra -Werror
DEPFLAGS := -MMD -MP
LDFLAGS :=
LDLIBS :=

ifneq ($(strip $(DEBUG)),)
CFLAGS += -g -O0
endif

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
LIBFT := $(LIBFT_DIR)/libft.a
CPPFLAGS += -I$(LIBFT_DIR)
LDFLAGS += -L$(LIBFT_DIR)
LDLIBS += -lft
endif

MAP ?= ../../maps/good/test_map.cub

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ) $(PARSE_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(PARSE_OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: ./%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/root_src/%.o: $(PARSE_SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

ifneq ($(wildcard $(LIBFT_DIR)/Makefile),)
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
endif

run: $(NAME)
	./$(NAME) $(MAP)

clean:
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re run
