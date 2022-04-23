#!/bin/bash
cd "$(dirname "$0")"

if [ "$EUID" -ne 0 ]; then
    echo "Please run this install script with elevated permissions."
    exit 1
fi

mv SGDBop /usr/local/bin/SGDBop
chmod +x /usr/local/bin/SGDBop
desktop-file-install com.steamgriddb.SGDBop.desktop --delete-original --rebuild-mime-info-cache

su - $SUDO_USER
xdg-mime default com.steamgriddb.SGDBop.desktop x-scheme-handler/sgdb

echo "Installation complete! You may delete this install script."
exit 0
