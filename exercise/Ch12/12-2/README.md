#### Question

Write a program that draws a tree showing the hierarchical parent-child relationships
of all processes on the system, going all the way back to init. For each process, the 
program should display the process ID and the command being executed. The output of 
the program should be similar to that produced by `pstree(1)`, although it does need 
not to be as sophisticated. The parent of each process on the system can be found 
by inspecing the `PPid:` line of all of the /proc/PID/status files on the system. 
Be careful to handle the possibilty that a process's parent (and thus its /proc/PID
directory) disappears during the scan of all /proc/PID directories.

#### Some tips
1. process `systemd` with pid `1` and process `kthread` with pid `2` have no parent
2. `pstree(1)` prints a tree with the root of `systemd`, and since `kthread` is not
   a child of `systemd` (it has no parent), it and its child processes are ignored.
3. The entries returned by `readdir(3)` is ordered by its entry name (PID), which 
   means `systemd` with pid 0 will be the first element of `array`.

#### Verification

```shell
$ cd c-12-2
$ make
$ ./12-2
1 systemd
	656 systemd-journal
	669 systemd-udevd
	807 systemd-oomd
	811 systemd-resolve
	812 systemd-userdbd
		57987 systemd-userwor
		57988 systemd-userwor
		57990 systemd-userwor
	813 auditd
	836 avahi-daemon
		865 avahi-daemon
	837 bluetoothd
	839 low-memory-moni
	840 polkitd
	843 power-profiles-
	844 rtkit-daemon
	846 accounts-daemon
	848 switcheroo-cont
	849 systemd-logind
	850 systemd-machine
	855 udisksd
	856 upowerd
	869 dbus-broker-lau
		891 dbus-broker
	890 abrtd
	893 chronyd
	926 ModemManager
	928 abrt-dump-journ
	929 abrt-dump-journ
	930 abrt-dump-journ
	932 alsactl
	933 firewalld
	967 NetworkManager
	974 cupsd
	978 gssproxy
	1113 wpa_supplicant
	1185 gdm
		1716 gdm-session-wor
			1777 gdm-wayland-ses
				1786 gnome-session-b
	1195 uresourced
	1325 packagekitd
	1458 pcscd
	1560 colord
	1740 systemd
		1745 (sd-pam)
		1781 dbus-broker-lau
			1784 dbus-broker
		1850 gnome-session-c
		1851 uresourced
		1856 gvfsd
			2592 gvfsd-trash
			7837 gvfsd-http
		1858 gnome-session-b
			2266 evolution-alarm
			2277 gsd-disk-utilit
		1864 gvfsd-fuse
		1871 pipewire
		1876 wireplumber
		1908 gnome-shell
			2720 pirate
				2727 pirate.bin
					2729 pirate.bin
					2839 pirate.bin
					2842 pirate.bin
					2882 core-linux-amd6
					2907 pirate.bin
			2749 Xwayland
			3113 bwrap
				3129 bwrap
					3130 chrome
						3145 cat
						3146 cat
						3161 chrome
							3201 chrome
						3163 zypak-sandbox
						3206 chrome
						8544 chrome
					3151 chrome_crashpad
					3155 chrome_crashpad
					3175 bwrap
						3176 chrome
							3178 chrome-sandbox
							3217 chrome
							3238 chrome
							3275 chrome
							3313 chrome
							3331 chrome
							3353 chrome
							3383 chrome
							29284 chrome
							32398 chrome
							55624 chrome
							58024 chrome
							58120 chrome
			3499 alacritty
				3526 zsh
					3660 bash
						6693 tmux:
		2027 at-spi-bus-laun
			2033 dbus-broker-lau
				2034 dbus-broker
		2044 xdg-permission-
		2047 gnome-shell-cal
		2054 evolution-sourc
		2059 pipewire-pulse
		2063 goa-daemon
		2074 evolution-calen
		2089 gvfs-udisks2-vo
		2104 gvfs-mtp-volume
		2111 goa-identity-se
		2113 dconf-service
		2124 evolution-addre
		2126 gvfs-gphoto2-vo
		2131 gvfs-goa-volume
		2139 gvfs-afc-volume
		2155 gnome-pomodoro
		2156 gjs
		2158 at-spi2-registr
		2179 ibus-daemon
			2283 ibus-dconf
			2285 ibus-extension-
			2405 ibus-engine-sim
			6771 ibus-engine-lib
		2181 gsd-a11y-settin
		2184 gsd-color
		2186 gsd-datetime
		2190 gsd-housekeepin
		2194 gsd-keyboard
		2201 gsd-media-keys
		2202 gsd-power
		2205 gsd-print-notif
		2207 gsd-rfkill
		2210 gsd-screensaver
		2211 gsd-sharing
		2215 gsd-smartcard
		2219 gsd-sound
		2222 gsd-usb-protect
		2228 gsd-wacom
		2308 ibus-portal
		2310 abrt-applet
		2311 gjs
		2315 gnome-user-shar
		2351 httpd
			2352 httpd
			2353 httpd
		2415 gsd-printer
		2552 gnome-calendar
		2633 xdg-desktop-por
		2638 xdg-document-po
			2642 fusermount
		2648 xdg-desktop-por
		2664 xdg-desktop-por
		2683 tracker-miner-f
		2724 LibCyber-V2.1.0
		2783 gsd-xsettings
		2810 ibus-x11
		3119 flatpak-session
		3123 p11-kit-server
			3404 p11-kit-remote
		3126 bwrap
			3127 xdg-dbus-proxy
		3138 flatpak-portal
			3165 bwrap
		3674 tmux:
			3709 zsh
			3755 zsh
			4038 zsh
			4088 zsh
			4371 zsh
			4434 zsh
			4733 zsh
				39023 nvim
			4800 zsh
			4877 zsh
			5230 zsh
			5332 zsh
			5430 zsh
			5825 zsh
			5971 zsh
			6325 zsh
				58535 12-2
			6454 zsh
				57982 nvim
					58181 xsel
		6796 gvfsd-metadata
	1763 gnome-keyring-d
	2146 sssd_kcm
	2438 abrt-dbus
```
