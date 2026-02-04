#SECTION - debug commands

make: all
	

help: 
	@echo "make help\nmake conf_test\nmake request_test\nmake update"

update: all
	chmod +x conf_testing.sh
	chmod +x request_testing.sh
	./conf_testing.sh update
	./request_testing.sh update

conf_test: all
	chmod +x conf_testing.sh
	./conf_testing.sh all

request_test: all
	chmod +x request_testing.sh
	./request_testing.sh all

#SECTION - global variables
PROJ_ROOT = ..
PROJ_OBJ_DIR = $(PROJ_ROOT)/build/srcs
OBJ_DIR = build
OUTPUT_DIR = output
UTILS_DIR = utils_dir
CONF_DIR = conf_dir
REQUEST_DIR = request_dir
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
								string/string.o string/buffer.o url.o env.o directory.o \
								cgi.o html_pokedex.o)

WEB_OBJ = $(addprefix $(PROJ_OBJ_DIR)/, $(CPP) $(PARSING) $(UTILS))

#SECTION - list testing srcs
SRC_CONF 	= $(addprefix $(CONF_DIR)/, 	$(shell ls srcs_cpp/$(CONF_DIR)))
SRC_REQUEST = $(addprefix $(REQUEST_DIR)/, $(shell ls srcs_cpp/$(REQUEST_DIR)))
SRC_UTILS 	= $(addprefix $(UTILS_DIR)/, 	$(shell ls srcs_cpp/$(UTILS_DIR)))
SRCS = $(addprefix srcs_cpp/, $(SRC_CONF) $(SRC_TESTING) $(SRC_UTILS))

OBJ = 			$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
OBJ_CONF = 		$(addprefix $(OBJ_DIR)/, $(SRC_CONF:.cpp=.o) $(SRC_UTILS:.cpp=.o))
OBJ_REQUEST = 	$(addprefix $(OBJ_DIR)/, $(SRC_REQUEST:.cpp=.o) $(SRC_UTILS:.cpp=.o))

#SECTION - execs
NAME = conf request

all: compile_web $(OBJ_DIR)/conf $(OBJ_DIR)/request

compile_web:
	$(MAKE) -C $(PROJ_ROOT)

$(OBJ_DIR)/conf: $(OBJ_CONF) $(WEB_OBJ) | $(OBJ_DIR)
	$(CC) $(FLAGS) $(CPPFLAGS) $(OBJ_CONF) $(WEB_OBJ) -o $@

$(OBJ_DIR)/request: $(OBJ_REQUEST) $(WEB_OBJ) | $(OBJ_DIR)
	$(CC) $(FLAGS) $(CPPFLAGS) $(OBJ_REQUEST) $(WEB_OBJ) -o $@

$(OBJ_DIR)/%.o: srcs_cpp/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)/srcs_cpp/$(CONF_DIR)/
	mkdir -p $(OBJ_DIR)/srcs_cpp/$(REQUEST_DIR)/
	mkdir -p $(OBJ_DIR)/srcs_cpp/$(UTILS_DIR)/

#$(OBJ_DIR)/%/%.o: %.cpp
#	@mkdir -p $(dir $@)
#	$(CC) $(FLAGS) -c $< -o $@

clean: 
	rm -rf $(OBJ_DIR)/conf $(OBJ_DIR)/request

fclean: clean
	rm -rf $(OBJ_DIR) $(OUTPUT_DIR)

re: fclean all

.PHONY: compile_web conf_test request_test