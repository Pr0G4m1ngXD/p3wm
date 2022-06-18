![p3wm logo](./p3wm.png?raw=true "Title")
# p3wm
> Powerfull, Perfection, Pretty  

## Easily customizable windowmanager

The p3 window manager lets you easily customize the behaivior and look of your windows with more customization than other WMs. It's built with C++ and XLib to keep the source code also easy to customize.

## Installation
To install prebuild binaries of p3wm, use your distro's package manager. Curently p3wm is available on:
* currently none. i will make it availible once i could call it a powerfull and pretty window manager.

(more distributions soon)

## Manual Installation
If you cannot use a package manager that is supported download the binaries tarball and extract the following 4 files/directories to these locations:
| File in tarball | Location to put it            | Permissions |
|-----------------|-------------------------------|-------------|
| p3wm.x11        | /usr/bin/p3wm                 | +x          |
| config.conf     | /usr/share/p3wm/config.conf   | +r          |
| keybinds.conf   | /usr/share/p3wm/keybinds.conf | +r          |
| theme/          | /usr/share/p3wm/theme/        | +r          |

## Configuration
Feel like you want to change the behaivor, shortcuts or look? Change the config files in your ~/.p3wm directory, if the file gets corrupted or doesnt exsist it will revert to the defaults stored in the /usr/share/p3wm directory. Explanations for the variables in the config files are available in [TODO: CONFIGVARS.md](./CONFIGVARS.md).

## Contributing
If you're intrested in reporting/fixing issues and contributing to the codebase, please see [CONTRIBUTING.md](./CONTRIBUTING.md) for more information on how to get started.
