## TCP protocol test

Compile
```
gcc tcp_server.c -o tcp/server
gcc tcp_client.c -o tcp/client
```

Run
```
./tcp/server

# 別のターミナルで実行
./tcp/client
```

## UDP protocol test

Compile
```
gcc udp_server.c -o udp/server
gcc udp_client.c -o udp/client
```

Run
```
./udp/server

# 別のターミナルで実行
./udp/client
```

## TCP jaist server access test

Compile
```
gcc jaist_server.c -o jaist/server
gcc jaist_client.c -o jaist/client
```

Run
```
./jaist/server

# 別のターミナルで実行
./jaist/client
```