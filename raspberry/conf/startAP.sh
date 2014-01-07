ifconfig wlan1 10.0.0.1
hostapd -dd /etc/hostapd.conf &
dnsmasq -d -C /etc/dnsmasq.conf &
