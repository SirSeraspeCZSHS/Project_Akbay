ESP32 PlatformIO Arduino starter

Files:
- platformio.ini: PlatformIO project config
- src/main.cpp: Blink + serial example

Build & upload (Windows)

1. Install VS Code and PlatformIO IDE extension, or install PlatformIO Core:

```powershell
pip install -U platformio
```

2. Build:

```powershell
pio run
```

3. Upload (auto-detects serial port or set `upload_port` in `platformio.ini`):

```powershell
pio run -t upload
```

4. Monitor serial output:

```powershell
pio device monitor -b 115200
```

If using VS Code, open the folder and use PlatformIO commands from the status bar.
