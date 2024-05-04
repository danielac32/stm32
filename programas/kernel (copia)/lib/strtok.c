

#include <os.h>


static unsigned char *strpbrk(unsigned char *s1, unsigned char *s2)
{
   unsigned char *sc1, *sc2;

   for (sc1 = s1; *sc1 != 0; sc1++)
      for (sc2 = s2; *sc2 != 0; sc2++)
         if (*sc1 == *sc2)
            return(sc1);
   return(0);
}

char *
strtok(char *s1, const char *s2)
{
	 unsigned char *beg, *end;
   static char *save;

   beg = (s1)? s1: save;
   beg += strspn(beg, s2);
   if (*beg == '\0')
      return(0);
      
   end = strpbrk(beg, s2);
   if (end != '\0')
   {
      *end = '\0';
      end++;
      save = end;
   }
   else
      save = beg + strlen(beg);
   
   return(beg);
}

