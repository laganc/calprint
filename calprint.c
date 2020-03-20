/*
 * calprint.c
 *
 * Starter file provided to students for Assignment #1, SENG 265,
 * Summer 2019.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 80
#define MAX_EVENTS 500


/*
 * A struct type you are welcome to use, but are not
 * required to use.
 */

struct event_t{
    char dt_start[MAX_LINE_LEN];
    char dt_end[MAX_LINE_LEN];
    char formatted_date[MAX_LINE_LEN];
    char time_start[MAX_LINE_LEN];
    char time_end[MAX_LINE_LEN];
    char formatted_time_start[MAX_LINE_LEN];
    char formatted_time_end[MAX_LINE_LEN];
    char summary[MAX_LINE_LEN];
    char location[MAX_LINE_LEN];
    char dt_repeat_until[MAX_LINE_LEN];
    char hyphens[MAX_LINE_LEN];
    char *am_or_pm_start;
    char *am_or_pm_end;
};


/*
 * Function dt_format.
 *
 * Given a date-time, creates a more readable version of the
 * calendar date by using some C-library routines. For example,
 * if the string in "dt_time" corresponds to:
 *
 *   20190520T111500
 *
 * then the string stored at "formatted_time" is:
 *
 *   May 20, 2019 (Mon).
 *
 */

