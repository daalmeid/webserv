#!/usr/bin/python3

# Import modules for CGI handling 
import cgi, os, sys

# Date header creation
from wsgiref.handlers import format_date_time
from datetime import datetime
from time import mktime

def checkAccept():
	reqAccept = os.environ['HTTP_ACCEPT']
	if reqAccept.find('text/html') != -1 or reqAccept.find('text/*') != -1 or reqAccept.find('*/*') != -1:
		return 0
	return -1
	
if checkAccept() == -1:
	sys.exit(5) #406 Not Acceptable

now = datetime.now()
stamp = mktime(now.timetuple())

# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
try:
	first_name = form.getvalue('fname')
	last_name  = form.getvalue('lname')

	if first_name != None and last_name != None:
		content = "<html>\r\n<head>\r\n<title>Hello User!</title>\r\n</head>\r\n<body>\r\n"
		content += "<h2>Hello " + first_name + " " + last_name + "</h2>\r\n</body>\r\n</html>"
		s = "HTTP/1.1 200 OK\r\n"
		if os.getenv('FORCE_CODE') != '0':
			s = "HTTP/1.1 " + os.getenv('FORCE_CODE') + "\r\n"
		s += "content-length: " + str(len(content)) + "\r\n" + "content-type:text/html\r\nconnection: " + os.environ['HTTP_CONNECTION'] + "\r\n"
		s += "server: " + os.environ['SERVER_SOFTWARE'] + "\r\n"
		s += "date: " + str(format_date_time(stamp)) + "\r\n\r\n"
		s += content

		print(s)
	else:
		sys.exit(4)			#400 Bad Request
except KeyError:
	sys.exit(4)				#If fname or lname does not exist in form, the form was not well written and client receives 400
