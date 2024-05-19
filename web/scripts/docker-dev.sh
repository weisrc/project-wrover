docker build -t wrover .
docker run --rm -t --name wrover-app -p 8080:80 -p 8443:443 wrover