# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erazumov <erazumov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/11 17:31:34 by erazumov          #+#    #+#              #
#    Updated: 2025/08/11 11:31:31 by erazumov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================ #
#                                 Informations                                 #
# ============================================================================ #
NAME = minishell
CC = cc
RM = rm -f

# ============================================================================ #
#                                   Flags                                      #
# ============================================================================ #
CFLAGS = -Wall -Wextra -Werror -g -MMD -MP

# ============================================================================ #
#                                 Dossiers                                     #
# ============================================================================ #
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include

# ============================================================================ #
#                         Listes des fichiers sources (manuel)                 #
# ============================================================================ #
LEXER_FILES = lexer.c lexer_handlers.c lexer_token_utils.c lexer_word_utils.c
PARSER_FILES = parser.c parser_argv_utils.c parser_list_utils.c \
			   parser_free_utils.c parser_print_utils.c
EXPANSION_FILES = expansion.c expansion_len_utils.c expansion_var_utils.c \
				  expansion_append_utils.c
EXEC_FILES = execution.c execution_utils.c pipeline.c pipeline_utils.c \
			 process_utils.c redirections.c path.c env_utils.c env_set.c \
			 env_unset.c
SIGNALS_FILES = signals.c
BUILTINS_FILES = builtin_cd.c builtin_echo.c builtin_env.c builtin_export.c \
				 builtin_export_utils.c builtin_exit.c builtin_pwd.c \
				 builtin_unset.c

# ============================================================================ #
#                  Construction des chemins et des objets                      #
# ============================================================================ #
MAIN_SRC = $(SRC_DIR)/main.c
LEXER_SRC = $(addprefix $(SRC_DIR)/lexer/, $(LEXER_FILES))
PARSER_SRC = $(addprefix $(SRC_DIR)/parser/, $(PARSER_FILES))
EXPANSION_SRC = $(addprefix $(SRC_DIR)/expansion/, $(EXPANSION_FILES))
EXEC_SRC = $(addprefix $(SRC_DIR)/execution/, $(EXEC_FILES))
SIGNALS_SRC = $(addprefix $(SRC_DIR)/signals/, $(SIGNALS_FILES))
BUILTINS_SRC = $(addprefix $(SRC_DIR)/execution/builtins/, $(BUILTINS_FILES))

SRC = $(MAIN_SRC) $(LEXER_SRC) $(PARSER_SRC) $(EXPANSION_SRC) $(EXEC_SRC) \
	  $(SIGNALS_SRC) $(BUILTINS_SRC)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

DEPS = $(OBJS:.o=.d)

# ============================================================================ #
#                                Bibliothèques                                 #
# ============================================================================ #
READLINE_PATH = /opt/homebrew/opt/readline

INCLUDES = -I $(INCLUDE_DIR) -I $(LIBFT_DIR)/include -I $(READLINE_PATH)/include
LDFLAGS = -L $(LIBFT_DIR) -L $(READLINE_PATH)/lib -lft -lreadline
LIBFT = $(LIBFT_DIR)/libft.a

# ============================================================================ #
#                                    Règles                                    #
# ============================================================================ #
all: $(NAME)

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "\033[0;32m✅ Minishell est prêt !\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "\033[0;33m✨ Compiling:\033[0m $<"

-include $(DEPS)

clean:
	@$(RM) -r $(OBJ_DIR)
	@make clean -s -C $(LIBFT_DIR)

fclean: clean
	@$(RM) $(NAME)
	@make fclean -s -C $(LIBFT_DIR)

re: fclean all

norm:
	@norminette $(SRC_DIR) include

.PHONY: all clean fclean re norm
