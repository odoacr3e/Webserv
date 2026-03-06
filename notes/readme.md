# Webserv Architecture Documentation

## Overview

Questo progetto implementa un **HTTP Web Server event-driven in C++**, ispirato al comportamento di **nginx**.

### Caratteristiche principali

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

```
Server
├── Client
│   └── Request
│
├── CGI
│
└── Conf
    ├── s_conf_server
    └── s_conf_location
```

---

# Server

Classe principale che gestisce l'intero webserver.

## Responsabilità principali

- gestione socket server
- gestione client connessi
- gestione `poll()`
- dispatch delle request
- gestione CGI
- generazione delle response

---

# Strutture principali

## Server:: pollfd array 

```cpp
std::vector<pollfd> _addrs;
```

Contiene tutti i file descriptor monitorati da `poll()`.

### Tipi possibili

- server socket
- client socket
- pipe CGI read
- pipe CGI write

### Esempio

```
[0] server_fd_1
[1] server_fd_2
[2] client_fd_1
[3] client_fd_2
[4] pipe_fd
```

---

## Server:: FD metadata

```cpp
std::vector<s_fd_data> _fd_data;
```

Contiene informazioni sul tipo di file descriptor.

```cpp
struct s_fd_data
{
    s_cgi*      cgi;
    Client*     client;
    e_fd_type   type;
    bool        cgi_ready;
};
```

Serve per sapere cosa rappresenta ogni `pollfd`.

---

## Client map

```cpp
std::map<int, Client*> _clients;
```

Associazione:

```
socket_fd -> Client object
```

---

## Server configuration map

```cpp
std::map<int, t_conf_server*> _server_data;
```

Associazione:

```
server_socket -> configurazione server
```

---

## Cookie storage

```cpp
cookieMap _cookie_map;
```

Gestisce sessioni client tramite cookie.

---

# Client

Rappresenta una connessione HTTP con un client.

## Responsabilità

- mantenere lo stato della connessione
- contenere la request corrente
- buffer dati ricevuti
- configurazione server/location attiva

---

## Variabili principali

### Socket client

```cpp
int _sockfd;
```

Socket del client.

### Socket server

```cpp
int _srvfd;
```

Socket del server che ha accettato la connessione.

### Request HTTP

```cpp
Request _request;
```

Oggetto che rappresenta la request HTTP.

### Buffer dati

```cpp
std::vector<char> _buffer;
```

Buffer usato per:

- body request
- output CGI
- contenuti file

---

## Interazione con CGI

Il client espone metodi per comunicare con CGI:

```
readCgi()
writeCgi()
bindCgiSocket()
```

Gestiscono:

- pipe
- comunicazione processo CGI
- integrazione con `poll()`

---

# Request

Classe responsabile del **parsing della request HTTP**.

Gestisce:

- method
- URL
- headers
- body
- error handling

---

## Struttura request

```
METHOD URI HTTP_VERSION
Headers
Body
```

### Esempio

```
GET /index.html HTTP/1.1
Host: localhost
Content-Length: 10

body data
```

---

## Variabili principali

### Metodo HTTP

```cpp
std::string _method;
```

Metodi supportati:

```
GET
POST
DELETE
```

---

### URL

```cpp
std::string _url;
```

---

### Headers

```cpp
headermap _header;
```

Struttura:

```
key -> value
```

---

### Body

```cpp
std::string _body;
```

---

### Status code

```cpp
e_http_codes _status_code;
```

Status code risultante dal parsing.

---

## Parsing principale

```
checkHeader()
```

Controlla:

- validità metodo
- headers
- lunghezza URI
- body

---

# CGI

Gestisce l'esecuzione di script esterni.

Supporta:

- CGI standard
- FastCGI

---

## Struttura CGI

```cpp
struct s_cgi
{
    std::string input;
    char* output;

    int pipe[2];
    int poll_index[2];

    int pid;

    bool isFastCgiBool;
};
```

---

## Processo CGI

```
Client Request
      │
      ▼
Server detects CGI
      │
      ▼
fork()
      │
      ▼
exec script
      │
      ▼
pipe output
      │
      ▼
Server reads pipe
      │
      ▼
Response to client
```

---

## Header CGI

Le CGI usano un header custom:

```
OK|0123456789|
```

Formato:

```
status | bytes_to_read
```

Dimensione header:

```
14 bytes
```

---

# Configuration System

Configurazione stile **nginx**.

Gestita dalla classe:

```
Conf
```

---

## Esempio file config

```
server {

    listen 127.0.0.1:8080;

    root /var/www;

    location /images {
        root /var/www/img;
    }

}
```

---

# s_conf_server

Rappresenta un blocco `server`.

### Variabili principali

```
root
index
server_names
client_max_body_size
autoindex
mask_methods
err_pages
```

---

# s_conf_location

Rappresenta un blocco `location`.

### Variabili principali

```
root
index
cgi_path
script_type
autoindex
alias
fastcgi_bool
mask_methods
err_pages
```

---

# Poll Event System

Il server utilizza `poll()` per gestire I/O multiplexing.

### Tipi di file descriptor

```
FD_SERVER
FD_CLIENT
FD_PIPE_RD
FD_PIPE_WR
```

---

# Event Loop

Struttura semplificata:

```cpp
while (true)
{
    poll();

    for each fd
    {
        if SERVER
            accept();

        if CLIENT
            read request;

        if PIPE
            read CGI output;
    }
}
```

---

# Cookie System

I cookie sono gestiti tramite:

```
s_cookieData
```

### Struttura

```cpp
struct s_cookieData
{
    std::string login;
    std::string id;
    s_cgi* cgi;
    bool exist;
};
```

---

# HTTP Methods

Metodi supportati:

| Method | Descrizione |
|------|------|
| GET | recupera risorsa |
| POST | invia dati |
| DELETE | elimina risorsa |

Gestiti tramite:

```
Server::runMethod()
```

che dispatcha verso:

```
getMethod()
postMethod()
deleteMethod()
```

---

# Response Generation

La risposta HTTP viene costruita tramite:

```
Server::createResponse()
```

### Struttura risposta

```
HTTP/1.1 STATUS_CODE
Headers

Body
```

---

# Autoindex

Se una directory non contiene `index`, il server può generare automaticamente una pagina HTML.

Funzione:

```
createAutoindex()
```

---

# Maximum Connections

```
MAX_CONNECTION = 1024
```

Scelta coerente con nginx.

---

# Error Handling

Gli errori HTTP vengono gestiti tramite:

```
status_codes.hpp
```

E possono essere associati a pagine personalizzate nel file di configurazione.

### Esempio

```
error_page 404 /404.html;
```

---

# Concurrency Model

```
Single Thread
Event Driven
Non Blocking I/O
```

Gestito tramite:

```
poll()
```

---

# Summary

| Component | Responsabilità |
|------|------|
| Server | gestione connessioni |
| Client | stato connessione |
| Request | parsing HTTP |
| CGI | esecuzione script |
| Conf | configurazione server |