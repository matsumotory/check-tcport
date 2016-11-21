# check-tcport

Check listening tcp port by uid, port and ipaddr on Linux

## build

```
make
```

## example

- return value is 0 when listening with your parameters
- return value is 1 when not listening with your parameters

```
./check-tcport ipaddr port uid
```

```
$ ./check-tcport 0.0.0.0 8080 808000
state=TCP_LISTEN ipaddr=0.0.0.0 port=8080 inode=3760955 uid=808000

$ netstat -lnte | grep '0.0.0.0:8080\|808000'
tcp        0      0 0.0.0.0:8080                0.0.0.0:*                   LISTEN      808000     3760955
```

## time

```
$ time netstat -lnte | grep '0.0.0.0:8080\|808000'
tcp        0      0 0.0.0.0:8080                0.0.0.0:*                   LISTEN      808000     3760955    

real    0m0.010s
user    0m0.001s
sys     0m0.011s

$ time ./check-tcport 0.0.0.0 8080 808000
state=TCP_LISTEN ipaddr=0.0.0.0 port=8080 inode=3760955 uid=808000

real    0m0.005s
user    0m0.000s
sys     0m0.005s
```

