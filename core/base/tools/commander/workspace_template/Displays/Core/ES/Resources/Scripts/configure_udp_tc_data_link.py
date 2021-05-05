from com.windhoverlabs.studio.server.util import ServerUtil

print('sending request to server...')
port = display.getWidget("Port").getPropertyValue('pv_value').getValue()
ip_address = display.getWidget("IP").getPropertyValue('pv_value').getValue()

ServerUtil.configureUpdTcDataLink(int(port), ip_address)