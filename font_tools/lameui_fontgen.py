import json
import os
from PIL import Image, ImageOps
import sys
from operator import itemgetter

# def rename_files():
#     i = 1
#     for png_file in os.listdir("."):
#         if png_file.__contains__(".png"):
#             new_name = png_file[4:]
#             new_name = new_name.split("-sharp_out.png")[0] + ".png"
#             new_name = "[%02X]%s" % (i, new_name)
#             print(new_name)
#             os.rename(png_file, new_name)
#             i = i+1

def is_valid_icon_name(icon_name, is_sys_icon):
    icon_id = -1
    is_valid = False
    if len(icon_name) > 3 and icon_name[0] == '[' and icon_name[3] == ']':
        try:
            icon_id = int(icon_name[1:3], 16)
            if (is_sys_icon == False) and (icon_id < 128 or icon_id > 255):
                print("\n[Error] USER ICON file: %s has out of range hex number! Allowed range for user icon is Hex: 80-FF (Dec: 128-255).\n" % icon_name)
            elif (is_sys_icon == True) and (icon_id < 1 or icon_id > 29):
                print("\n[Error] SYSTEM ICON file: %s has out of range hex number! Allowed range for syetem icon is Hex: 01-1D (Dec: 1-29).\n" % icon_name)
            else:
                is_valid = True
        except(ValueError):
            print("\n[Error] File: %s has invalid hex number: %s!" % (icon_name, icon_name[1:3]))
            print("Note: Number must be in Hex format. Allowed hex number range:")
            print("    for syetem icons: 01-1D (Dec: 1-29)")
            print("    for user icons  : 80-FF (Dec: 128-255)")
            print()
    else:
        print("\n[Error] File: %s has invalid structure! " % icon_name)
        print("Icon name must follow this pattern: [<hex_id>]<name>.png")
        print("Examples: [01]home.png, [1D]stop.png etc.")
        print("Allowed hex number range:")
        print("    for syetem icons: 01-1D (Dec: 1-29)")
        print("    for user icons  : 80-FF (Dec: 128-255)")
        print()

    return (is_valid, icon_id)

def rgba_invert(rgba_im):
    #print('Inverting RGBA image')
    r,g,b,a = rgba_im.split()
    rgb_image = Image.merge('RGB', (r,g,b))
    inverted_image = ImageOps.invert(rgb_image)
    r2,g2,b2 = inverted_image.split()
    inverted_rgba_image = Image.merge('RGBA', (r2,g2,b2,a))
    return inverted_rgba_image

# this is a workaround method to create 1-bit image from RGBA
# Because, directly calling im.convert('1') creates blank image somethimes
def rgba_to_monochrome(rgba_im):
    # print('Converting RGBA image to monochrome')
    rgba_im.load() # required for rgba_im.split()
    mono_im = Image.new("1", rgba_im.size, color=0)
    mono_im.paste(rgba_im, mask=rgba_im.split()[3]) # 3 is the alpha channel
    return mono_im

