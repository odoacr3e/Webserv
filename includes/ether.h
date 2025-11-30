
#include <iostream>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <cstdlib>

#define LISTENING_PORT 8080