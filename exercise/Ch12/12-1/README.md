#### Question

Write a program that lists the process ID and command name for all processes being
run by the user named in the program's command-line argument. (You may find the 
`userIdFromName()` function from Listing 8-1, on page 159, useful.) This can be
done by inspecting the `Name` and `Uid` lines of all the `/proc/PID/status` files
on the system. Walking through all of the `/proc/PID` directories on the system 
requires the use of `readdir(3)`, which is described in section 18.8. Make sure your
program correctly handles the possibility that a `/proc/PID` directory disappears
between the time that the program determines that the directory exists and the time
that it tries to open the corresponding `/proc/PID/status` file.

#### Verification

```shell
$ cd c-12-1
$ make
gcc 12-1.c ../../../../lib/error_functions.c \
    ../../../../users_groups/ugid_functions-c/ugid_functions.c \
        -o 12-1

$ ./12-1 $(id -u)
systemd 1740
(sd-pam) 1745
gnome-keyring-d 1763
gdm-wayland-ses 1777
dbus-broker-lau 1781
dbus-broker 1784
gnome-session-b 1786
gnome-session-c 1850
uresourced 1851
gvfsd 1856
gnome-session-b 1858
gvfsd-fuse 1864
pipewire 1871
wireplumber 1876
gnome-shell 1908
at-spi-bus-laun 2027
dbus-broker-lau 2033
dbus-broker 2034
xdg-permission- 2044
gnome-shell-cal 2047
evolution-sourc 2054
pipewire-pulse 2059
goa-daemon 2063
evolution-calen 2074
gvfs-udisks2-vo 2089
gvfs-mtp-volume 2104
goa-identity-se 2111
dconf-service 2113
evolution-addre 2124
gvfs-gphoto2-vo 2126
gvfs-goa-volume 2131
gvfs-afc-volume 2139
gnome-pomodoro 2155
gjs 2156
at-spi2-registr 2158
ibus-daemon 2179
gsd-a11y-settin 2181
gsd-color 2184
gsd-datetime 2186
gsd-housekeepin 2190
gsd-keyboard 2194
gsd-media-keys 2201
gsd-power 2202
gsd-print-notif 2205
gsd-rfkill 2207
gsd-screensaver 2210
gsd-sharing 2211
gsd-smartcard 2215
gsd-sound 2219
gsd-usb-protect 2222
gsd-wacom 2228
evolution-alarm 2266
gsd-disk-utilit 2277
ibus-dconf 2283
ibus-extension- 2285
ibus-portal 2308
abrt-applet 2310
gjs 2311
gnome-user-shar 2315
httpd 2351
httpd 2352
httpd 2353
ibus-engine-sim 2405
gsd-printer 2415
gnome-calendar 2552
gvfsd-trash 2592
xdg-desktop-por 2633
xdg-document-po 2638
fusermount 2642
xdg-desktop-por 2648
xdg-desktop-por 2664
tracker-miner-f 2683
pirate 2720
LibCyber-V2.1.0 2724
pirate.bin 2727
pirate.bin 2729
Xwayland 2749
gsd-xsettings 2783
ibus-x11 2810
pirate.bin 2839
pirate.bin 2842
core-linux-amd6 2882
pirate.bin 2907
bwrap 3113
flatpak-session 3119
p11-kit-server 3123
bwrap 3126
xdg-dbus-proxy 3127
bwrap 3129
chrome 3130
flatpak-portal 3138
cat 3145
cat 3146
chrome_crashpad 3151
chrome_crashpad 3155
chrome 3161
zypak-sandbox 3163
bwrap 3165
bwrap 3175
chrome 3176
chrome-sandbox 3178
chrome 3201
chrome 3206
chrome 3217
chrome 3238
chrome 3275
chrome 3313
chrome 3331
chrome 3353
chrome 3383
p11-kit-remote 3404
alacritty 3499
zsh 3526
bash 3660
tmux:
zsh 3709
zsh 3755
zsh 4038
zsh 4088
zsh 4371
zsh 4434
zsh 4733
zsh 4800
zsh 4877
zsh 5230
zsh 5332
zsh 5430
zsh 5825
zsh 5971
zsh 6325
zsh 6454
tmux:
ibus-engine-lib 6771
gvfsd-metadata 6796
java 7503
fsnotifier 7613
jcef_helper 7634
jcef_helper 7635
jcef_helper 7650
jcef_helper 7668
jcef_helper 7673
gvfsd-http 7837
chrome 8544
zsh 8617
chrome 29284
chrome 32398
chrome 32432
xsel 32547
12-1 33034
```
