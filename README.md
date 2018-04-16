# subcal

subcal is a commmand line tool that will allows the following:
- Subnet calculator, classful and classless
- Subnet design, find subnetmasks that meet the requirements
- Get detailed information about a specific network/subnet 
- Find Residnetsubnet and more 

### Installation
```
1. Clone or dowload subcal
2. Extract and cd to subcal directory 
3. make
4. sudo make install
```

### Get information specific classful network
```
$ subcal info --ip4 192.168.10.3/26
    Subnet Address:         192.168.10.0
    Host Range:             192.168.10.1 - 192.168.10.62
    Broadcast Address:      192.168.10.63
    CIDR Mask:              255.255.255.0 (/24)
    Subnet Mask:            255.255.255.192 (/26)
    Subnet Size:            62
    Num of Subnets:         4
    Network Class:          Class C
    Subnet Mask Bits:       TODO:
```
### Get information specific classless network
```
$ subcal info --ip4 192.168.10.3/22 /26
    Subnet Address:         192.168.10.0
    Host Range:             192.168.10.1 - 192.168.10.62
    Broadcast Address:      192.168.10.63
    CIDR Mask:              255.255.252.0 (/22)
    Subnet Mask:            255.255.255.192 (/26)
    Subnet Size:            62
    Num of Subnets:         16
    Network Class:          Class C
    Subnet Mask Bits:       TODO:
```
### Classful network subnetting (classful subnet generator)
```
    $ subcal gen --ip4 192.168.10.3/28
    ---------------------------------------------------------------------------
       Subnet        |             Host Range              |   Broadcast      |
    -----------------+-------------------------------------+------------------+
    192.168.10.0     | 192.168.10.1    -  192.168.10.14    | 192.168.10.15    |
    192.168.10.16    | 192.168.10.17   -  192.168.10.30    | 192.168.10.31    |
    192.168.10.32    | 192.168.10.33   -  192.168.10.46    | 192.168.10.47    |
    192.168.10.48    | 192.168.10.49   -  192.168.10.62    | 192.168.10.63    |
    192.168.10.64    | 192.168.10.65   -  192.168.10.78    | 192.168.10.79    |
    192.168.10.80    | 192.168.10.81   -  192.168.10.94    | 192.168.10.95    |
    192.168.10.96    | 192.168.10.97   -  192.168.10.110   | 192.168.10.111   |
    192.168.10.112   | 192.168.10.113  -  192.168.10.126   | 192.168.10.127   |
    192.168.10.128   | 192.168.10.129  -  192.168.10.142   | 192.168.10.143   |
    192.168.10.144   | 192.168.10.145  -  192.168.10.158   | 192.168.10.159   |
    192.168.10.160   | 192.168.10.161  -  192.168.10.174   | 192.168.10.175   |
    192.168.10.176   | 192.168.10.177  -  192.168.10.190   | 192.168.10.191   |
    192.168.10.192   | 192.168.10.193  -  192.168.10.206   | 192.168.10.207   |
    192.168.10.208   | 192.168.10.209  -  192.168.10.222   | 192.168.10.223   |
    192.168.10.224   | 192.168.10.225  -  192.168.10.238   | 192.168.10.239   |
    192.168.10.240   | 192.168.10.241  -  192.168.10.254   | 192.168.10.255   |
    -----------------+-------------------------------------+------------------+
```
### Classless network subnetting (CIDR subnet calcolator)
```
$ subcal gen --ip4 192.168.10.3/16 /28 
    ---------------------------------------------------------------------------
       Subnet        |             Host Range              |   Broadcast      |
    -----------------+-------------------------------------+------------------+
    192.168.0.0      | 192.168.0.1     -  192.168.0.14     | 192.168.0.15     |
    192.168.0.16     | 192.168.0.17    -  192.168.0.30     | 192.168.0.31     |
    192.168.0.32     | 192.168.0.33    -  192.168.0.46     | 192.168.0.47     |
    192.168.0.48     | 192.168.0.49    -  192.168.0.62     | 192.168.0.63     |
    192.168.0.64     | 192.168.0.65    -  192.168.0.78     | 192.168.0.79     |
    192.168.0.80     | 192.168.0.81    -  192.168.0.94     | 192.168.0.95     |
    192.168.0.96     | 192.168.0.97    -  192.168.0.110    | 192.168.0.111    |
    192.168.0.112    | 192.168.0.113   -  192.168.0.126    | 192.168.0.127    |
    192.168.0.128    | 192.168.0.129   -  192.168.0.142    | 192.168.0.143    |
    192.168.0.144    | 192.168.0.145   -  192.168.0.158    | 192.168.0.159    |
    192.168.0.160    | 192.168.0.161   -  192.168.0.174    | 192.168.0.175    |
    192.168.0.176    | 192.168.0.177   -  192.168.0.190    | 192.168.0.191    |
    192.168.0.192    | 192.168.0.193   -  192.168.0.206    | 192.168.0.207    |
    192.168.0.208    | 192.168.0.209   -  192.168.0.222    | 192.168.0.223    |
    192.168.0.224    | 192.168.0.225   -  192.168.0.238    | 192.168.0.239    |
    192.168.0.240    | 192.168.0.241   -  192.168.0.254    | 192.168.0.255    |
    192.168.1.0      | 192.168.1.1     -  192.168.1.14     | 192.168.1.15     |
    192.168.1.16     | 192.168.1.17    -  192.168.1.30     | 192.168.1.31     |
    192.168.1.32     | 192.168.1.33    -  192.168.1.46     | 192.168.1.47     |
    192.168.1.48     | 192.168.1.49    -  192.168.1.62     | 192.168.1.63     |
    Use '-v' to generate all possible subnets(4096).
```
By default the number of generated subnets is 20, you can use '-v' to generate all the subnets.
### Subnet Design 
A subnetting design requires 200 subnets and 120 hosts/subnet, find all possible mask that meet this requirements 
```
$ subcal design -s 200 -h 120 -c A
    255.255.0.0 (/16), #subnets: 256, #hosts: 65534
    255.255.128.0 (/17), #subnets: 512, #hosts: 32766
    255.255.192.0 (/18), #subnets: 1024, #hosts: 16382
    255.255.224.0 (/19), #subnets: 2048, #hosts: 8190
    255.255.240.0 (/20), #subnets: 4096, #hosts: 4094
    255.255.248.0 (/21), #subnets: 8192, #hosts: 2046
    255.255.252.0 (/22), #subnets: 16384, #hosts: 1022
    255.255.254.0 (/23), #subnets: 32768, #hosts: 510
    255.255.255.0 (/24), #subnets: 65536, #hosts: 254
    255.255.255.128 (/25), #subnets: 131072, #hosts: 126
    Total: 10 possible subnet masks.

$ subcal design -s 200 -h 120 -c B
    255.255.255.0 (/24), #subnets: 256, #hosts: 254
    255.255.255.128 (/25), #subnets: 512, #hosts: 126
    Total: 2 possible subnet masks.

$ subcal design -s 200 -h 120 -c C
    Network Class 'C' cannot produce 200 subnets and 120 hosts. Try different Class.
```
**note: subcal syntax allow the mask to be in DDN or prefix format e.g. /24 or 255.255.255.0 both format are valid.
###Future plan
    - ipv4 summary
    - ipv6 subnet calculator
    - ipv4 to ipv6 convertion
    - Simplify or extend ipv6 address
    - other stuff related to ipv6
