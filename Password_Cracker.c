#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void solve(int idx);
char a[10][11],str[2001];
int dp[2000],N,len;

int main(){
  int T,i;
  scanf("%d",&T);
  while(T--){
    memset(dp,-1,sizeof(dp));
    scanf("%d",&N);
    for(i=0;i<N;i++)
      scanf("%s",&a[i][0]);
    scanf("%s",str);
    len=strlen(str);
    solve(0);
    if(dp[0]==-2)
      printf("WRONG PASSWORD\n");
    else{
      i=0;
      while(i<len){
        printf("%s ",&a[dp[i]][0]);
        i+=strlen(&a[dp[i]][0]);
      }
      printf("\n");
    }
  }
  return 0;
}
void solve(int idx){
  int i;
  if(idx>=len || dp[idx]!=-1)
    return;
  for(i=0;i<N;i++)
    if(!strncmp(&str[idx],&a[i][0],strlen(&a[i][0])))
      if(!str[idx+strlen(&a[i][0])]){
        dp[idx]=i;
        break;
      }
      else{
        solve(idx+strlen(&a[i][0]));
        if(dp[idx+strlen(&a[i][0])]>=0){
          dp[idx]=i;
          break;
        }
      }
  if(dp[idx]==-1)
    dp[idx]=-2;
  return;
}
