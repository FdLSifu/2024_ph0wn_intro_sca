FROM debian:12-slim as builder
RUN apt update && apt install -y cmake build-essential
COPY CMakeLists.txt .
COPY src /src
RUN cmake .
RUN make

FROM scratch
COPY --from=builder /aes_leakage /aes_leakage
EXPOSE 5000
CMD ["/aes_leakage"]