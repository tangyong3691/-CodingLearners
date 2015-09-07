/* check file change in path
 *
 * Copyright (C) 2015 by Tang Yong.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

int main()
{
    int fd = inotify_init();
    if ( fd == -1 ) return -1;
    int wd = inotify_add_watch(fd, "/mnt/smbfs", IN_CREATE | IN_MOVED_TO);// | IN_MODIFY);
    if( wd == -1 ) {
        close(fd);
        return -1;
    }
    fd_set readSet, writeSet;
    struct timeval timeVal;
    timeVal.tv_sec = 10;
    timeVal.tv_usec = 0;
    FD_ZERO (& readSet);
    FD_ZERO (& writeSet);
    FD_SET (fd, & readSet);
    do{
          int selectCount;
          FD_ZERO (& readSet);
          FD_SET (fd, & readSet);
          timeVal.tv_sec = 10;
          timeVal.tv_usec = 0;
          selectCount = select (fd + 1, & readSet, & writeSet, NULL, & timeVal);
          if(selectCount == -1) break;
          if(selectCount == 0) continue;
          if (FD_ISSET (fd, & readSet)) {
              int n;
              printf("check\n");
              char buf[sizeof(struct inotify_event)+NAME_MAX+1] = {0, };
              n = read(fd, buf, sizeof(buf));
              if(n >= sizeof(struct inotify_event))
              {
                  struct inotify_event *ev = (struct inotify_event *)buf;
                 printf("watch is %s, len = %d, name = %s\n",( ( ev->wd == wd ) ? "me": "other"), ev->len, ev->name);
              }
          }
    }while(1);
    inotify_rm_watch(fd, wd);
    close(fd);
    return 0;
}

