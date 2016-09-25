from smtplib import SMTP_SSL
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email import encoders
import os


def send_mail(address, filepath):
    
    basename = os.path.basename(filepath)

    # Compose attachment
    part = MIMEBase('application', "octet-stream")
    part.set_payload(open(filepath,"rb").read() )
    encoders.encode_base64(part)
    part.add_header('Content-Disposition', 'attachment; filename="%s"' % basename)

    # Compose message
    msg = MIMEMultipart()
    msg['From'] = address
    msg['To'] = address
    msg.attach(part)

    # Send mail
    smtp = SMTP_SSL()
    smtp.connect('smtp.mail.ru')
    smtp.login(address, '9+uyp5lbZHJH')
    smtp.sendmail(address, address, msg.as_string())
    smtp.quit()

send_mail(os.argv[1], os.argv[2])
