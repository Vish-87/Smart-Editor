FROM gcc:13

WORKDIR /app
COPY . .

# Linux needs -pthread instead of -lws2_32
RUN g++ -std=c++17 -O2 server.cpp -o server -pthread

CMD ["./server"]
