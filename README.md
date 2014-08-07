wifi-hostapd-ap

The origin project is located at Google code.Because of the GFW launched by China's government,I copy the project to Github.
Mean while,fixed some bugs to make it running on Ubuntu 13.10 & Ubuntu 14.04.

Tips to run it on Ubuntu14.04:

In 64bit System
	cd /tmp
	wget http://archive.ubuntu.com/ubuntu/pool/universe/w/wpa/hostapd_1.0-3ubuntu2.1_amd64.deb
	sudo dpkg -i hostapd*.deb
	sudo apt-mark hold hostapd

In 32bit System
	cd /tmp
	wget http://archive.ubuntu.com/ubuntu/pool/universe/w/wpa/hostapd_1.0-3ubuntu2.1_i386.deb
	sudo dpkg -i hostapd*.deb
	sudo apt-mark hold hostapd
===============
