PROJ_ROOT = ..
PROJ_OBJ_DIR = $(PROJ_ROOT)/build/
OBJ_DIR = build/
CC = c++
FLAGS = -Wall -Wextra -Werror -g -D_GLIBCXX_DEBUG
CPPFLAGS = -std=c++98

#SECTION - list webserver object
PARSING = $(addprefix parsing/, parseConf.o parseRequest.o $(CONF_PARSING) $(REQUEST_PARSING))
CONF_PARSING = $(addprefix conf/, parseEvent.o parseMain.o parseServer.o parseLocation.o parseHttp.o)
REQUEST_PARSING = $(addprefix request/, utils.o)

CPP = $(addprefix cpp/, 		Client.o \
		$(addprefix Server/,	Server.o getter.o setter.o methods.o setup_utils.o)\
		$(addprefix conf/, 		getter.o setter.o print.o Conf.o) \
		$(addprefix request/, 	getter.o setter.o print.o Request.o))
UTILS = $(addprefix utils/, 	utils_page1.o path_checker.o value_checker.o \
								string/string.o url.o env.o directory.o \
								cgi.o html_pokedex.o)

WEB_OBJ = $(addprefix $(PROJ_OBJ_DIR), $(CPP) $(PARSING) $(UTILS))

#SECTION - list testing srcs
SRC_CONF 	= $(addprefix conf/, 	$(shell ls srcs_cpp/conf))
SRC_REQUEST = $(addprefix request/, $(shell ls srcs_cpp/request))
SRC_UTILS 	= $(addprefix utils/, 	$(shell ls srcs_cpp/utils))
SRCS = $(addprefix srcs_cpp/, $(SRC_CONF) $(SRC_TESTING) $(SRC_UTILS))

OBJ = $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))

#SECTION - execs
NAME = conf request

compile_web:
	$(MAKE) -C $(PROJ_ROOT)

all: compile_web | $(OBJ_DIR) $(NAME:%=$(OBJ_DIR)/%)

$(OBJ_DIR)/conf: $(OBJ)
	$(CC) $(FLAGS) $(CPPFLAGS) $(OBJ) $(WEB_OBJ) -o $@

$(OBJ_DIR)/request: $(OBJ)
	$(CC) $(FLAGS) $(CPPFLAGS) $(OBJ) $(WEB_OBJ) -o $@

$(OBJ_DIR)/srcs_cpp/%.o: srcs_cpp/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(PROJ_OBJ_DIR) : 
	mkdir -p $(OBJ_DIR)

$(PROJ_OBJ_DIR)/%/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: compile_web