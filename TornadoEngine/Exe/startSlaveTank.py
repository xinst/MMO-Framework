import subprocess;
import time;

port     = 9000;
max_port = 9002;
while port < max_port:
	cmd = "start Tornado_d.exe -v 1 -d DeveloperDLL_d.dll -p port_self %d" % port;
	PIPE = subprocess.PIPE;
	p = subprocess.Popen(cmd, shell = True);
	time.sleep(0.7)
	port = port + 1;
