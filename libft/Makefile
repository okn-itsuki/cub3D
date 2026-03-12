# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kyanagis <kyanagis@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 01:05:21 by kyanagis          #+#    #+#              #
#    Updated: 2025/11/12 04:58:25 by kyanagis         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME        := libft.a

CC          := cc
CFLAGS      := -Wall -Wextra -Werror
INC_DIR     := .
CPPFLAGS    := -MMD -MP -I$(INC_DIR)

AR          := ar
ARFLAGS     := rcs
RM          := rm -f
MKDIRP      := mkdir -p

OBJ_DIR     := obj


SRC := \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	ft_atoi.c ft_calloc.c ft_strdup.c \
	ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
	ft_putendl_fd.c ft_putnbr_fd.c ft_atof.c ft_strcmp.c \
	ft_isspace.c get_next_line.c ft_strndup.c ft_xmalloc.c\
    ft_strcpy.c ft_xcalloc.c ft_xstrdup.c

BONUS_SRC := \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
	ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
	ft_lstmap.c


OBJ       := $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
BONUS_OBJ := $(addprefix $(OBJ_DIR)/,$(BONUS_SRC:.c=.o))
DEPS      := $(OBJ:.o=.d) $(BONUS_OBJ:.o=.d)


.PHONY: all clean fclean re bonus norm

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

bonus: $(OBJ) $(BONUS_OBJ)
	$(AR) $(ARFLAGS) $(NAME) $(BONUS_OBJ)

$(OBJ_DIR):
	$(MKDIRP) $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(BONUS_OBJ) $(DEPS)
	- rmdir $(OBJ_DIR) 2>/dev/null || true

fclean: clean
	$(RM) $(NAME)

re: fclean all

-include $(DEPS)
