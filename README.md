IoT Botnet
C-based botnet for compromising IoT devices via weak Telnet credentials and launching DDoS attacks.
Setup

Install dependencies: sudo apt install gcc libssl-dev
Run ./setup.sh
Execute: ./botnet

Usage

Scans for Telnet (port 23) with default creds.
Loads payload to infected devices.
C2 interface via TCP (port 6666).
DDoS: UDP flood, TCP flood, GRE attack.

Warning: For CTF use only in controlled environments.
