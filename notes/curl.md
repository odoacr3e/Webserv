curl -X POST -f "file=@www/var/favicon.ico" http://localhost:9021 # segfault
curl -X POST -F "file=@www/var/favicon.ico" http://localhost:9021/upload/
curl -X POST http://localhost:9021 -d "ciao"
curl -X POST -d "aaaaaaaaaa" -H "Content-Length: 6" http://localhost:9021
curl -X POST -d "a" -H "Content-Length: 6" http://localhost:9021
curl -F "file=@www/var/favicon.ico" http://localhost:9021
curl -X DELETE -H "Authorization: Giorno Giovanna" http://localhost:9021/upload/favicon.ico