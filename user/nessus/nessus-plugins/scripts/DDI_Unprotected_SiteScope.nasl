
#
# This script was written by H D Moore
# 


if(description)
{
    script_id(10778);
 script_version ("$Revision: 1.11 $");
    name["english"] = "Unprotected SiteScope Service";
    script_name(english:name["english"]);


    desc["english"] = "
The SiteScope web service has no password set. An attacker
who can connect to this server could view usernames and
passwords stored in the preferences section or reconfigure
the service.

Solution: Make sure that a password is set in the configuration 
for this service. Depending on where this server is located, 
you may want to restrict access by IP address in addition to 
username.

Risk factor : Medium/High";


    script_description(english:desc["english"]);


    summary["english"] = "Unprotected SiteScope Service";
    script_summary(english:summary["english"]);


    script_category(ACT_ATTACK);

    script_copyright(english:"This script is Copyright (C) 2001 Digital Defense Inc.",
               francais:"Ce script est Copyright (C) 2001 Digital Defense Inc.");

    family["english"] = "CGI abuses";
    family["francais"] = "Abus de CGI";
    script_family(english:family["english"], francais:family["francais"]);
    script_dependencie("find_service.nes");
    script_require_ports("Services/www", 8888);
    
    exit(0);
}

include("http_func.inc");
include("misc_func.inc");

function sendrequest (request, port)
{
    soc = http_open_socket(port);
    if(!soc)exit(0);
    send(socket:soc, data:req);
    reply = http_recv(socket:soc);
    http_close_socket(soc);
    return(reply);
}

#
# The script code starts here
#


ports = add_port_in_list(list:get_kb_list("Services/www"), port:8888);
foreach port (ports)
{
 req = http_get(item:"/SiteScope/cgi/go.exe/SiteScope?page=eventLog&machine=&logName=System&account=administrator", port:port);
 reply = sendrequest(request:req, port:port);

 if ("Event Log" >< reply)
 {
    security_hole(port:port);
 }
}
