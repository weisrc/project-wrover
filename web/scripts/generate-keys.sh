openssl req -x509 -newkey rsa:4096 -keyout keys/key.pem -out keys/cert.pem \
    -sha256 -days 3650 -nodes \
    -subj "/C=CA/ST=QC/L=Montreal/O=Dawson/OU=Etech/CN=example.com"