# check-tcport

Check listening tcp port by uid, port and ipaddr on Linux

## build

```
make
```

## example

```
./check-tcport ipaddr port uid
```

```
$ ./netstat-port 0.0.0.0 8080 808000
Listen: 0.0.0.0:8080 by uid=808000

$ netstat -lnt | grep 8080
tcp        0      0 0.0.0.0:8080                0.0.0.0:*                   LISTEN      
```
