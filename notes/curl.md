curl -X POST -f "file=@www/var/favicon.ico" http://localhost:9021 # segfault
curl -X POST -F "file=@www/var/favicon.ico" http://localhost:9021