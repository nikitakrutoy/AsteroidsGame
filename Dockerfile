FROM silkeh/clang:latest
RUN apt-get update && \
    apt-get install -y \
      libx11-dev\
      cmake
WORKDIR /app/build