#include<stdio.h>
#include <string.h>
#include <stdlib.h>
char* my_find(char  input[],   char rule[])
{
    //write your code here
    int input_len = strlen(input);
    int rule_len = strlen(rule);
    //find string : no '*'
    void find_simple_string(const int r_start, const int r_end, 
        int *s_start, int *s_end) {
        int i = r_start, j = *s_start;
        //init return
        *s_start = -1;
        *s_end = -1;
        while(i < r_end && j < input_len) {
            if(rule[i] != '?' && rule[i] != input[j]) {
                i = r_start;
                ++j;
                *s_start = -1;
            } else {
                if(i == r_start) {
                    *s_start = j;
                }
                ++i;
                ++j;
            }
        }
        if(i != r_end)
            *s_start = -1;
        if(i == r_end && j <= input_len)
            *s_end = j ;
    }
    
    //find simple rule: no '*'
    void find_simple_rule(int *r_start, int *r_end) {
        int i = *r_start;
        *r_start = -1;
        *r_end = -1;
        //start
        while(i < rule_len && rule[i] == '*') ++i;
        if(i < rule_len) *r_start = i;
        else return;
        //end
        while(i < rule_len && rule[i] != '*')  ++i;
        *r_end = i;
    }
    
    int r_start = 0, r_end = -1, sub_r_start = 0, sub_r_end = 0;
    int s_start = -1 , s_end = 0, sub_s_start = 0, sub_s_end = 0; 
    char * ret = (char*)malloc(sizeof(char) * input_len);
    ret[0] = '\0';
    char * ptr = ret;
    //start to match
    do{ 
        printf("s_start:%d, s_end:%d, r_start:%d, r_end:%d\n", s_start, s_end, r_start, r_end);
        if(sub_r_end == rule_len) {
            //found the matched substring
            //output 
            if(s_start != -1 && s_end <= input_len) {
                strncpy(ptr, input + s_start, s_end - s_start);
                ptr += s_end - s_start;
                strcpy(ptr, " ");
                ptr += 1;
            }
            //find next
            r_start = 0;
            sub_r_end = 0;
            sub_r_start = 0;
            s_start = -1;
        }
        
        if(s_end >= input_len)
           break;
    
        sub_r_start = sub_r_end;
        r_start = sub_r_start;
        find_simple_rule(&sub_r_start, &sub_r_end);
        printf("rule start:%d, end:%d\n", sub_r_start, sub_r_end);
        if(sub_r_start == -1) {//not found 
            if(r_start < rule_len) { //match all rest
                if(s_start == -1)
                    s_start = 0;
                s_end = input_len;
            }
            sub_r_end = rule_len;
        } else {
            sub_s_start = sub_s_end;
            find_simple_string(sub_r_start, sub_r_end, &sub_s_start, &sub_s_end);
            printf("sub s start: %d, end:%d\n", sub_s_start, sub_s_end);
            if(sub_s_start == -1) //no substring match the rule
                return ret;
            else {
                //set the matched substring start
                if(s_start == -1) {
                    if(sub_r_start != 0) { //rule starts with '*'
                        s_start = 0;
                    }
                    else {
                        s_start = sub_s_start;
                    }
                }
                s_end = sub_s_end;
                //is this the last simple rule
                int tmp_start = sub_r_end, tmp_end ;
                find_simple_rule(&tmp_start, &tmp_end);
                if(tmp_start == -1 && sub_r_end >= rule_len) { //find last match
                do{
                sub_s_start = sub_s_end;
                s_end = sub_s_end;
                find_simple_string(sub_r_start, sub_r_end, &sub_s_start, &sub_s_end);
                } while(sub_s_start != -1);
                sub_s_end = s_end;
                }
            }
        }
    } while(1);
    return ret;
}



//start 提示：自动阅卷起始唯一标识，请勿删除或增加。
int main()
{    
    //write your code here
    struct T {
        char input[100];
        char rule[100];
    } test[7] = {
        {
            {"abcdefg"},
            {"ef"}
        },
        {
            {"abcdefg"},
            {"????e??"}
        },
        {
            {"abcdedg"},
            {"a*d"}
        },
        {
            {"abdabdefbdemoadkofdslefl"},
            {"de*mo*e*"}
        },
        {
            {"dsfa82739eworiq3oi2lkadsafwroeuirq"},
            {"ds*i*af"}
        },
        {
            {"ab2304lkfdjaslfd;jadsriowqeri"},
            {"??*l?"}
        },
        {
            {"aslkfjsdafklasjowqrowerulfakfafsdl"},
            {"lk*lk*"}
        }
    };
    int i ;
    for(i = 0; i < 7; ++i) {
        printf("+++++++++++++++++test %d:\n" , i);
        char * ret = my_find(test[i].input, test[i].rule);
        printf("input:%s \n rule:%s \n result:%s \n", test[i].input, test[i].rule, ret);
        free(ret);
    }
    return 0;
}
//end //提示：自动阅卷结束唯一标识，请勿删除或增加。
