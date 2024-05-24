### Man in the middle

**This project is from a network security branch**

This project is implementation of the Address Resolution Protocol spoofing/poisoning method, which is one of the most basic Man In The Middle attacks. This attack is possible using a vulnerability present in the the way the ARP protocol works and interacts in a network.

The whole project is done in a ***virtual machine***: You are only allowed to spoof IPs that belong to you, such as your
VM. Spoofing other IPs might result in problems and/or sanctions.


* Program takes the following four arguments:
```
◦ source ip
◦ source mac address 
◦ target ip
◦ target mac address
```

* Program has following options:
```
◦ -v	verbose mode to print packet information.
◦ -g	gratuitous ARP broadcast.
◦ -h	hostname resolution for IPv4 addresses.
```

* To run project
```
make
./ft_malcolm [spoofed_ip] [spoofed_mac] [ip] [mac] [..options]
```

When started, program will have to wait for an ARP request sent on the broadcast by the target, requesting the source IP,
before sending a single ARP reply to the target and exit. 

**References**:
https://www.rfc-editor.org/rfc/rfc826
https://www.rfc-editor.org/rfc/rfc7042
https://www.ccnahub.com/ip-fundamentals/understanding-arp-broadcast-message/
https://lynxbee.com/sending-arp-request-and-receiving-arp-reply-using-c-code/
https://www.practicalnetworking.net/series/arp/gratuitous-arp/
