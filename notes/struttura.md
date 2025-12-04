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

`ANSWER` -->  	{
descrizione:	(attuale create_http)
}

`CONF` --> 	{
descrizione:	una classe/struct che contiene i dati presi dal config
contiene:
}