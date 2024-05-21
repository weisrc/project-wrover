docker build -t wrover -f Dockerfile.arm .
docker rm -f wrover-app
docker run -d --restart always --name wrover-app -p 8000:80 -p 8443:443 wrover