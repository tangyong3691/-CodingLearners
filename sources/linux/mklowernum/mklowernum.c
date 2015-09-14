/* This is create random dict for numbers(0-35).
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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

bool open_dev_random(int *fd)
{
    if(fd == 0) return false;
    *fd = open ("/dev/random", O_RDONLY);
    return (*fd > 0);
}

bool get_data (int *fd, unsigned char *buffer, int size)
{
  // int fd;
  if(buffer == 0 || size <= 0 || size > SSIZE_MAX) return false;
  //fd = open ("/dev/random", O_RDONLY);

  //if (fd > 0)
  {
    int rd;
    do{
      rd = read (*fd, buffer, size);
      buffer += rd;
      size -= rd;
    }while(rd >= 0 && size > 0);
  }
  //close (fd);
  return size == 0;
}

bool isdict(unsigned char *dict, int length, unsigned char c)
{
  int ii;
  for(ii = 0; ii < length; ii++)
    if(dict[ii] == c) return true;
  return false;
}

int main(int argc, char *argv[])
{
  //char randpoll[27];
  unsigned char dict[36];
  int cnt = 0;
  int fd;
  int ii;
  if(!open_dev_random(&fd))
  {
        printf("failed\n");
        exit(1);
  }
  printf("start get random number.\nwhen it blocks here, please do another thing by mouse or keyboard, then you should get result.\n");
  if(!get_data(&fd, dict, 36))
  {
    printf("failed, you can rerun it later.\n");
    close(fd);
    exit(1);
  }

  for(ii = 0; ii < 36; ii++)
  {
     dict[ii] = dict[ii] % 36;
     if(!isdict(dict, cnt, dict[ii]))
     {
        dict[cnt++] = dict[ii];
     }
  }
  while(cnt < 35)
  {
     unsigned char c;
     if(!get_data(&fd, &c, 1))
     {
        printf("failed, you can rerun it later.\n");
        close(fd);
        exit(1);
     }
     c = c % 36;
    if(!isdict(dict, cnt, c)) dict[cnt++] = c;
  }

  close(fd);

  if(cnt == 35)
  {
    unsigned char sortdt[36];
    for(ii = 0; ii < 36; ii++)
    {
        sortdt[ii] = 0xff;
    }
    for(ii = 0; ii < cnt; ii++)
    {
        sortdt[dict[ii]] = dict[ii];
    }
    for(ii = 0; ii < 36; ii++)
    {
        if(sortdt[ii] == 0xff)
        {
            dict[cnt++] = ii;
            break;
        }
    }
  }

  if(cnt != 36)
  {
    printf("failed\n");
    exit(1);
  }

  printf("success:\n");
  for(ii = 0; ii < 36; ii++)
  {
    if((ii%10) == 0) printf("\n");
    printf("%2d  ", (unsigned)dict[ii]);
  }

  printf("\n");
  return 0;
}
