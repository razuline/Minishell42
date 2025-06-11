# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 17:31:34 by erazumov          #+#    #+#              #
#    Updated: 2025/06/11 18:01:30 by erazumov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project Name
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CFLAGS += -MMD -MP
RM = rm -f

# Directories
SRC_DIR = src/
OBJ_DIR = obj/
LIBFT_DIR = libft/
INCLUDES = -I include/

# Libft specifics
LIBFT_A = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I $(LIBFT_DIR)/include

# Source Files
SRC = main.c \
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRC:.c=.o)))
DEPS = $(OBJS:.o=.d)

# Libraries
LDFLAGS = -lreadline -lhistory

# Colors
RESET = \033[0m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m

# Rules
all: $(NAME)

$(LIBFT_A):
	@echo "$(BLUE)📘 Building Libft...$(RESET)"
	@make -s -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✅ Compilation successful ➜ $(NAME)$(RESET)"

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)📁 Created object directory: $(OBJ_DIR)$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)%.c
	@$(CC) $(CFLAGS) $(INCLUDES) $(LIBFT_INC) -c $< -o $@
	@echo "$(YELLOW)🪄 Compiling:$(RESET) $< -> $@"

-include $(DEPS)

clean:
	@$(RM) -r $(OBJ_DIR)
	@make clean -s -C $(LIBFT_DIR)
	@echo "$(RED)🧹 Object files removed$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@echo "$(RED)🗑️ Executables removed$(RESET)"
	@echo "$(RED)🗑️ Libft cleaned$(RESET)"

re: fclean all

norm:
	@norminette $(SRC_DIR) $(INCLUDES) $(LIBFT_DIR) | grep -v "OK" || true

.PHONY: all clean fclean re norm
