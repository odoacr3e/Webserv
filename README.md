# Custom C++ Web Server

A lightweight **HTTP/1.1 web server written from scratch in C++**.
This project focuses on understanding how web servers work internally: sockets, HTTP parsing, request routing, and response generation.

---

## Overview

This server implements the core components required to handle HTTP requests:

* TCP socket handling
* HTTP request parsing
* Static file serving
* Basic routing
* Multiple client handling
* Configurable server behavior

The goal of the project is educational: to understand the internal mechanics behind modern web servers such as Nginx or Apache.

---

## Features

* HTTP/1.1 request parsing
* Support for **GET / POST / DELETE**
* Static file serving
* Configurable routes
* Multiple client handling using `poll()` / `select()`
* Request body parsing with `Content-Length`
* Basic error handling (404, 500, etc.)
* Simple configuration system

---

## Project Structure

```
.
├── src/
│   ├── server/        # Server socket management
│   ├── http/          # Request and response parsing
│   ├── router/        # Route handling
│   └── utils/         # Utility functions
│
├── include/           # Header files
├── config/            # Server configuration
├── www/               # Static files
└── main.cpp
```

---

## How It Works

1. The server creates a **listening socket**.
2. Incoming connections are accepted.
3. Client sockets are monitored using `poll()` or `select()`.
4. Incoming data is read using `recv()`.
5. The HTTP request is parsed.
6. The server generates an HTTP response.
7. The response is sent back to the client.

Workflow:

```
Client
  ↓
TCP Connection
  ↓
HTTP Request Parsing
  ↓
Routing
  ↓
Response Generation
  ↓
HTTP Response
```

---

## Build

Compile using `make`:

```bash
make
```

Run the server:

```bash
./webserv config/server.conf
```

---

## Example Request

```
GET /index.html HTTP/1.1
Host: localhost
```

Response:

```
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234
```

---

## Configuration

Example configuration file:

```
server {
    listen 8080;
    root ./www;
    index index.html;
}
```

---

## Future Improvements

* CGI support
* Chunked transfer encoding
* HTTP keep-alive
* Better request parsing
* Logging system
* Performance optimizations

---

## License

This project is intended for educational purposes.
