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

- return 0 when listening with your parameters
- return 1 when not listening with your parameters

```
$ ./netstat-port 0.0.0.0 8080 808000
Listen: 0.0.0.0:8080 by uid=808000

$ netstat -lnt | grep 8080
tcp        0      0 0.0.0.0:8080                0.0.0.0:*                   LISTEN      
```

## time

```
$ time netstat -lnte | grep '0.0.0.0:8080\|808000'
tcp        0      0 0.0.0.0:8080                0.0.0.0:*                   LISTEN      808000     3760955    

real    0m0.010s
user    0m0.001s
sys     0m0.011s

$ time ./netstat-port 0.0.0.0 8080 808000
Listen: 0.0.0.0:8080 by uid=808000

real    0m0.006s
user    0m0.000s
sys     0m0.006s
```
