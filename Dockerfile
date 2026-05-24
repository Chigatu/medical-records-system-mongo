FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest AS builder

WORKDIR /app
COPY src/ src/
COPY configs/ configs/
COPY CMakeLists.txt .

RUN export CCACHE_DISABLE=1 && \
    mkdir build && cd build && \
    cmake .. && \
    make -j4

FROM ghcr.io/userver-framework/ubuntu-22.04-userver:latest

WORKDIR /app
COPY --from=builder /app/build/medical_api .
COPY --from=builder /app/configs ./configs

EXPOSE 8080

CMD ["./medical_api", "--config", "configs/static_config.yaml", "--config_vars", "configs/config_vars.yaml"]
