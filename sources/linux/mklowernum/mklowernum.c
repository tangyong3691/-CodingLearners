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

bool open_dev_random(int *fd)
{
    if(fd == 0) return false;
    *fd = open ("/dev/random", O_RDONLY);
    return (rd > 0);
}

bool get_data (int *fd, char *buffer, int size)
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

bool isdict(char *dict, int length, char c)
{
  int ii;
  for(ii = 0; ii < length; ii++)
    if(dict[ii] == c) return true;
  return false;
}

int main(int argc, char *argv[])
{
  char randpoll[27];
  char dict[36];
  int cnt = 0;
  int fd;
  int ii;
  if(!open_dev_random(&fd))
  {
        printf("failed\n");
        exit(1);
  }
  printf("start get random number.\nwhen it blocks here, please do another thing by mouse or keyboard, then you should get result.\n");
  if(!get_data(dict, 36))
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
  while(cnt < 36)
  {
     char c;
     if(!get_data(&c, 1))
     {
        printf("failed, you can rerun it later.\n");
        close(fd);
        exit(1);
     }
    if(!isdict(dict, cnt, c)) dict[cnt++] = c;
  }
  
  close(fd);
  printf("success:\n");
  for(ii = 0; ii < 36; ii++)
    printf("%c ", dict[ii] + '0');
  printf("\n");
}
