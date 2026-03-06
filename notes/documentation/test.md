# Webserv Architecture Documentation

## Overview

Questo progetto implementa un **HTTP Web Server event-driven in C++**, ispirato al comportamento di **nginx**.

Caratteristiche principali:

- gestione **multi server (ip:porta)**
- architettura **non-blocking**
- I/O multiplexing tramite **poll()**
- supporto **CGI / FastCGI**
- parsing completo **HTTP/1.1**
- gestione **cookies**
- configurazione stile **nginx**

Il server utilizza una struttura centrale (`Server`) che coordina:

- connessioni
- client
- parsing request
- generazione response
- esecuzione CGI

---

# Core Components

Server
├── Client
│ └── Request
│
├── CGI
│
└── Conf
├── s_conf_server
└── s_conf_location


---

# Server

Classe principale che gestisce l'intero webserver.

Responsabilità principali:

- gestione socket server
- gestione client connessi
- gestione poll()
- dispatch delle request
- gestione CGI
- generazione delle response

## Strutture principali

### pollfd array

```cpp
std::vector<pollfd> _addrs;

Contiene tutti i file descriptor monitorati da poll().

Tipi possibili:

server socket

client socket

pipe CGI read

pipe CGI write

Esempio:

[0] server_fd_1
[1] server_fd_2
[2] client_fd_1
[3] client_fd_2
[4] pipe_fd

std::vector<s_fd_data> _fd_data;

struct s_fd_data
{
    s_cgi*      cgi;
    Client*     client;
    e_fd_type   type;
    bool        cgi_ready;
};

Serve per sapere cosa rappresenta ogni pollfd.