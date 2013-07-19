
import smtplib

def SMTP():
  SERVER = "'smtp.gmail.com', 587"
  FROM = "m.st3ph3ns@gmail.com"
  TO = ["matthew.stephens@internode.on.net"] # must be a list
  server = smtplib.SMTP(SERVER)
  server.login("m.st3ph3ns@gmail.com", "scr3wdriv3r") 
  msg = "\nHello!" 
  server.sendmail(FROM, TO, msg)


