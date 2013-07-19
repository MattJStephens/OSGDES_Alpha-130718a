import smtplib

# needs a mailserver running on localhost

def ex1():
    server = smtplib.SMTP('localhost')

server.sendmail('matthew.stephens@internode.on.net', 'matthew.stephens@internode.on.net',
 """To: matthew.stephens@internode.on.net
From: matthew.stephens@internode.on.net
 Beware the Ides of March.
 """)

server.quit()
