import argparse
import yaml
from graphics import graphics
from tkinter import *
from tkinter import ttk
import tkinter.font as tkfont
from messageArt import *
from getter_functions import *


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="yaml configuration file",
                        type=str)

    args = parser.parse_args()
    print(args.file)

    yaml_dict = read_yaml(args.file)
    print(yaml_dict)
    return yaml_dict


def on_combo_configure(event):
    combo = event.widget
    style = ttk.Style()
    # check if the combobox already has the "postoffest" property
    current_combo_style = combo.cget('style') or "TCombobox"
    if len(style.lookup(current_combo_style, 'postoffset'))>0:
        return
    combo_values = combo.cget('values')
    if len(combo_values) == 0:
        return
    longest_value = max(combo_values, key=len)
    font = tkfont.nametofont(str(combo.cget('font')))
    width = font.measure(longest_value + "0") - event.width
    if (width<0):
        # no need to make the popdown smaller
        return
    # create an unique style name using widget's id
    unique_name='Combobox{}'.format(combo.winfo_id())
    # the new style must inherit from curret widget style (unless it's our custom style!)
    if unique_name in current_combo_style:
        style_name = current_combo_style
    else:
        style_name = "{}.{}".format(unique_name, current_combo_style)

    style.configure(style_name, postoffset=(0,0,width,0))
    combo.configure(style=style_name)


def show_message_ids(yaml_dict, MID, window):
    # for MID in yaml_dict['airliner_px4_msg_flow']['telemetry']:
    window.clear()
    sender = getSender(yaml_dict, MID)  # might be NONE
    AttrList = getAttrList(yaml_dict, MID)  # list
    UsersList = getUsers(yaml_dict, MID)
    if sender is None and len(AttrList) == 0 and len(UsersList) == 0:
        option1(window, MID)
    elif sender is not None and len(AttrList) == 0 and len(UsersList) == 0:
        option2(window, sender, MID)
    elif sender is not None and len(AttrList) != 0 and len(UsersList) == 0:
        option3(window, sender, MID, AttrList)
    elif sender is not None and len(AttrList) != 0 and len(UsersList) != 0:
        option4(window, sender, MID, AttrList, UsersList)
    elif sender is None and len(AttrList) != 0 and len(UsersList) != 0:
        option5(window, MID, AttrList, UsersList)
    # draw_uml_png(yaml_dict, MID)


def show_sender(dict, sender, window):
    window.clear()
    sender_MIDs = getSenderMIDs(dict, sender)
    option6(window, sender, sender_MIDs)


def show_user_app(dict, user_app, window):
    window.clear()
    MIDs_used = getUserMIDS(dict, user_app)
    option7(window, user_app, MIDs_used, dict)


def combobox1_show():
    MID = combobox1_clicked.get()
    # combobox1_label.config(text=MID)
    show_message_ids(yaml_dict, MID, message_UML_window)


def combobox2_show():
    sender = combobox2_clicked.get()
    # combobox2_label.config(text=sender)
    show_sender(yaml_dict, sender, sender_UML_window)


def combobox3_show():
    user_app = combobox3_clicked.get()
    # combobox2_label.config(text=sender)
    show_user_app(yaml_dict, user_app, user_app_UML_window)


yaml_dict = main()
root = Tk()
root.title('Selector Window')

# Adjust size
root.geometry("400x300")

message_UML_window = graphics(800, 650, "Message UML Window")
sender_UML_window = graphics(800, 650, "Sender App UML Window")
user_app_UML_window = graphics(800, 650, "User App UML Window")

combobox1_options = getMIDs(yaml_dict)

# datatype of menu text
combobox1_clicked = StringVar(root)
combobox1_clicked.set("Select Message")

combobox1_label = Label(root, text="Select MID")
combobox1_label.pack()

# Create Dropdown menu
combobox1 = ttk.Combobox(root, textvariable=combobox1_clicked, values=combobox1_options)
combobox1.bind('<Configure>', on_combo_configure)
combobox1.pack()

# Create button, it will change label text
combobox1_button = Button(root, text="Generate MID UML", command=combobox1_show).pack()

combobox2_options = getSenders(yaml_dict)

combobox2_clicked = StringVar(root)
combobox2_clicked.set("Select Sender")

# Create Label
combobox2_label = Label(root, text="Select Sender")
combobox2_label.pack()

# Create Dropdown menu
combobox2 = ttk.Combobox(root, textvariable=combobox2_clicked, values=combobox2_options)
combobox2.bind('<Configure>', on_combo_configure)
combobox2.pack()

# Create button, it will change label text
combobox2_button = Button(root, text="Generate Sender UML", command=combobox2_show).pack()

combobox3_options = getAllUsers(yaml_dict)

combobox3_clicked = StringVar(root)
combobox3_clicked.set("Select User App")

# Create Label
combobox3_label = Label(root, text="Select User App")
combobox3_label.pack()

# Create Dropdown menu
combobox3 = ttk.Combobox(root, textvariable=combobox3_clicked, values=combobox3_options)
combobox3.bind('<Configure>', on_combo_configure)
combobox3.pack()

# Create button, it will change label text
combobox3_button = Button(root, text="Generate User App UML", command=combobox3_show).pack()

# Execute tkinter
root.mainloop()
