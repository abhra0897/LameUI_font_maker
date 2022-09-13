This folder contains all the system icon files. Icons are embedded within the font and are not meant to be used separately. 

System icon files are needed by LameUI. Do NOT delete files from this folder without replacing them with suitable alternatives. For example, if you delete **`[06]check-mark.png`**, replace it with some other file with the same hex ID in its name (06). Because, LaemUI expects a check mark icon with ID: 06. If that icon is missing from font, runtime error will occur.

To add your own custom icons, use the user icons folder: `usr_icons` instead.

Icon files must be named in following pattern:

`[<hex_id>]<name>.png`

Examples:

`[01]home.png, [0B]remove.png, [1A]settings.png`

*Note: IDs must be in Hexadecimal format and NOT in decimal.*

Allowed hex number range:
- **for syetem icons: 01-1D (Dec: 1-29)**
- for user icons  : 80-FF (Dec: 128-255)

Icon files with out-of-range ID will be ignored and won't be added to the generated bitmap font.