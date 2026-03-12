# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rzamolo- <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/12 15:25:38 by rzamolo-          #+#    #+#              #
#    Updated: 2026/03/12 15:25:49 by rzamolo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #

#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rzamolo- <rzamolo-@student.42madrid.com>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/10 17:29:29 by rzamolo-          #+#    #+#              #
#    Updated: 2026/03/10 18:27:32 by rzamolo-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX					=	c++
CXXFLAGS			=	-Wall -Wextra -Werror -g -std=c++98
CXXFLAGS			+=	-Iincludes
DEPFLAGS			=	-MMD -MP

RM					=	rm -rf
NAME				=	ircserv
VALGRIND_LOG		=	valgrind.log

PORT				=	
PASSWD				=	

SRC_FILES			=	main.cpp Server.cpp Client.cpp
SRC					=	$(addprefix sources/, $(SRC_FILES))
OBJ					=	$(SRC:.cpp=.o)
DEP					=	$(SRC:.cpp=.d)

all:				$(NAME)

$(NAME):			$(OBJ)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -o $@ $^

%.o:				%.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(DEP)

fclean:				clean
	$(RM) $(NAME) $(VALGRIND_LOG) client

re:					fclean all

run:				re
	$(MAKE) clean
	clear
	valgrind --leak-check=full --log-file=$(VALGRIND_LOG) ./$(NAME) $(PORT) $(PASSWD)

client:
	$(CXX) $(CXXFLAGS) -o client client.cpp


.PHONY:				all clean fclean re

-include $(DEP)

