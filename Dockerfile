FROM alpine:3
# RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.tuna.tsinghua.edu.cn/g' /etc/apk/repositories
RUN apk add cmake gcc g++ libgcc build-base make --no-cache
WORKDIR /app/
COPY . /app/
WORKDIR /app/build
RUN cmake ..
RUN make -j4
