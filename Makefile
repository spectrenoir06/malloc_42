# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/20 21:46:05 by adoussau          #+#    #+#              #
#    Updated: 2017/11/20 21:58:26 by adoussau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

STATIC_EXE	= malloc
DEBUG_EXE	= malloc_debug
GPROF_EXE	= malloc_gprof

SRC		=	main.c

HEAD_DIR	= includes
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
GPROF_DIR	= gprof
C_HEAD_DIR	= debug

LIBFT_STATIC= libft/libft.a
LIBFT_DEBUG	= libft/libft_debug.a
LIBFT_HEAD	= libft/includes/

STATIC_OBJ	= $(patsubst %.c,$(STATIC_DIR)/%.o,$(SRC))
DEBUG_OBJ	= $(patsubst %.c,$(DEBUG_DIR)/%.o,$(SRC))
GPROF_OBJ	= $(patsubst %.c,$(GPROF_DIR)/%.o,$(SRC))

CC			= gcc -fdiagnostics-color=always
NORMINETTE	= ~/project/colorminette/colorminette
OPTI		= O3
OPTI_DEBUG	= O0

OBJ		= $(SRC:.c=.o)
CC		= gcc -O3 -fdiagnostics-color=always

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	FLAGS		= -Wall -Wextra
	SRC	+=
endif

ifeq ($(UNAME_S), Darwin)
	FLAGS		= -Wall -Wextra -Werror
	SRC	+=
endif



$(shell mkdir -p $(STATIC_DIR) $(DEBUG_DIR) $(GPROF_DIR))

all: $(STATIC_EXE)
	@echo "Compilation terminee. (realease)"

debug: $(DEBUG_EXE)
	@echo "Compilation terminee. (debug)"

gprof: $(GPROF_EXE)
	@echo "Compilation terminee. (gprof)"

###############################################################################################################

$(DEBUG_EXE): $(DEBUG_OBJ) $(LIBFT_DEBUG)
	$(CC) -$(OPTI_DEBUG) -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $(DEBUG_EXE) $(DEBUG_OBJ) $(LIBFT_DEBUG) $(FLAGS) -g

$(STATIC_EXE): $(STATIC_OBJ) $(LIBFT_STATIC)
	$(CC) -$(OPTI) -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $@ $(STATIC_OBJ) $(LIBFT_STATIC) $(FLAGS)

$(GPROF_EXE): $(GPROF_OBJ) $(LIBFT_STATIC)
	$(CC) -$(OPTI) -pg -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $@ $(GPROF_OBJ) $(LIBFT_STATIC) $(FLAGS)

################################################################################################################

$(STATIC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -$(OPTI) -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $@ -c $< $(FLAGS)

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -$(OPTI_DEBUG) -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $@ -c $< $(FLAGS) -g

$(GPROF_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -$(OPTI) -pg -I $(HEAD_DIR) -I $(LIBFT_HEAD) -o $@ -c $< $(FLAGS) -g

################################################################################################################

$(LIBFT_STATIC):
	make -C libft/ libft.a

$(LIBFT_DEBUG):
	make -C libft/ libft_debug.a

.PHONY: clean fclean re debug norme gprof

clean:
	rm -f $(STATIC_OBJ) $(DEBUG_OBJ) $(GPROF_OBJ)
	make -C libft clean

fclean: clean
	rm -f $(STATIC_EXE) $(DEBUG_EXE) $(GPROF_EXE)
	make -C libft fclean

norme:
	$(NORMINETTE) $(SRC_DIR)/ $(HEAD_DIR)/
	make -C libft norme

re: fclean
	make
