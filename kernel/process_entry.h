#ifndef __QNXCOMM_PROCESS_ENTRY_H
#define __QNXCOMM_PROCESS_ENTRY_H


#include <linux/list.h>
#include <linux/fs.h>
#include <linux/spinlock.h>
#include <linux/kref.h>

#include "connection_table.h"
#include "qnxcomm_driver.h"


// forward decls
struct qnx_driver_data;
struct qnx_internal_msgsend;
struct qnx_channel;

struct qnx_process_entry
{
   struct list_head hook;
   struct kref refcnt;
   
   pid_t pid;
   
   struct list_head channels;
   struct qnx_connection_table connections;
   struct list_head pending;
   struct list_head pollfds;
      
   spinlock_t channels_lock;  
   spinlock_t pending_lock;
   spinlock_t pollfds_lock;
   
   struct qnx_driver_data* driver;
};


// ---------------------------------------------------------------------


/// constructor and destructor
void qnx_process_entry_init(struct qnx_process_entry* entry, struct qnx_driver_data* driver);

void qnx_process_entry_release(struct qnx_process_entry* entry);


/// channel management
int qnx_process_entry_add_channel(struct qnx_process_entry* entry);

int qnx_process_entry_remove_channel(struct qnx_process_entry* entry, int chid);

struct qnx_channel* qnx_process_entry_find_channel(struct qnx_process_entry* entry, int chid);

int qnx_process_entry_is_channel_available(struct qnx_process_entry* entry, int chid);


/// pollfd support
int qnx_process_entry_add_pollfd(struct qnx_process_entry* entry, struct file* f, int chid);

int qnx_process_entry_remove_pollfd(struct qnx_process_entry* entry, struct file* f);

struct qnx_channel* qnx_process_entry_find_channel_from_file(struct qnx_process_entry* entry, struct file* f);


/// connection management
int qnx_process_entry_add_connection(struct qnx_process_entry* entry, struct qnx_io_attach* att_data);

int qnx_process_entry_remove_connection(struct qnx_process_entry* entry, int coid);

struct qnx_connection qnx_process_entry_find_connection(struct qnx_process_entry* entry, int coid);


/// pending requests management
void qnx_process_entry_add_pending(struct qnx_process_entry* entry, struct qnx_internal_msgsend* data);

struct qnx_internal_msgsend* qnx_process_entry_release_pending(struct qnx_process_entry* entry, int rcvid);


#endif   // __QNXCOMM_PROCESS_ENTRY_H
