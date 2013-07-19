import smtplib
  
def sendemail(from_addr, to_addr_list, cc_addr_list,
              subject, message,
              login, password,
              smtpserver='smtp.gmail.com:587'):
    header  = 'From: %s\n' % from_addr
    header += 'To: %s\n' % ','.join(to_addr_list)
    header += 'Cc: %s\n' % ','.join(cc_addr_list)
    header += 'Subject: %s\n\n' % subject
    message = header + message
  
    server = smtplib.SMTP(smtpserver)
    server.starttls()
    server.login(login,password)
    problems = server.sendmail(from_addr, to_addr_list, message)
    server.quit()

def SMTP()

sendemail(from_addr    = 'm.st3ph3ns@gmail.com', 
          to_addr_list = ['m.st3ph3ns@gmail.com'],
          cc_addr_list = ['matthew.stephens@internode.on.net'], 
          subject      = 'Howdy', 
          message      = 'Howdy from a python function', 
          login        = 'm.st3ph3ns@gmail.com', 
          password     = 'scr3wdriv3r')
