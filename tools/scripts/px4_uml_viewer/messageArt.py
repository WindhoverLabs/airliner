from getter_functions import *

def drawSender(gui, sender):
    gui.text(50, 240, sender, "black", 15)
    gui.line(50 + len(sender) + 75, 250, 250, 250, "black", 1)


def drawMidBox(gui, MID):
    gui.rectangle(250, 23, 300, 600, "grey")
    gui.text(270, 50, MID, "black", 10)


def drawSenderBox(gui, MID):
    gui.rectangle(50, 23, 100, 100, "grey")
    gui.text(70, 50, MID, "black", 10)


def drawUserAppBox(gui, user_app):
    gui.rectangle(600, 223, 100, 100, "grey")
    gui.text(620, 250, user_app, "black", 10)


def drawAttributes(gui, AttrList):
    gap = 15
    x = 300
    y = 75
    userCord = {}
    for attrDict in AttrList:
        if type(attrDict) == type(userCord):
            for attr, dict in attrDict.items():
                gui.text(x, y, attr, "black", 7)
                gui.line(x + len(attr) * 5, y + 7, 550, y + 7, "black", 1)
                if dict is not None:
                    for user in dict["users"]:
                        if user not in userCord:
                            userCord[user] = []
                        userCord[user].append((550, y + 7))

            y += gap
    return userCord


def drawUsers(gui, AttrList, UserList, cords):
    x = 675
    y = 80
    for user in UserList:
        gui.text(x, y, user, "black", 12)
        if user in cords:
            for tuple in cords[user]:
                xi = tuple[0]
                yi = tuple[1]
                gui.line(xi, yi, x, y + 7, "black", 1)

        y += 75


def drawSenderMIDs(gui, sender_MIDS):
    x = 275
    y = 80
    xi = 150
    yi = 75
    for MID in sender_MIDS:
        gui.text(x, y, MID, "black", 12)
        gui.line(xi, yi, x, y + 7, "black", 1)

        y += 75

def drawUserAppMIDs(gui, MIDs_used, dict):
    x = 150
    y = 80
    xi = 600
    yi = 275
    for MID in MIDs_used:
        gui.text(x, y, MID, "black", 12)
        gui.line(xi, yi, x+len(MID) * 10, y + 7, "black", 1)
        sender_text = getSender(dict, MID)
        gui.text(x-75, y, sender_text, "black", 10)

        y += 25


def option1(gui, MID):
    '''
    No sender
    No Attr
    No Users
    :param gui:
    :param MID:
    :return:
    '''
    drawMidBox(gui, MID)
    gui.update_frame(1)  # FPS


def option2(gui, sender, MID):
    '''
    Sender
    No attr
    No Users
    This scenario doesn't come up in the yaml file
    :param gui:
    :param sender:
    :param MID:
    :return:
    '''

    drawSender(gui, sender)
    drawMidBox(gui, MID)
    gui.update_frame(1)


def option3(gui, sender, MID, AttrList):
    '''
    Sender
    Attr
    No Users
    :param gui:
    :param sender:
    :param AttrList:
    :return:
    '''

    drawSender(gui, sender)
    drawMidBox(gui, MID)
    drawAttributes(gui, AttrList)
    gui.update_frame(1)


def option4(gui, sender, MID, AttrList, UserList):
    '''
    Sender
    Attr
    Users
    :param gui:
    :param sender:
    :param MID:
    :param AttrList:
    :param UserList:
    :return:
    '''
    drawSender(gui, sender)
    drawMidBox(gui, MID)
    userCord = drawAttributes(gui, AttrList)
    drawUsers(gui, AttrList, UserList, userCord)
    gui.update_frame(1)


def option5(gui, MID, AttrList, UserList):
    '''
    No Sender
    Attr
    User
    :param gui:
    :param MID:
    :param AttrList:
    :param UserList:
    :return:
    '''
    drawMidBox(gui, MID)
    userCord = drawAttributes(gui, AttrList)
    drawUsers(gui, AttrList, UserList, userCord)
    gui.update_frame(1)


def option6(gui, sender, sender_MIDs):
    drawSenderBox(gui, sender)
    drawSenderMIDs(gui, sender_MIDs)


def option7(gui, user_app, MIDs_used, dict):
    drawUserAppBox(gui, user_app)
    drawUserAppMIDs(gui, MIDs_used, dict)
