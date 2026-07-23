FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

USER root
RUN apt-get update \
  && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

USER vscode
