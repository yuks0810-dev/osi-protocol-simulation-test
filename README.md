![image](https://github.com/user-attachments/assets/d909badd-f958-4ac1-907b-7656f30ce513)

参照：https://hldc.co.jp/blog/2019/07/11/2819/

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

### 結果

![image](https://github.com/user-attachments/assets/a21ecdcd-4d0a-4ec0-b9b1-e4c50d9e6164)

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

### 結果

![CleanShot 2024-12-22 at 20 33 03](https://github.com/user-attachments/assets/8e242c1f-8d0e-4f2e-b8e0-e6b7167b4095)


## TCP jaist server access test

外部ネットワークに接続してみる。

Compile
```
gcc jaist_client.c -o jaist/client
```

Run
```
./jaist/client
```

### 結果

![image](https://github.com/user-attachments/assets/4f18e868-8956-4897-a824-9a07b634dfde)
