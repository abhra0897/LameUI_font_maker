Icon files are embedded into the font by `lameui_fontgen.py` script. A collection of icons are provided in zipped format: `icons_all.tar.gz`. 

You may download and add more png icons as needed. But they must follow these properties:

1. icons must be `.png` image.
2. Image mode must be `RGBA`. No other modes are supported.
3. Icons should have transparent background and black foreground.

Folders:

- **sys_icons:** This folder contains all the system icons files. These files are needed by LameUI. It's recommended NOT to change any file here.
- **usr_icons:** This folder has all the user icon files. Keep required icons here to add them in the font.

Icon files must be named in following pattern:

`[<hex_id>]<name>.png`

Examples:

`[01]home.png, [0B]remove.png, [1A]settings.png`

*Note: IDs must be in Hexadecimal format and NOT in decimal.*

Allowed hex number range:
- for syetem icons: 01-1D (Dec: 1-29)
- for user icons  : 80-FF (Dec: 128-255)

Icon files with out-of-range ID will be ignored and won't be added to the generated bitmap font.
