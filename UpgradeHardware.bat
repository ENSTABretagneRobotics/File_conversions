cd /d "%~dp0"

set HARDWARE_PATH=%~d0\Personal data\Software\Other\Software development\C++\Robotics\Current\UxVCtrl\Hardware

recycle -f .\dataflash2SBG\MAVLinkProtocol.h
copy /B /Y /Z "%HARDWARE_PATH%\MAVLinkProtocol.h" .\dataflash2SBG\MAVLinkProtocol.h

recycle -f .\dataflash2tlog\MAVLinkProtocol.h
copy /B /Y /Z "%HARDWARE_PATH%\MAVLinkProtocol.h" .\dataflash2tlog\MAVLinkProtocol.h

recycle -f .\logstate2TLOG\MAVLinkProtocol.h
copy /B /Y /Z "%HARDWARE_PATH%\MAVLinkProtocol.h" .\logstate2TLOG\MAVLinkProtocol.h

pause
