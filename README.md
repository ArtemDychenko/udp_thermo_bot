# udp_thermo_bot

UDP BOT, sending cyclically simulated digital thermometer data to the server.

# How to run the project:

1. Create a new directory in the project:
```
mkdir build
cd build
```
2. Generate build files:
```
cmake ..
```
3. Build the project:
```
make
```
4. Run the executable:
```
./udp_thermo_bot 127.0.0.1 12345 1
```
### Parameters:
- `<server_ip>`: IP address of the UDP server (e.g., 127.0.0.1 for localhost)
- `<server_port>`: Port number (1-65535, preferably above 1024)
- `<interval_sec>`: Time interval between measurements in seconds

