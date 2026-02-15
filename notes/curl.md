curl -X POST -f "file=@www/var/favicon.ico" http://localhost:9021 # segfault
curl -X POST -F "file=@www/var/favicon.ico" http://localhost:9021/upload/
curl -X POST http://localhost:9021 -d "ciao"
curl -X DELETE -H "Authorization: Giorno Giovanna" http://localhost:9021/upload/favicon.ico