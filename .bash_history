clear
cd ..
ls
cd
ls
clear
cd uart
ls
g++ uart_send.cpp -o uart_send
sudo su
clear
g++ led_test.cpp -o led_test
jioh@rpi:~/uart $ g++ led_test.cpp -o led_test
led_test.cpp:55:1: fatal error: error writing to /tmp/ccofDGJu.s: No space left on device
compilation terminated.
jioh@rpi:~/uart $ g++ led_test.cpp -o led_test
led_test.cpp:55:1: fatal error: error writing to /tmp/ccJhPYV8.s: No space left on device
compilation terminated.
df -h
sudo apt clean
sudo journalctl --vacuum-time=3d
sudo du -ah / | sort -rh | head -n 20
df -h
sudo su
g++ led_test.cpp -o led_test
sudo g++ led_test.cpp -o led_test
lls
ls
./led_test 
sudo g++ led_test.cpp -o led_test
./led_test 
ls -l /dev/serial0
dmesg | grep tty
echo -n "1" > /dev/serial0
ls -l /dev/serial0
dmesg | grep tty
sudo g++ led_test.cpp -o led_test
./led_test 
echo -n "1" > /dev/serial0
echo -n "0" > /dev/serial0
sudo chmod 666 /dev/serial0
echo -n "0" > /dev/serial0
echo -n "1" > /dev/serial0
echo -n "0" > /dev/serial0
sudo raspi-config
su
sudo poweroff
ls
clear
dtoverlay -a | grep uart
dtoverlay -h uart 2
dtoverlay -h uart2
gpio readall
dtoverlay -h uart0
vim /boot/config.txt 
clear
python3 uart_test.py 
sudo poweroff
clean
clear
python3 tcp.py 
hostname -I
sudo raspi-config
hostname -I
nc 192.168.4.1 5000
sudo poweroff
clear
echo "HiESP" | nc 192.168.4.1 5000
clear
nc 192.168.4.1 5000
sudo raspi-config
ping 192.168.4.1
sudo reboot
ifconfig
sudo nmcli dev wifi connect "ESP_WIFI" password "12345678"
sudo systemctl status NetworkManager
sudo poweroff
clear
cd
cd uart/
ls
g++ -o uart_send uart_send.cpp
sudo g++ -o uart_send uart_send.cpp
ls
sudo ./uart_send on
sudo ./uart_send off
sudo ./uart_send on
sudo g++ -o uart_send uart_send.cpp
sudo ./uart_send on
sudo ./uart_send off
sudo ./uart_send on
sudo ./uart_send off
