# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: abobas <abobas@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2020/07/04 14:02:54 by abobas        #+#    #+#                  #
#    Updated: 2020/10/28 20:50:19 by abobas        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

SRC = 			main.cpp \
				Log.cpp \
				Server.cpp \
				Socket.cpp \
				response/Response.cpp \
				response/Data.cpp \
				response/Resource.cpp \
				response/Directory.cpp \
				response/Proxy.cpp \
				response/Cgi.cpp \
				response/Upload.cpp \
				json/Json.cpp \
				http/HttpParser.cpp \
				http/HttpRequest.cpp \
				http/HttpResponse.cpp

SRC_DIR	=		./src/

SRC :=			$(SRC:%=$(SRC_DIR)%)
			
INCLUDE :=		$(INCLUDE:%=$(INCLUDE_DIR)%)

NAME = 			webserv

FLAGS =			-Wall -Werror -Wextra -std=c++17

C =				gcc

all: 			$(NAME)

$(NAME):		$(SRC)
				$(CXX) $(FLAGS) $(SRC) -o $(NAME)

run:
				sudo ./webserv --config config/config.json

clean:
				rm -rf *.o

fclean:			clean
				rm -rf $(NAME)

re:				fclean all
