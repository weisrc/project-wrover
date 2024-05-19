from netmiko import ConnectHandler

switch = {
    'device_type': 'cisco_ios',
    'host':   '192.168.1.10',
    'username': 'test',
    'password': '123456',
    'port' : 22,
    'secret': '123456'
}

net_connect = ConnectHandler(**switch)

output = net_connect.send_command('show ip int brief')
print(output)