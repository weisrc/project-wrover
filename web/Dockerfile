FROM node AS builder

WORKDIR /app

COPY package-lock.json package.json ./
RUN npm ci

COPY app app
COPY components components
COPY lib lib
COPY public public
COPY .eslintrc.json next-env.d.ts ./
COPY tsconfig.json postcss.config.js tailwind.config.ts ./
COPY next.docker.config.mjs next.config.mjs
RUN npm run build

FROM httpd:2.4-alpine

WORKDIR /app

COPY --from=builder /app/build /usr/local/apache2/htdocs/
COPY keys/cert.pem /usr/local/apache2/conf/server.crt
COPY keys/key.pem /usr/local/apache2/conf/server.key
COPY httpd.conf /usr/local/apache2/conf/httpd.conf

EXPOSE 80
EXPOSE 443