void dt_format(char *formatted_time, const char *dt_time, const int len)
{
    struct tm temp_time;
    time_t    full_time;
    char      temp[5];

    /*  
     * Ignore for now everything other than the year, month and date.
     * For conversion to work, months must be numbered from 0, and the 
     * year from 1900.
     */  
    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(dt_time, "%4d%2d%2d",
        &temp_time.tm_year, &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    full_time = mktime(&temp_time);
    strftime(formatted_time, len, "%B %d, %Y (%a)", 
        localtime(&full_time));
}


/*
 * Function dt_increment:
 *
 * Given a date-time, it adds the number of days in a way that
 * results in the correct year, month, and day. For example,
 * if the string in "before" corresponds to:
 *
 *   20190520T111500
 *
 * then the datetime string stored in "after", assuming that
 * "num_days" is 100, will be:
 *
 *   20190828T111500
 *
 * which is 100 days after May 20, 2019 (i.e., August 28, 2019).
 *
 */

void dt_increment(char *after, const char *before, int const num_days)
{
    struct tm temp_time, *p_temp_time;
    time_t    full_time;
    char      temp[5];

    memset(&temp_time, 0, sizeof(struct tm));
    sscanf(before, "%4d%2d%2d", &temp_time.tm_year,
        &temp_time.tm_mon, &temp_time.tm_mday);
    temp_time.tm_year -= 1900;
    temp_time.tm_mon -= 1;
    temp_time.tm_mday += num_days;

    full_time = mktime(&temp_time);
    after[0] = '\0';
/* Apparently there is an 9 required as the second parameter,
 * not an 8...  MZ
    strftime(after, 8, "%Y%m%d", localtime(&full_time));
*/
    strftime(after, 9, "%Y%m%d", localtime(&full_time));
    strncpy(after + 8, before + 8, MAX_LINE_LEN - 8); 
    after[MAX_LINE_LEN - 1] = '\0';
}

// changes time into HH:MM format
char time_format(char buffer[], char time[]){
    char buff[MAX_LINE_LEN];
    int x = atoi(time);
    int left_int = x/100;
    int right_int = x%100;
    char left_str[3];
    char right_str[3];

    if(left_int >= 13){
        left_int = left_int%12;
    }

    if(left_int == 0){
        snprintf(left_str, 3, "%d", 12);
    }
    else{
        snprintf(left_str, 3, "%d", left_int);
    }
    if(right_int == 0){
        snprintf(right_str, 3, "0%d", right_int);
    }
    else{
        snprintf(right_str, 3, "%d", right_int);
    }


    if(strlen(left_str) == 1){
        buffer[0] = left_str[0];
        buffer[1] = ':';
        buffer[2] = right_str[0];
        buffer[3] = right_str[1];
    }
    else if(strlen(left_str) == 2){
        buffer[0] = left_str[0];
        buffer[1] = left_str[1];
        buffer[2] = ':';
        buffer[3] = right_str[0];
        buffer[4] = right_str[1];
    }

    return *buffer;
}

// swaps entries in order to make them in chronological order
void swap(struct event_t *one, struct event_t *two){
    struct event_t temp;

    temp = *one;
    *one = *two;
    *two = temp;    
}

/*
 * Some global variables (as few as possible). Again, you 
 * are not required to use these -- but ensure that what is
 * **not** used is deleted.
 */

struct event_t entry[MAX_EVENTS];
int     num_entries = 0;
int k = -1;


int main(int argc, char *argv[])
{
    int from_y = 0, from_m = 0, from_d = 0;
    int to_y = 0, to_m = 0, to_d = 0;
    char *filename = NULL;
    int i; 

    for (i = 0; i < argc; i++) {
        if (strncmp(argv[i], "--from=", 7) == 0) {
            sscanf(argv[i], "--from=%d/%d/%d", &from_d, &from_m, &from_y);
        } else if (strncmp(argv[i], "--to=", 5) == 0) {
            sscanf(argv[i], "--to=%d/%d/%d", &to_d, &to_m, &to_y);
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            filename = argv[i]+7;
        }
    }

    if (from_y == 0 || to_y == 0 || filename == NULL) {
        fprintf(stderr, 
            "usage: %s --from=dd/mm/yyyy --to=dd/mm/yyyy --file=icsfile\n",
            argv[0]);
        exit(1);
    }


    char fromy[5], fromd[3], fromm[3], toy[5], tod[3], tom[3], from_date[20], to_date[20], test[3];
    char line[MAX_LINE_LEN];
    char *ch;
    char rrule_buffer[MAX_LINE_LEN], rrule_buffer2[MAX_LINE_LEN];
    int int_start_time = atoi(entry[k].time_start);

    // Combining the input year, month, day into one string
    snprintf(fromy, 5, "%d", from_y);
    snprintf(toy, 5, "%d", to_y);
    snprintf(fromm, 3, "%d", from_m);
    snprintf(tom, 3, "%d", to_m);
    snprintf(fromd, 3, "%d", from_d);
    snprintf(tod, 3, "%d", to_d);


    strcpy(from_date, fromy);
    if(strlen(fromm) == 1){
        strncat(from_date, "0", 1);
        strncat(from_date, fromm, 1);
    }
    else{
        strncat(from_date, fromm, 2);
    }

    if(strlen(fromd) == 1){
        strncat(from_date, "0", 1);
        strncat(from_date, fromd, 1);
    }
    else{
        strncat(from_date, fromd, 2);
    }

    strcpy(to_date, toy);
    if(strlen(tom) == 1){
        strncat(to_date, "0", 1);
        strncat(to_date, tom, 1);
    }
    else{
        strncat(to_date, tom, 2);
    }

    if(strlen(tod) == 1){
        strncat(to_date, "0", 1);
        strncat(to_date, tod, 1);
    }
    else{
        strncat(to_date, tod, 2);
    }

    // opening file
    FILE *file = fopen(filename, "r");

    // if there is no file, prints out error and exits
    if(file == NULL){
        printf("Error.\n");
        exit(0);
    }
    
    // loops through file line by line
    while(!feof(file)){
        fgets(line, MAX_LINE_LEN, file);

        // takes everything after the semicolon
        ch = strchr(line, ':');

        if(strncmp(line, "VERSION", 7) == 0){
        }
        
        // Gets multiple things such as date, start time, amount of hyphens needed for output
        else if(strncmp(line, "DTSTART", 7) == 0){
            k++;
            num_entries++;
            strncpy(entry[k].dt_start, &ch[1], 8);
            strncpy(entry[k].time_start, &ch[10], 4);

            int c = atoi(entry[k].time_start);
        
            entry[k].am_or_pm_start = "am";
            if(c >= 1200){
                entry[k].am_or_pm_start = "pm";
            }

            time_format(entry[k].formatted_time_start, entry[k].time_start);

            dt_format(entry[k].formatted_date, entry[k].dt_start, MAX_LINE_LEN);

            for(int h = 0; h < strlen(entry[k].formatted_date); h++){
                entry[k].hyphens[h] = '-';
            }
        }

        // gets end time
        else if(strncmp(line, "DTEND", 5) == 0){
            strncpy(entry[k].time_end, &ch[10], 4);
            int v = atoi(entry[k].time_end);

            entry[k].am_or_pm_end = "am";
            if(v >= 1200){
                entry[k].am_or_pm_end = "pm";
            }   
            time_format(entry[k].formatted_time_end, entry[k].time_end);
        }

        // gets location
        else if(strncmp(line, "LOCATION", 8) == 0){
            strncpy(entry[k].location, &ch[1], MAX_LINE_LEN);
            entry[k].location[strcspn(entry[k].location, "\n")] = '\0';
        }

        // gets summary
        else if(strncmp(line, "SUMMARY", 7) == 0){
            strncpy(entry[k].summary, &ch[1], MAX_LINE_LEN);
            entry[k].summary[strcspn(entry[k].summary, "\n")] = '\0';
        }

        // gets the UNTIL part of the RRULE line
        else if(strncmp(line, "RRULE", 5) == 0){
            strncpy(rrule_buffer2, &ch[1], MAX_LINE_LEN);
            strncpy(rrule_buffer, strchr(rrule_buffer2, '2'), 8);
            strncpy(entry[k].dt_repeat_until, rrule_buffer, 8);
        }
    }

        // char variables needed for outputting
        char temp1[MAX_LINE_LEN];
        char temp2[MAX_LINE_LEN];
        char huff[MAX_LINE_LEN];
        char duplicate_date1[MAX_LINE_LEN];
        char duplicate_date2[MAX_LINE_LEN];

        // finds out which entries to sort
        for(int u = 0; u < num_entries-1; u++){
            int cmp = strcmp(entry[u+1].dt_start, entry[u].dt_start);
            if(cmp < 0){
                swap(&entry[u+1], &entry[u]);
            }
        }

        
        for(int r = 0; r < num_entries; r++){
            strncpy(temp1, entry[r].dt_start, MAX_LINE_LEN);

            // converts string dates to int dates (easier to compare)
            int dt_start_int = atoi(entry[r].dt_start);
            int from_date_int = atoi(from_date);
            int to_date_int = atoi(to_date);
            int dt_repeat_until_int = atoi(entry[r].dt_repeat_until);
            int temp_int = atoi(entry[r].dt_start);


            // If it is a non-repeating event and within boundaries
            if(entry[r].dt_repeat_until[0] == '\0'){   
                if((dt_start_int >= from_date_int) && (dt_start_int <= to_date_int)){
                    dt_format(temp2, temp1, MAX_LINE_LEN);
                    printf("%s\n", temp2);
                    strncpy(duplicate_date2, temp2, MAX_LINE_LEN);
                    printf("%s\n", entry[r].hyphens);
                    printf("%5s %s to %s %s: %s [%s]\n\n", entry[r].formatted_time_start, entry[r].am_or_pm_start, entry[r].formatted_time_end, entry[r].am_or_pm_end, entry[r].summary, entry[r].location);
                    dt_increment(huff, temp1, 7);
                    strncpy(temp1, huff, 8);
                }
            }
                
            // If it is a repeating event
            if(entry[r].dt_repeat_until[0] != '\0'){
                char temp_buff[MAX_LINE_LEN];
                while(1==1){
                    if(temp_int >= from_date_int && temp_int <= to_date_int && temp_int <= dt_repeat_until_int){
                        dt_format(temp2, temp1, MAX_LINE_LEN);
                        printf("%s\n", temp2);
                        strncpy(duplicate_date1, temp2, MAX_LINE_LEN);
                        printf("%s\n", entry[r].hyphens);
                        printf("%5s %s to %s %s: %s [%s]\n\n", entry[r].formatted_time_start, entry[r].am_or_pm_start, entry[r].formatted_time_end, entry[r].am_or_pm_end, entry[r].summary, entry[r].location);
                        dt_increment(huff, temp1, 7);
                        strncpy(temp1, huff, 8);
                        temp_int = atoi(temp1);
                    }
                    else if(temp_int < dt_repeat_until_int){
                        dt_increment(huff, temp1, 7);
                        strncpy(temp1, huff, 8);
                        temp_int = atoi(temp1);
                    }
                    else{
                        break;
                    }
                }
            }
        }
    exit(0);
}