valid_icon_files = []   # [{'id': png_id, 'path': icon_path, 'width': width, 'x_offset': offset}, ...]
def combine_font_and_icons(font_png_path, sys_icons_dir, usr_icons_dir):
    font_image = Image.open(font_png_path)
    font_image = rgba_to_monochrome(font_image)
    font_texture_height = font_image.height
    total_width = 0

    sys_icons_list = os.listdir(sys_icons_dir)
    sys_icons_cnt = len(os.listdir(sys_icons_dir))
    # If number of system icons is not exactly 29, abort operation
    if (sys_icons_cnt < 29):
        print("\n[Error] Must be 29 system icons. But found %d. Aborting!\n" % sys_icons_cnt)
        exit(-1)


    usr_icons_list = os.listdir(usr_icons_dir)
    all_icons_list = sys_icons_list + usr_icons_list

    i = 0
    for i in range(len(all_icons_list)):
        icon_name = all_icons_list[i]
        if icon_name.endswith(".png") or icon_name.endswith(".PNG"):
            if i < sys_icons_cnt:
                is_valid, png_id = is_valid_icon_name(icon_name, True)
                # if sys_icon is not valid, abort.
                if is_valid == False:
                    print("\n[Error] Aborting due to invalid sytem icon!\n")
                    exit(-1)
            else:
                is_valid, png_id = is_valid_icon_name(icon_name, False)
            
            if is_valid:
                if i < sys_icons_cnt:
                    icon_path = sys_icons_dir + icon_name
                else:
                    icon_path = usr_icons_dir + icon_name
                icon_img = Image.open(icon_path)
                if (icon_img.mode == "RGBA"):
                    print("File: %s is valid.. ID: %d" % (icon_name, png_id))
                    valid_icon_files.append({'id': png_id, 'path': icon_path, 'width': 0, 'x_offset': 0}) # append id and path. Keep width and x_offset as 0
                else:
                    print("File: %s has unsupported color mode: %s, Only RGBA allowed.. Skipping.." % (icon_name, icon_img.mode))

    # Sort the font files list by the id
    valid_icon_files.sort(key=itemgetter('id'));
    total_width = font_image.width
    for icon in valid_icon_files:
        icon_img = Image.open(icon['path'])
        height_percent = (font_texture_height / float(icon_img.height))
        icon_new_width = int((float(icon_img.width) * float(height_percent)))
        icon['width'] = icon_new_width
        icon['x_offset'] = total_width
        total_width += icon_new_width
    
    # print(valid_icon_files)

    combined_image = Image.new('1', (total_width, font_texture_height))
    x_offset = 0
    combined_image.paste(font_image, (x_offset, 0))
    x_offset += font_image.width
    for icon in valid_icon_files:
        icon_img = Image.open(icon['path'])
        # Sequence of steps for best result:
        # 1. invert, 2. convert to monochrome, 3. resize, 4. paste to canvas
        icon_img = rgba_invert(icon_img)
        icon_img = rgba_to_monochrome(icon_img)
        icon_img = icon_img.resize((icon['width'], font_texture_height))
        combined_image.paste(icon_img, (x_offset,0))
        x_offset += icon_img.width

    combined_image.show()
    return combined_image
 
#returns tuple (name, height, width, payload)
#Serialization order is left-to-right, top-to-bottom
def make_image_array(mono_img_data):
    width, height = mono_img_data.size
    print("Processing file %s color depth is %s, width %d, height %d" % (file_path, mono_img_data.mode, width, height))    
    name = os.path.splitext(font_png_name)[0]
    payload = []
    for x in range(0, width):
        bit_list = [0] * 8
        bit_index = 0
        for y in range(0, height):
            pixel = mono_img_data.getpixel((x,y))
            if pixel:
                bit_list[bit_index] = 1
            bit_index += 1
            if bit_index == 8:
                byte = 0
                for bit in bit_list:
                    byte = (byte << 1) | bit
                payload.append(byte)
                bit_index = 0
                bit_list = [0] * 8
        if bit_index % 8: #there is something left over in this row (row is not multiple of 8 bits)
            byte = 0
            for bit in bit_list:
                byte = (byte << 1) | bit
            payload.append(byte)
    return (name, height, width, payload)


def serialize_image_array(name, height, width, payload):
    print('Serializing image array %s' % name)
    header_output = 'extern const lui_bitmap_t BITMAP_'+name+';\n'
    output = '\nstatic const uint8_t %s_payload[%d] ={\n' % (name, len(payload))
    row = ''
    c = 0
    for byte in payload:
        row += '0x%02X,' % byte
        c += 1
        if c > 30:
            c = 0
            row += '\n'
    row = row[0:-1] #get rid of last comma
    row += '};\n\n'
    output += row
    output += 'const lui_bitmap_t BITMAP_%s = {.size_x=%d, .size_y=%d, .payload=%s_payload, .bpp=1};\n' % (name, width, height, name)
    return (header_output, output)
 
