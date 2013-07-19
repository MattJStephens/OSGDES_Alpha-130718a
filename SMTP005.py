"""The first step is to create an SMTP object, each object is used for connection 
with one server."""
 
import smtplib

def SMTP():

server = smtplib.SMTP('smtp.gmail.com', 587)
server.login("m.st3ph3ns@gmail.com", "scr3wdriv3r") 
#Send the mail
msg = "\nHello!" 
server.sendmail("m.st3ph3ns@gmail.com", "matthew.stephens@internode.on.net", msg)


