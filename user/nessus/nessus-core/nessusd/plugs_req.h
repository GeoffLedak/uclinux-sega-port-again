#ifndef PLUGINS_REQUIREMENTS_H__
#define PLUGINS_REQUIREMENTS_H__

char * requirements_plugin(struct arglist *, 
                           struct arglist *, 
			   struct arglist *);
 

struct arglist*  requirements_common_ports(struct arglist *,
			       struct arglist *);
			       
#endif			  
