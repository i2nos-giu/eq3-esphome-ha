## Development workflow

Development and testing are performed directly on Home Assistant OS.

The working ESPHome files are periodically synchronized to the Git repository
using rsync over SSH. On Home Assistant OS, rsync may be temporarily installed
when required using:

    apk add rsync

The Raspberry Pi is typically up for long periods of time, so this installation
is usually needed only after a reboot.

Secrets and local configuration files are kept on the Home Assistant system
and are not included in the repository.


