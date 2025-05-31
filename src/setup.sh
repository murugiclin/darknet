#!/bin/bash
gcc -o botnet src/main.c -I src
chmod +x botnet
echo "[*] Setup complete. Run ./botnet"
