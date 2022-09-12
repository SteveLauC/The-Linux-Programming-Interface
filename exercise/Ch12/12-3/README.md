#### Question

Write a program that lists all processes that have a particular file pathname 
open. This can be achieved by inspecting the contents of all of the 
`/proc/PID/fd/*` symbolic links. This will require nested loops employing 
`readdir(3)` to scan all `/proc/PID` directories, and then the contents of all 
`/proc/PID/fd` entries within each `/proc/PID` directory. To read the contents 
of a `/proc/PID/fd/n` symbolic link requires the use of `readlink(2)`, described 
in Section 18.5.

#### Some tips

1. Some `/proc/PID/fd`s can only be read by `root` (which is the owner of that 
   process), I ignored these processes in my implementation.

   ```shell
   $ ./12-3 | wc
        98     197    1826
   
   $ sudo ./12-3 | wc
    163     327    2910
   ```

#### Verification

```shell
$ cd c-12-3
$ make
$ ./12-3 /dev/null
Name:	systemd
Name:	pipewire
Name:	wireplumber
Name:	pipewire-pulse
Name:	LibCyber-V2.1.0
Name:	pirate.bin
Name:	pirate.bin
Name:	core-linux-amd6
Name:	tmux: server
Name:	dbus-broker-lau
Name:	dbus-broker
Name:	gnome-user-shar
Name:	httpd
Name:	httpd
Name:	httpd
Name:	gnome-keyring-d
Name:	gnome-session-b
Name:	gnome-session-c
Name:	uresourced
Name:	gnome-session-b
Name:	gvfsd
Name:	gvfsd-fuse
Name:	gnome-shell
Name:	at-spi-bus-laun
Name:	dbus-broker-lau
Name:	dbus-broker
Name:	xdg-permission-
Name:	gnome-shell-cal
Name:	evolution-sourc
Name:	goa-daemon
Name:	evolution-calen
Name:	goa-identity-se
Name:	gvfs-udisks2-vo
Name:	dconf-service
Name:	evolution-addre
Name:	gvfs-mtp-volume
Name:	gvfs-gphoto2-vo
Name:	gvfs-goa-volume
Name:	gvfs-afc-volume
Name:	gnome-pomodoro
Name:	gjs
Name:	at-spi2-registr
Name:	ibus-daemon
Name:	gsd-a11y-settin
Name:	gsd-color
Name:	gsd-datetime
Name:	gsd-housekeepin
Name:	gsd-keyboard
Name:	gsd-media-keys
Name:	gsd-power
Name:	gsd-print-notif
Name:	gsd-rfkill
Name:	gsd-screensaver
Name:	gsd-sharing
Name:	gsd-smartcard
Name:	gsd-sound
Name:	gsd-usb-protect
Name:	gsd-wacom
Name:	evolution-alarm
Name:	ibus-dconf
Name:	ibus-extension-
Name:	gsd-disk-utilit
Name:	ibus-portal
Name:	abrt-applet
Name:	gjs
Name:	gsd-printer
Name:	ibus-engine-sim
Name:	gnome-calendar
Name:	gvfsd-trash
Name:	xdg-desktop-por
Name:	xdg-document-po
Name:	xdg-desktop-por
Name:	xdg-desktop-por
Name:	alacritty
Name:	tracker-miner-f
Name:	Xwayland
Name:	gsd-xsettings
Name:	ibus-x11
Name:	bwrap
Name:	flatpak-session
Name:	p11-kit-server
Name:	bwrap
Name:	xdg-dbus-proxy
Name:	bwrap
Name:	chrome
Name:	flatpak-portal
Name:	chrome_crashpad
Name:	chrome_crashpad
Name:	chrome
Name:	bwrap
Name:	bwrap
Name:	chrome
Name:	chrome
Name:	p11-kit-remote
Name:	ibus-engine-lib
Name:	gvfsd-metadata
Name:	nvim
Name:	xsel
```
