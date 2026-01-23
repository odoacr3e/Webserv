**STRUTTURA CLASSI**

# `SERVER`
##		std::vector<struct pollfd> _addrs; //pollfd per poll(), una struct per ogni ip:porta in ascolto

	public:
		Server();
		~Server();
		
		void 			addSocket();
		struct pollfd	*getAddrs(void);
		size_t			getAddrSize(void) const;
		void			checkForConnection();
{
1. descrizione:    gestire socket e connessioni su più porte
2. contiene:
	- vector di pollfd contenente:
		- socket server (listen)
		- socket client (established) -> forse con map<socket, Client *>
}

`CLIENT`  --> 	{
descrizione:	contiene dati del client. Quali dati?
contiene:
- request (Classe?)
- response (Classe?)
}

##	POST / HTTP/1.1
##	Host: localhost:8080
##	User-Agent: curl/8.17.1-DEV
##  Accept: */*
##  Content-Length: 10
##  Content-Type: application/x-www-form-urlencoded
`REQUEST` --> 	{
descrizione:	(quello che ora prendiamo con recv)
contiene:
- metodo
- lunghezza contenuto
- header
- body
- location
}

`RESPONSE` -->  	{
descrizione:	(attuale createResponse)
}

`CONF` --> 	{
descrizione:	una classe/struct che contiene i dati presi dal config
contiene:
- struct s_conf_server:			contenuto blocchi server
- std::vector<t_conf_server>:	array di blocchi server	
- struct s_conf_location:		contenuto blocchi location
}

`CONF_SERVER` --> 	{
descrizione:	struct che contiene i dati di un blocco server
contiene:
- location(std::map<std::string, t_conf_location>):	
	mappa di location accettate <-->	struct s_conf_location
	esempio:	location /pippo
		provando ad accedere a location["/pippo"] : ottengo dati location

- ipports(std::map<std::string, int>):
	mappa di indirizzi ip <--->	numero porta associata
	esempio:	listen 127.0.0.1:8080
		provando ad accedere a ipports["127.0.0.1"] : ottengo 8080

- root(std::string):	path dove trovare i file. Viene inserita prima della path richiesta.
	esempio:	root /var/www/html
		POST /images/piedini_loli HTTP/1.1
		/images/piedini_loli ----->	/var/www/html/images/piedini_loli

- server_names(std::vector<std::string>):
	vector di nomi del server.
	esempio:	server_name server server.com www.server.com *server.com
		##	POST / HTTP/1.1
		##	Host: server
		essendo che host == server_name, questo server verrà usato per la richiesta

- client_max_body_size(int):	massima lunghezza del body per le richieste client	
}

`CONF_LOCATION` --> 	{
descrizione:	struct che contiene i dati di un blocco location
contiene:
}

			CLIENT A }	
SERVER ---> CLIENT B } --->	{Response, 	Request}
			CLIENT C }			|			|
								|			|
								|			Request_data
								|			Parsing richiesta
								|			
								|
								|
								Tornare codice errore
								Tornare risorsa