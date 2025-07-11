# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: preltien <preltien@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 17:31:34 by erazumov          #+#    #+#              #
#    Updated: 2025/07/11 11:19:11 by preltien         ###   ########.fr        #
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
LIBFT_DIR = libft/
OBJ_DIR = obj/
INCLUDES = -I include/

# Libft specifics
LIBFT_A = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I $(LIBFT_DIR)/include

# Source Files
SRC = src/main.c \
      src/execution/execution.c \
      src/execution/execution_utils.c \
      src/expansion/expansion_append_utils.c \
      src/expansion/expansion.c \
      src/expansion/expansion_var_utils.c \
      src/lexer/lexer.c \
      src/lexer/lexer_ops_utils.c \
      src/lexer/lexer_token_utils.c \
      src/lexer/lexer_word.c \
      src/lexer/lexer_word_utils.c \
      src/parser/parser.c \
      src/parser/parser_free_utils.c \
      src/parser/parser_print_utils.c \
      src/parser/parser_utils.c
OBJS = $(patsubst src/%.c,$(OBJ_DIR)%.o,$(SRC))
OBJ_DIRS = $(sort $(dir $(OBJS)))
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


$(OBJ_DIR):
	@mkdir -p $(OBJ_DIRS)
	@echo "$(BLUE)📁 Created object directories$(RESET)"

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBFT_INC) -c $< -o $@
	@echo "$(YELLOW)🪄 Compiling:$(RESET) $< -> $@"

-include $(DEPS)

clean:
	@$(RM) -r $(OBJ_DIR)
	@make clean -s -C $(LIBFT_DIR)

fclean: clean
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT_DIR)
	@echo "$(RED)🗑️ Executables minishell removed$(RESET)"

re: fclean all

norm:
	@norminette $(SRC_DIR) $(INCLUDES) $(LIBFT_DIR) | grep -v "OK" || true

.PHONY: all clean fclean re norm
