# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nguiard <nguiard@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/16 15:42:20 by nguiard           #+#    #+#              #
#    Updated: 2023/01/21 17:05:00 by nguiard          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =	$(addsuffix .cpp, 	\
		$(addprefix srcs/,	\
		main 				\
		))

SHELL	= /bin/zsh

OBJ		= ${SRC:srcs/%.cpp=obj/%.o}

CC		= c++

INCLUDE = -Iinclude/

CFLAGS	= -Wall -Werror -Wextra -std=c++98 ${INCLUDE} -g #-fsanitize=address

NAME	= ircserv

MAKE	= --silent --jobs

MY_MAKE	= make --silent --jobs

total	:= $(shell echo ${SRC} | wc -w)

percent	:= 0

current	:= 1

save_percent ?= 0

len := $(shell echo ${NAME} | wc -c)

all:
	@${MY_MAKE} setup
	@echo -ne "\033[5;3H\033[1;32mObjets deja compilés!\033[m";
	@make --silent ${OBJ}
	@echo -ne "\033[10;3H\033[1;32m${NAME} deja compilé!\033[m";
	@echo -ne "\033[12;H"
	@make --silent ${NAME}
	@${MY_MAKE} end_make

obj/%.o: srcs/%.cpp
	@printf "\033[5;2H                                                  \033[5;3H%s" $< ${<:.c=⠀⠀}
	@echo -ne "\033[12;H"
	@${CC} ${CFLAGS} -c $< -o ${<:srcs/%.cpp=obj/%.o}	
	@$(eval percent=$(shell expr ${current} "*" 100 / ${total}))
	@echo -ne "\033[6;3H"
	@printf "%d/%d:   \t\t%d%%" ${current} ${total} ${percent}
	$(call loading,${percent})
	@$(eval current=$(shell expr ${current} + 1))

${NAME}: ${OBJ}
	@${CC} ${OBJ} ${CFLAGS}  -lXext -lX11 -lm -o ${NAME};
	@printf "\033[10;3H\033[1mCompilation de ${NAME} \033[32mterminee\033[1;37m!";
	@${MY_MAKE} end_make

setup:
	@echo -ne "\033[0;0H\033[J\033[?25l"
	@echo -ne "\033[1;38;5;57m"
	@echo "╔══════════════════════════════════════════════════╗"
	@echo "╟────────────────┤ \033[3mTURBO MAKEFILE ├────────────────╢"
	@echo "║                                                  ║"
	@echo "║ Objets:                                          ║"
	@echo "║                                                  ║"
	@echo "║                                                  ║"
	@echo "║                                                  ║"
	@echo "║                                                  ║"
	@echo "║ Executable:                                      ║"
	@echo "║                                                  ║"
	@echo "╚══════════════════════════════════════════════════╝"
	@echo -ne "\033[m"

define loading
	@$(eval save_percent=$(shell expr ${percent}))
	@echo -ne "\033[7;13H"
	@save_percent=$(shell expr ${save_percent} "*" 2); \
	while (( $${save_percent} > 7)) ; do \
		echo -ne "\033[47;32m█"; \
		((save_percent-=7)); \
	done;	\
	echo "\u`printf '%x' $(expr 9608 + $$save_percent)`"		
	@echo -ne "\033[0m"
	@true
endef

end_make:
	echo -ne "\033[12;H\033[?25h"

re: fclean all

fclean:
	@rm -rf ${NAME} ${OBJ} 

clean:
	@rm -rf ${OBJ}

.PHONY: clean fclean re end_make all setup libft_rule
