#ifndef __PLUGINLAUNCH_H__
#define __PLUGINLAUNCH_H__

void pluginlaunch_init();
void pluginlaunch_wait();
void pluginlaunch_wait_for_free_process();
void pluginlaunch_stop();
int plugin_launch(struct arglist*, struct arglist*, struct arglist*, struct
arglist*, struct arglist*, char*, pl_class_t*);

void pluginlaunch_disable_parrallel_checks();
void pluginlaunch_enable_parrallel_checks();


int wait_for_children();
#endif
