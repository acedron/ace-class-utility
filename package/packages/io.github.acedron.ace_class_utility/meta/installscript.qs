/****************************************************************************
** Ace Class Utility - A Basic Utility App for Classes.
** Copyright (C) 2021  acedron <acedrons@yahoo.co.jp>
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/

function Component()
{
    // Default Constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType == "windows" || systemInfo.productType == "winrt") {
        // Windows Start Menu Shortcut
        component.addOperation("CreateShortcut",
            "@TargetDir@/ace_class_utility/ace_class_utility-win32_x64.exe",
            "@StartMenuDir@/Ace Class Utility.lnk",
            "workingDirectory=@TargetDir@/ace_class_utility",
            "iconPath=@TargetDir@/ace_class_utility/icon.ico");

        // Windows Desktop Shortcut
        component.addOperation("CreateShortcut",
            "@TargetDir@/ace_class_utility/ace_class_utility-win32_x64.exe",
            "@DesktopDir@/Ace Class Utility.lnk",
            "workingDirectory=@TargetDir@/ace_class_utility",
            "iconPath=@TargetDir@/ace_class_utility/icon.ico");
    } else if (systemInfo.productType != "osx" && systemInfo.productType != "darwin") {
        // GNU/Linux X11/Wayland Desktop Entry
        component.addOperation("CreateDesktopEntry",
            "ace_class_utility.desktop",
            "Type=Application\n\
Encoding=UTF-8\n\
Name=Ace Class Utility\n\
Comment=A Basic Utility App for Classes.\n\
Exec=@TargetDir@/ace_class_utility/ace_class_utility-linux_x64\n\
Icon=@TargetDir@/ace_class_utility/icon.png\n\
Terminal=False");
    }
}
