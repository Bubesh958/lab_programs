%{ 
#include<stdio.h>
#include<string.h>
char  str[100]={0};
char  tok[100]={0};
int i=0,s1=0,t1=0;
int flag=0;
%} 

%% 

[a-z+.*?#] {
	if(flag == 0) str[s1++] = yytext[yyleng-1];
	else  tok[t1++] = yytext[yyleng-1];
} 
\n {
  if(flag == 0) 
  {
   flag = 1;
  }
  else
  {
     match(str,tok);
     return 0;
  }
}
%%
void print(char s[],int st,int e)
{
  printf("\n");
  for(int i=st;i<e;i++) printf("%c",s[i]);
  printf("\n");
}

int match(char str[],char pat[])
{
	//char str[] = "sdaasdglknasdaldggfdafafadcaqafsg";
	//char pat[] = "a+l.g*o?f+#";
	int j=0,s=-1,e=-1,t=0,pre=0-1;
	int n1 = strlen(str);
	int n2 = strlen(pat);
	for(int i=0;i<n1;i++)
	{
	    s = i;
	    e = i;
		for(j=0;j<(n2-1);j++)
		{
			if(pat[j] == '.') e++;
		        else	 
			    if(pat[j+1] == '#')
			    {
				if(pat[j] >= 'a' && pat[j] <= 'z'  && pat[j] == str[e] && pre != e)
				{
					print(str,s,e);
					pre = e;
				}
				else 
				    if((pat[j] == '+' || pat[j] == '*' || pat[j] == '?') && pre != e) 
				 {
					print(str,s,e);
					pre = e;
				}
				break;
			    }
			    else 
			        if(pat[j] == '+' || pat[j] == '*' || pat[j] == '?') ;
				else
				    if(pat[j+1] == '*')
				    {
				   	while(e<(n1-1) && str[e] == pat[j]) e++;
				    }
				    else 
				        if(pat[j+1] == '?')
				 	{
				 	      if(str[e] == pat[j]) e++;
				 	}
				 	else 
				 	    if(pat[j+1] == '+')
				 	    {
				 	     	t = e;
				 	  	while(e<(n1) && str[e] == pat[j]) e++;
				 	  	if(t == e) break;
				 	    }
				 	    else  
				 	        if((pat[j+1] >= 'a' && pat[j+1] <= 'z') || pat[j+1] == '.')
				 	  	{
				 	  	    if(pat[j] == str[e]) e++;
				 	  	    else
				 	  	        break;
				 	        }		 	  		     	 	  	
		}
	}
	return 0;
}
int yywrap(){} 
int main()
{ 
yylex();   
return 0; 
} 
