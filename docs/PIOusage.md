Open Serial Monitor
pio device monitor
pio device monitor -b 115200

Build + Upload + Monitor
pio run -t upload && pio device monitor -b 115200

Clean Project
pio run -t clean

List Available Serial Ports
pio device list


PlatformIO inside VS Code:
✅ Build → Ctrl + Alt + B
✅ Upload → Ctrl + Alt + U
✅ Monitor → Ctrl + Alt + S
✅ Clean → Command Palette → PlatformIO: Clean