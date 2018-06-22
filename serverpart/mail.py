import smtplib
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
 
 
fromaddr = "nagthej.m.r007@gmail.com"
toaddr = "pramod.anprakash@gmail.com"
msg = MIMEMultipart()
msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = "FLIGHT TICKET BOOKED!"
 
body = "Hello, this is to inform you that your flight ticket has been booked successfully! Happy journey"
msg.attach(MIMEText(body, 'plain'))
 
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login(fromaddr, "optimusraiden")
text = msg.as_string()
server.sendmail(fromaddr, toaddr, text)
server.quit()
