def getSender(dict, MID):
    return dict['airliner_px4_msg_flow']['telemetry'][MID]["sender"]


def getAttrList(dict, MID):
    if "attributes" in dict['airliner_px4_msg_flow']['telemetry'][MID]:
        return dict['airliner_px4_msg_flow']['telemetry'][MID]["attributes"]
    return []


def getUsers(dict, MID):
    list = dict['airliner_px4_msg_flow']['telemetry'][MID]["users"]
    if list is None:
        return []
    return list


def getMIDs(dict):
    messageIDs = []
    for MID in dict['airliner_px4_msg_flow']['telemetry']:
        messageIDs.append(MID)
    return messageIDs


def getSenders(dict):
    senders = []
    for MID in dict['airliner_px4_msg_flow']['telemetry']:
        if dict['airliner_px4_msg_flow']['telemetry'][MID]['sender']:
            if dict['airliner_px4_msg_flow']['telemetry'][MID]['sender'] not in senders:
                senders.append(dict['airliner_px4_msg_flow']['telemetry'][MID]['sender'])
    return senders


def getSenderMIDs(dict, sender):
    senderMIDs = []
    for MID in dict['airliner_px4_msg_flow']['telemetry']:
        if dict['airliner_px4_msg_flow']['telemetry'][MID]['sender'] == sender:
            senderMIDs.append(MID)
    return senderMIDs


def getAllUsers(dict):
    all_users = []
    for MID in dict['airliner_px4_msg_flow']['telemetry']:
        current_users = getUsers(dict, MID)
        for current_user in current_users:
            if current_user not in all_users:
                all_users.append(current_user)
    return all_users


def getUserMIDS(dict, user_app):
    MIDs_used = []
    for MID in dict['airliner_px4_msg_flow']['telemetry']:
        list = dict['airliner_px4_msg_flow']['telemetry'][MID]["users"]
        if list is not None:
            if user_app in list:
                MIDs_used.append(MID)
    return MIDs_used
