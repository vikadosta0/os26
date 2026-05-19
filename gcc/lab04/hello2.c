#include <stdio.h>
int main()
{
printf("Content-type: text/html\n");
printf("\n");
printf("<html><body>");
for (int i=5; i>0; i--)
printf("<h%d> Hello WWW!</h%d>",i,i);
printf("</body></html>");
return 0;
}