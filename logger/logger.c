
#include "logger.h"

pthread_mutex_t log_mutex;
pthread_mutex_t time_mutex;
pthread_mutex_t sprintf_mutex;


extern int bizzounce;
extern mqd_t log_queue;
extern mqd_t ipc_queue;

extern file_t logfile;

extern int log_hb_count;
extern int log_hb_err;

//TODO set up shared memory to indicate when logger queue is initialized so that other
// threads don't try to write to it before it's ready

void* logger()
{
  // initialize log queue
  struct mq_attr log_attr;
 // struct sigevent qnotify;
  char queue_buf[256];
 // char* queue_buf = (char)malloc(sizeof(char)*LOG_ELEMENT_SIZE);
  unsigned int prio;
  ipcmessage_t ipc_msg;
  char msg_str[256];
  
  //printf("***Entering log queue***\n***************\n");
  // Temporary manual set of logfile name
  strcpy(logfile.filename, "prj.log");

  fileCreate(&logfile);
  //printf("Start of program: log queue uninitialized: %d.\n", log_queue);

  //printf("logger thread: log queue initialized: %d.\n", log_queue);
  //printf("create log mqueue error: %s\n", strerror(errno));

 /* qnotify.sigev_notify = SIGEV_THREAD;
  qnotify.sigev_notify_function = logger_handler;
  qnotify.sigev_notify_attributes = NULL;
  qnotify.sigev_value.sival_ptr = NULL;*/
  //mq_notify(log_queue, &qnotify);

  //  printf("mq_notify error: %s\n", strerror(errno));
 // printf("Trying to push to queue from logger thread...\n");
  //mq_send(log_queue, "9\0", 2, 0);
  //printf("logger mq_send: %s\n", strerror(errno));

  //signal(SIGUSR1, log_exit);    //signal handler for log_ops function
  
  while(bizzounce == 0)
  {
    mq_getattr(log_queue, &log_attr);  //keeps the thread alive to process signals and timer requests
    while(log_attr.mq_curmsgs > 0)
    {
     // printf("1\n");
      mq_receive(log_queue, queue_buf, 256, &prio);
      writeLogStr(&logfile, queue_buf);
      mq_getattr(log_queue, &log_attr);
      memset(queue_buf, 0, strlen(queue_buf));
    }
    log_hb_count = 0;
    log_hb_err = 0;
  }
  // close file
  fileClose(&logfile);
}

static void logger_handler()
{
  char queue_buf[256];
  unsigned int prio;
  // file ops:
  // open file
  // read from queue
  mq_receive(log_queue, queue_buf, LOG_ELEMENT_SIZE, &prio);
 // printf("**log queue received data**\n");

  //printf("Data received: %s\n", queue_buf);
  // add to file
  writeLogStr(&logfile, queue_buf);

  // close file ?

}

void log_exit()
{
  printf("exit signal received : log thread!\n\n");
}

void writeLogStruct(file_t* logfile, log_struct_t logitem)
{
  // logfile element: timestamp, log "level", source ID, log message
  char logfile_entry[LOG_LINE_SIZE];
  char ascii_buf[64];

  strcpy(logfile_entry, getCurrentTimeStr());

 // printf("Writing element to log.\n");
  switch(logitem.msg_type) // needs timestamps
  {
    case ERROR:
      strcat(logfile_entry, "ERROR -- ");
      strcat(logfile_entry, logitem.str_data);
      strcat(logfile_entry, "\n");
      break;
    case MESSAGE:
      //strcat(logfile_entry, "");
  //    strcat(logfile_entry, *(logitem->str_data));
      strcat(logfile_entry, "\n");
      break;
    case TEMP:
      strcat(logfile_entry, "Temperature: ");
      sprintf(ascii_buf, "%f", logitem.float_data);
      strcat(logfile_entry, ascii_buf);
      strcat(logfile_entry, logitem.data_units);
      strcat(logfile_entry, "\n");
      break;
    case LIGHT:
      strcat(logfile_entry, "Lux: ");
      sprintf(ascii_buf, "%f", logitem.float_data);
      strcat(logfile_entry, ascii_buf);
      strcat(logfile_entry, logitem.data_units);
      strcat(logfile_entry, "\n");
      break;
    default:
      break;
  }

  pthread_mutex_lock(&log_mutex);
  fileWrite(logfile, logfile_entry);  // logfile is already a pointer no need to
                                      // pass address-of
  pthread_mutex_unlock(&log_mutex);
}

void writeLogStr(file_t* logfile, char* log_str)
{
  pthread_mutex_lock(&log_mutex);
  fileWrite(logfile, log_str);  // logfile is already a pointer no need to
                                      // pass address-of
  pthread_mutex_unlock(&log_mutex);
}

/**
 *  @brief Return current time formatted appropriately for logging and display
 *  @return string containing current time as "hh:mm:dd > "
 *  Thread-safe
 */
char* getCurrentTimeStr()
{
  //localtime() converts epoch time to local time, rtn as struct tm
  time_t current_time;
  struct tm* current_time_tm;
  char ascii_int_buf[8];
  static char time_str[13]; // must be static to be returnable

  pthread_mutex_lock(&time_mutex);  // Be thread-safe so other requests don't
                                    //  overwrite the calling thread's time

  //time (&current_time);
  current_time = time(0);
  current_time_tm = localtime(&current_time);

  // Format time string properly and return it
  thread_sprintf(ascii_int_buf, current_time_tm->tm_hour, "%02lu");
  strcpy(time_str, ascii_int_buf);
  strcat(time_str, ":");
  thread_sprintf(ascii_int_buf, current_time_tm->tm_min, "%02lu");
  strcat(time_str, ascii_int_buf);
  strcat(time_str, ":");
  thread_sprintf(ascii_int_buf, current_time_tm->tm_sec, "%02lu");
  strcat(time_str, ascii_int_buf);

  strcat(time_str, " > ");

  pthread_mutex_unlock(&time_mutex);

  return time_str;
}

/**
 *  @brief Thread-safe sprintf for converting int to ascii
 *  @param format format of output
 *  @return return status of function
 *
 */
int8_t thread_sprintf(char* rtn_ascii, long lng, char format[])
{
  pthread_mutex_lock(&sprintf_mutex);
  sprintf(rtn_ascii, format, lng);    // Convert int to ascii and store
  pthread_mutex_unlock(&sprintf_mutex);
  return 0;
}

