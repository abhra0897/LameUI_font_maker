This folder contains all the user icon files. Icons are embedded within the font and are not meant to be used separately. If no user icons are needed, keep this folder empty.

Icon files must be named in following pattern:

`[<hex_id>]<name>.png`

Examples:

`[81]headset.png, [FA]mail-open.png, [9C]navigate.png`

*Note: IDs must be in Hexadecimal format and NOT in decimal.*

Allowed hex number range:
- for syetem icons: 01-1D (Dec: 1-29)
- **for user icons  : 80-FF (Dec: 128-255)**

Icon files with out-of-range ID will be ignored and won't be added to the generated bitmap font.