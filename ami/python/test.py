#!/usr/bin/python3
from ami.ami import Ami
from ami.functions import *

def printmsg(msg):
    print("From printmsg:" + str(msg.decode("utf-8")))

def actioncb(action):
    print("got an action!")
    
a = Ami()
a.set_message_cb(printmsg)
a.set_action_cb(actioncb)
a.parse_file("../bluekeep.ami")
ru = a.get_variable("$RU_attacker")
print("$RU_attacker = %s" % ru)
#a.debug()

