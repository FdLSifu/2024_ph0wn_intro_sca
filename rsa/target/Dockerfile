FROM debian:12-slim as builder
RUN apt update && apt install -y cmake build-essential pkg-config libgmp-dev
COPY CMakeLists.txt .
COPY src /src
RUN cmake .
RUN make

FROM debian:12-slim
RUN apt update && apt install -y libgmp10
COPY --from=builder /rsa_leakage /rsa_leakage
EXPOSE 6000
CMD ["/rsa_leakage"]