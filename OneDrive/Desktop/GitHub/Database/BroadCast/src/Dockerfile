FROM gcc:latest as builder
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN gcc -o broadcast -static broadcast.c

FROM alpine:latest as runtime
COPY --from=builder /usr/src/myapp/broadcast /broadcast
ENTRYPOINT ["./broadcast"]