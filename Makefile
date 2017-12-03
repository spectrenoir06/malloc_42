# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/20 21:46:05 by adoussau          #+#    #+#              #
#    Updated: 2017/12/03 19:19:55 by adoussau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

UNAME_S := $(shell uname -s)

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(UNAME_S)
endif

DYNAMIC_LIB	= libft_malloc_$(HOSTTYPE).so
DEBUG_LIB	= libft_malloc_$(HOSTTYPE)_debug.so

LIBNAME		= ft_malloc_$(HOSTTYPE)
DEBUGLIBNAME= ft_malloc_$(HOSTTYPE)_debug
LINKNAME	= libft_malloc.so

SRC		=	malloc.c \
			print.c

HEAD_DIR	= includes
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
GPROF_DIR	= gprof
C_HEAD_DIR	= debug

DYNAMIC_DIR	= dynamic

LIBFT_STATIC= libft/libft.a
LIBFT_DEBUG	= libft/libft_debug.a
LIBFT_HEAD	= libft/includes/

STATIC_OBJ	= $(patsubst %.c,$(STATIC_DIR)/%.o,$(SRC))
DEBUG_OBJ	= $(patsubst %.c,$(DEBUG_DIR)/%.o,$(SRC))
DYNAMIC_OBJ	= $(patsubst %.c,$(DYNAMIC_DIR)/%.o,$(SRC))

CC			= gcc -fdiagnostics-color=always
NORMINETTE	= ~/project/colorminette/colorminette
OPTI		= -O3
OPTI_DEBUG	= -O0
DEPENDS		= -MT $@ -MD -MP -MF $(subst .o,.d,$@)

OBJ		= $(SRC:.c=.o)
CC		= gcc -O3 -fdiagnostics-color=always

ifeq ($(UNAME_S), Linux)
	FLAGS		= -Wall -Wextra
	SRC	+=
endif

ifeq ($(UNAME_S), Darwin)
	FLAGS		= -Wall -Wextra -Werror
	SRC	+=
endif

$(shell mkdir -p $(DYNAMIC_DIR) $(DEBUG_DIR))

all: $(DYNAMIC_LIB)
	@echo "Compilation terminee. (realease)"

debug: $(DEBUG_LIB)
	@echo "Compilation terminee. (debug)"


$(DYNAMIC_LIB): $(DYNAMIC_OBJ)
	$(CC) $(OPTI) -shared -o $@ $(DYNAMIC_OBJ)
	ln -fs $(DYNAMIC_LIB) $(LINKNAME)

$(DEBUG_LIB): $(DEBUG_OBJ)
	$(CC) $(OPTI_DEBUG) -shared -o $@ $(DEBUG_OBJ)
	ln -fs $(DEBUG_LIB) $(LINKNAME)

-include $(DYNAMIC_OBJ:.o=.d)

$(DYNAMIC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(OPTI) -fPIC $(DEPENDS) -I$(HEAD_DIR) -I$(LIBFT_HEAD) -o $@ -c $<

-include $(DEBUG_OBJ:.o=.d)

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(OPTI_DEBUG) $(FLAGS) -g -fPIC $(DEPENDS) -I$(HEAD_DIR) -I$(LIBFT_HEAD) -o $@ -c $<



$(LIBFT_STATIC):
	make -C libft/ libft.a

$(LIBFT_DEBUG):
	make -C libft/ libft_debug.a

tests: $(DYNAMIC_LIB)
	$(CC) -o test0 test_srcs/test0.c  -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -o test1 test_srcs/test1.c  -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -o test2 test_srcs/test2.c  -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -o my_test3 test_srcs/test3.c -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -o my_test3+ test_srcs/test3+.c -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -o my_test4 test_srcs/test4.c -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)
	$(CC) -O0 -o my_test5 test_srcs/test5.c -L. -l$(LIBNAME) -I$(HEAD_DIR) -I$(LIBFT_HEAD)



.PHONY: clean fclean re debug norme gprof

clean:
	rm -f $(DYNAMIC_OBJ) $(DEBUG_OBJ)
	make -C libft clean

fclean: clean
	rm -f $(DYNAMIC_LIB) $(DEBUG_LIB) $(LINKNAME)
	make -C libft fclean

norme:
	$(NORMINETTE) $(SRC_DIR)/ $(HEAD_DIR)/
	make -C libft norme

re: fclean
	make
