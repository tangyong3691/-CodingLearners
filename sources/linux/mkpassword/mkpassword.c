/* This is create password by Random number generator.
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
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAX_INT_VAL 0xffffffff

unsigned int get_dev_urandom ()
{
  int fd;
  unsigned int n = 0;
  fd = open ("/dev/urandom", O_RDONLY);
  if (fd > 0)
  {
    read (fd, &n, sizeof (n));
  }
  close (fd);
  return n;
}

unsigned int get_dev_random ()
{
  int fd;
  unsigned int n = 0;
  fd = open ("/dev/random", O_RDONLY);
  if (fd > 0)
  {
    read (fd, &n, sizeof (n));
  }
  close (fd);
  return n;
}


void init_random ()
{
  unsigned int ticks;
  struct timeval tv;
  int fd;
  gettimeofday (&tv, NULL);
  ticks = tv.tv_sec + tv.tv_usec;
  fd = open ("/dev/urandom", O_RDONLY);
  if (fd > 0)
    {
      unsigned int r;
      int i;
      for (i = 0; i < 512; i++)
        {
          if(read (fd, &r, sizeof (r)) == sizeof(r))
            ticks += r;
        }
      close (fd);
    }
  srand (ticks);
}

showalphnum(unsigned int val)
{
    unsigned char s[7] = {0,};
    unsigned long long max = 26 * 36 * 36 * 36 * 36 * 36 - 1;
    val = val * max / MAX_INT_VAL ;
    int ii;
    int t;
    for (ii = 0; ii < 5; ii++)
    {
        t= val % 36;
        s[5-ii] = t>=10 ? (t - 10 + 'a') : (t + '0');
        val /= 36;
    }
    s[0] = val + 'a';
   printf("alphnum is: %s\n", s);
}


int main(int argc, char* argv[])
{
#define MAX_NUMS  10
	time_t tm;
	unsigned int val;
        unsigned long long max;
        unsigned int min;
	int ii;
/*	tm = time(0);
	srand(tm); */
        init_random ();
        val = rand();
        max = 0;
        min = 0;
        ii = MAX_NUMS;
	while(ii--)
	{
		max *= 10;
		max += max ? 9 : 8;
                min *= 10;
                min += min ? 0 : 1;
	}
        
	printf("max = %d\n", (unsigned int) max);
	val = min + ((unsigned long long)val) * max / MAX_INT_VAL;
	printf("rand pass word create: %6d\n", val);
        printf("dev rand pass word create: %6u\n",((unsigned int) (min + (get_dev_random() * max) / MAX_INT_VAL)));
        
        printf("dev urand pass word create: %6u\n",(unsigned int) (min + (get_dev_urandom() * max)/MAX_INT_VAL));
        printf(" dev rand ");
        showalphnum(get_dev_random());
}
