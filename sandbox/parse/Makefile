NAME := DEBUG_parse_test

ROOT_DIR := ../..
INCLUDE_DIR := $(ROOT_DIR)/include
LIBFT_DIR := $(ROOT_DIR)/libft
OBJ_DIR := obj

CC := cc
RM := rm -f
RMDIR := rm -rf

SRC := $(shell find ./parse_test -type f -name '*.c' 2>/dev/null)
OBJ := $(patsubst ./%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

CPPFLAGS := -I$(INCLUDE_DIR) -I./parse_test
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

MAP ?= ../../maps/good/test_map.cub

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: ./%.c
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
