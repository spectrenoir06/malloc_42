# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adoussau <adoussau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/20 21:46:05 by adoussau          #+#    #+#              #
#    Updated: 2017/12/09 17:32:00 by adoussau         ###   ########.fr        #
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
			print.c \
			block.c \
			page.c \
			print_mem.c \
			realloc.c \

HEAD_DIR	= includes
SRC_DIR		= src
DEBUG_DIR	= debug
STATIC_DIR	= static
GPROF_DIR	= gprof
C_HEAD_DIR	= debug

DYNAMIC_DIR	= dynamic

STATIC_OBJ	= $(patsubst %.c,$(STATIC_DIR)/%.o,$(SRC))
DEBUG_OBJ	= $(patsubst %.c,$(DEBUG_DIR)/%.o,$(SRC))
DYNAMIC_OBJ	= $(patsubst %.c,$(DYNAMIC_DIR)/%.o,$(SRC))

CC			= gcc -fdiagnostics-color=always
NORMINETTE	= norminette
OPTI		= -O0
OPTI_DEBUG	= -O0
DLFLAGS		= -shared -fPIC
DEPENDS		= -MT $@ -MD -MP -MF $(subst .o,.d,$@)

OBJ		= $(SRC:.c=.o)

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
	$(CC) $(OPTI) $(DLFLAGS) -o $@ $(DYNAMIC_OBJ)
	ln -fs $(DYNAMIC_LIB) $(LINKNAME)

$(DEBUG_LIB): $(DEBUG_OBJ)
	$(CC) $(OPTI_DEBUG) $(DLFLAGS) -o $@ $(DEBUG_OBJ)
	ln -fs $(DEBUG_LIB) $(LINKNAME)

-include $(DYNAMIC_OBJ:.o=.d)

$(DYNAMIC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(FLAGS) $(OPTI) -I$(HEAD_DIR) -o $@ -c $<

-include $(DEBUG_OBJ:.o=.d)

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(OPTI_DEBUG) $(FLAGS) -g -I$(HEAD_DIR) -o $@ -c $<


tests: $(DYNAMIC_LIB)
	$(CC) -o test0 test_srcs/test0.c   -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test1 test_srcs/test1.c   -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test2 test_srcs/test2.c   -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test3 test_srcs/test3.c   -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test3+ test_srcs/test3+.c -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test4 test_srcs/test4.c   -L. -lft_malloc -I$(HEAD_DIR)
	$(CC) -o test5 test_srcs/test5.c   -L. -lft_malloc -I$(HEAD_DIR)


.PHONY: clean fclean re debug norme gprof

clean:
	rm -f $(DYNAMIC_OBJ) $(DEBUG_OBJ)

fclean: clean
	rm -f $(DYNAMIC_LIB) $(DEBUG_LIB) $(LINKNAME)

norme:
	$(NORMINETTE) $(SRC_DIR)/ $(HEAD_DIR)/

re: fclean
	make
