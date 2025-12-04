##	CONTENUTO, DESCRIZIONE			ESEMPIO

METODO TARGET VERSIONE_HTTP		-	POST / HTTP/1.1
Host: porta client				-	Host: localhost:8080
User-Agent: ???					-	User-Agent: curl/8.17.1-DEV
ACCEPT: ???						-	Accept: */*
Content-Length: lunghezza msg	-	Content-Length: 10
	La lunghezza non deve 
	superare un tot
	il tot va definito
Content-Type:	???				-	Content-Type: application/x-www-form-urlencoded
Connection: TYPE				-	Connection: keep-alive
Transfer-Encoding: ???			-	???
\n	(riga vuota)				-	\n
MESSAGGIO						-	ciao monco

POST / HTTP/1.1
Host: localhost:8080
User-Agent: curl/8.17.1-DEV
Accept: */*
Content-Length: 10
Content-Type: application/x-www-form-urlencoded

ciao monco





## NOTION
/*
Il messaggio di richiesta del protocollo HTTP e composto da 4 parti:

- *Request line*
- Header
- Riga vuota
- Body

## Riga di richiesta

E composta da **metodo**, **URI** e **versione del protocollo**.

```basic
GET /index.html HTTP/1.1
```

### Metodi

I metodi possono essere GET, POST, HEAD, PUT, DELETE, PATCH, TRACE, OPTIONS, CONNECT.

I metodi piu comuni sono:

- **GET** →usato per ottenere il contenuto della risorsa indicata come URI
- **HEAD** → analogo a GET ma restituisce solo i campi dell’header (per esempio per verificare la data di modifica di un file)
- **POST** → usato per inviare informazioni al server. In questo caso l’URI indica che cosa sta inviando e il body ne indica il contenuto

### URI

L’URI (Uniform Resource Identifier) indica l’oggetto della richiesta*/