#returns tuple (header, data) with header and .c code
def make_font_description(file_path):
    print('Making font description from %s' % file_path)
    input_description = json.loads(open(file_path).read())
    bitmap_height = int(input_description['config']['textureHeight'])
    bytes_per_col = (int(bitmap_height / 8) + (1 if (bitmap_height % 8 > 0) else 0))
    name = os.path.splitext(font_json_name)[0]
    output_header = '\nextern const lui_font_t FONT_%s;\n' % name
    output_data = ''
 
    output_data += 'static const _lui_glyph_t FONT_GLYPHS_%s[] = {\n' % name
    counter = 0
    for g in input_description['symbols']:
        index_offset = bytes_per_col * int(g['x'])
        output_data += '{ .character=%d/*%c*/, .width=%d, .payload_index=%d }, ' % (g['id'], chr(g['id']), g['width'], index_offset)
        if (counter & 1):
            output_data += '\n'
        else:
            output_data += "\t\t\t"
        counter += 1
    output_data += '\n'

    for icon in valid_icon_files:
        index_offset = bytes_per_col * icon['x_offset']
        output_data += '{ .character=%d/*%s*/, .width=%d, .payload_index=%d }, ' % (icon['id'], os.path.basename(icon['path']), icon['width'], index_offset)
        if (counter & 1):
            output_data += '\n'
        else:
            output_data += "\t\t\t"
        counter += 1

    print(counter)

    output_data = output_data[0:-2] #get rid of last space and comma
    output_data += '};\n\n'
 
    output_data += '\nconst lui_font_t FONT_%s = { .bitmap = &BITMAP_%s, .glyph_count = %d, .glyphs = FONT_GLYPHS_%s };\n\n' % (name, name, counter, name)
    
    return (output_header, output_data)
 
#----------------- main -----------------
try:
    font_png_path = sys.argv[1]
    font_json_path = sys.argv[2]
    sys_icons_dir = sys.argv[3]
    usr_icons_dir = sys.argv[4]
    destination_path = sys.argv[5]
except Exception:
    print("Usage: python lameui_fontgen.py <font_png_file> <font_json_file> <system_icons_folder> <user_icons_folder> <C_files_output_folder>\n")
    print("Examples: python lameui_fontgen.py ubuntu_bold_16.PNG ubuntu_bold_16.json icons/sys_icons/ icons/usr_icons/ out/")
    sys.exit(1)

font_png_name = os.path.basename(font_png_path)
font_json_name = os.path.basename(font_json_path)

output_h_file = open(destination_path+'/'+ os.path.splitext(font_png_name)[0]+ '.h', 'w')
output_c_file = open(destination_path+'/'+ os.path.splitext(font_png_name)[0]+ '.c', 'w')
 
output_h_file.write('//AUTOGENERATED FILE! DO NOT EDIT!\n')
output_h_file.write('#include "lame_ui.h"\n')
 
output_c_file.write('//AUTOGENERATED FILE! DO NOT EDIT!\n')
output_c_file.write('#include "' + os.path.splitext(font_png_name)[0] + '.h"\n')
 
 
#serialize images
#for file_name in os.listdir(source_path):

if font_png_name.endswith(".png") or font_png_name.endswith(".PNG"):
    file_path = font_png_path
    mono_img = combine_font_and_icons(font_png_path, sys_icons_dir, usr_icons_dir)
    name, height, width, payload = make_image_array(mono_img)
    header, data = serialize_image_array(name, height, width, payload)
    output_h_file.write(header)
    output_c_file.write(data)
 
#serialize font descriptions - font description must come after bitmaps to compile properly
#for file_name in os.listdir(source_path):
if font_json_name.endswith(".json"):
    file_path = font_json_path
    header, data = make_font_description(font_json_path)
    print(header)
    output_h_file.write(header)
    output_c_file.write(data)
