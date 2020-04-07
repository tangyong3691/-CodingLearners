#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <signal.h>

char datatt[1024*10];
 
struct WriteThis {
  const char *readptr;
  size_t sizeleft;
};


 
static size_t read_callback(void *dest, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *wt = (struct WriteThis *)userp;
  size_t buffer_size = size*nmemb;
 
  if(wt->sizeleft) {
    /* copy as much as possible from the source to the destination */ 
    size_t copy_this_much = wt->sizeleft;
    if(copy_this_much > buffer_size)
      copy_this_much = buffer_size;
    memcpy(dest, wt->readptr, copy_this_much);
 
    wt->readptr += copy_this_much;
    wt->sizeleft -= copy_this_much;
    return copy_this_much; /* we copied this many bytes */ 
  }
 
  return 0; /* no more data left to deliver */ 
}

int main(void)
{
  int multis = 16;
  pid_t pid;
  signal(SIGCHLD, SIG_IGN);
  while (multis--){
     //pid_t pid;
     pid = fork();
     if(pid < 0) break;
     if(pid != 0) continue;
     else {
       post_send();
       break;
      }
  }
  if(pid) wait();
  return 0; 
}
 
int post_send(void)
{
  int send_counts = 1000000;
  int index_tt = send_counts;
  int send_fails = 0;
  CURL *curl;
  CURLcode res;
  struct timeval tv;
  struct timeval ntv;

    gettimeofday(&tv, NULL);

  memset(datatt, 0, sizeof(datatt));
  strcpy(datatt, "{\"ts\": 1564217295000, \"values\":{\"temperature\": 20.15}}");

  strcpy(datatt, "{\"drc_id\": \"862952026635648\", \"tractor_id\": \"862952026635648\", \"version\": \"1.1.0\", \"latitude\": 34.596483, \"longitude\": 112.572138, \"altitude\": 80.0, \"speed\": 0.0, \"azimuth\": 212.48, \"total_mileage\": \"0\", \"single_mileage\": \"0\", \"cellid\": \"0\", \"lac\": \"0\", \"mcc\": \"0\", \"mnc\": \"0\", \"power\": 72.0, \"extpower\": 0.0, \"gps_num\": \"15\", \"send_time\": \"1490876806000\", \"ts\": 1490876810000}");  
 
  struct WriteThis wt;
 
  wt.readptr = datatt;
  wt.sizeleft = strlen(datatt);
 
  /* In windows, this will init the winsock stuff */ 
  res = curl_global_init(CURL_GLOBAL_DEFAULT);
  /* Check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_global_init() failed: %s\n",
            curl_easy_strerror(res));
    return 1;
  }

  int multis = 1;
  pid_t pid;
  while (multis--){
     //pid_t pid;
     pid = 0;// fork();
     if(pid < 0) break;
     if(pid != 0) continue;
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://10.153.46.240:8090/api/v1/862952026635846/telemetry");
 
    /* Now specify we want to POST data */ 
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
 
    /* we want to use our own read function */ 
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
 
    /* pointer to pass to our read function */ 
    curl_easy_setopt(curl, CURLOPT_READDATA, &wt);
 
    /* get verbose debug output please */ 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
 
    /*
      If you use POST to a HTTP 1.1 server, you can send data without knowing
      the size before starting the POST if you use chunked encoding. You
      enable this by adding a header like "Transfer-Encoding: chunked" with
      CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
      specify the size in the request.
    */ 
#ifdef USE_CHUNKED
    {
      struct curl_slist *chunk = NULL;
 
      chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */ 
    }
#else
    /* Set the expected POST size. If you want to POST large amounts of data,
       consider CURLOPT_POSTFIELDSIZE_LARGE */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)wt.sizeleft);
#endif
   
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_slist_append(headers, "Content-Type: application/json");
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
#ifdef DISABLE_EXPECT
    /*
      Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
      header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
      NOTE: if you want chunked transfer too, you need to combine these two
      since you can only set one list of headers with CURLOPT_HTTPHEADER. */ 
 
    /* A less good option would be to enforce HTTP 1.0, but that might also
       have other implications. */ 
    {
      struct curl_slist *chunk = NULL;
 
      chunk = curl_slist_append(chunk, "Expect:");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */ 
    }
#endif
 
    /* Perform the request, res will get the return code */
   
    while(--send_counts) {
    wt.readptr = datatt;
  wt.sizeleft = strlen(datatt); 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK){
       send_fails++;
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));}
    else
       printf("send ok: %d,pid=%d\n",index_tt-send_counts, getpid() );
    gettimeofday(&ntv, NULL);
    if(ntv.tv_sec - tv.tv_sec > 60 * 2) break;  
  }
   curl_slist_free_all(headers);
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
    gettimeofday(&ntv, NULL);
    printf("time seconds:%d, pid=%d\n", ntv.tv_sec - tv.tv_sec, getpid());
     printf("failed count: %d, pid=%d\n", send_fails, getpid());
    break;
  }
 // if(pid)wait();
 // else exit(0);
  curl_global_cleanup();
  return 0;
}

