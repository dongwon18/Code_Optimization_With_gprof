/**
 * Copyright (C) 2019-2020 by 김동원, All rights reserved.
 *
 * Project: program profiling & optimization
 *
 * File name : optimized_source_code.c
 *
 * Summery of this lab:
 *    1. get data of csv file
 *    2. csv file looks like 'stock's number, date, price'
 *    3. assign file's data to ROW_DATA *row_data
 *    4. combine data of same stock by STOCK *select_data
 *    5. calculate momentum of select_data
 *    6. get input from user
 *    7. user can select 4 modes
 *    8. calculate profit, momentum of minimum profit, rank
 *    9. print profit under the rank that user gave
 *    10. make output file 'final data.csv'
 *
 *  Modification History :
 *    1. Written by 김동원 on May 23, 2019
 *                 
 *    2. Modified by 김동원 on June 05, 2020
 *        remove loop inefficiency, try loop unrolling (optimization 6)
 *
 *  Compiler Used : gcc version 7.5.0
 *
 */

#include <stdio.h>
#include <stdlib.h>  //  to use malloc()


typedef struct a_stock {
    int stock_no;  //  stock's number which is used to distinct stocks
    int date[991];  //  array to save date of the data
    int price[991];  //  array to save price of the data
    int momentum[991];  //  array to save calculated momentum
    double profit[991];  //  array to save calculated profit
    int avg_momentum;  //  to save the 12 month average momentum of momentum
    int rank;  //  to save the ranking of the stock ranked by average momentum
}STOCK;


typedef struct ROW_DATA {
    int a_stock_no;  //  stock's number from the file
    int a_date;  //  date of recoding
    int a_price;  //  price of the stock of the day
} ROW_DATA;

// function prototype
int count_line(FILE *infile);
int count_variety(FILE *infile, ROW_DATA *row_data, int line);
void data_selection(int stock_variety, ROW_DATA *row_data, STOCK *select_data, int line, int month);
void calculate_momentum(int stock_variety, STOCK *select_data,
                        int buy_date, int line, int month);
void calculate_profit(int stock_variety, STOCK *select_data,
                      int buy_rank, double min_profit, int buy_date, char work, int line);
void print_row_data(FILE *infile, ROW_DATA *row_data, int line);
void print_the_data(int stock_variety, STOCK *select_data, int print_stock, int line, int month);
void make_output_file(int stock_variety, FILE *outfile, STOCK *select_data, int line, int month);
void get_user_input(FILE *infile, int *buy_date, int *buy_rank, double *min_profit,
                    int *print_stock, ROW_DATA *row_data, STOCK *select_data, FILE *outfile, int line);


int main(void) {
    //  pointer to handle files
    FILE *infile = fopen("all_stock_data.csv", "r");  //  input file
    FILE *outfile = fopen("final_data.csv", "w");  //  output file

    //  check file opening succeed
    if(infile == NULL || outfile == NULL){
        printf("file opening failed\n");
        return 1;
    }

    int line = count_line(infile);  //  whole line of file, same as data's no.
    printf("line : %d\n", line);


    //  pointer array to save data from infile
    ROW_DATA *row_data = (ROW_DATA *)malloc(sizeof(ROW_DATA) * line);
    int stock_variety = count_variety(infile, row_data, line);
    printf("stock variety : %d\n", stock_variety);

    int month = line/stock_variety;

    //  pointer array to save collected data
    STOCK *select_data = (STOCK *)malloc(sizeof(STOCK)*stock_variety);

    //  check if dynamic memory allocation is succeed
    if(row_data == NULL || select_data == NULL){
        printf("allocating dynamic memory failed\n");
        return 2;
    }

    // variables to save user's input
    int buy_date, buy_rank, print_stock;
    double min_profit;

    //  assign data of row_data to select data to combine data
    data_selection(stock_variety, row_data, select_data, line, month);

    //  get keyboard input from user
    get_user_input(infile, &buy_date, &buy_rank, &min_profit, &print_stock, row_data,
                   select_data, outfile, line);

    //  free dynamic data of pointer arrays
    free(select_data);
    free(row_data);

    //  close file
    fclose(infile);
    fclose(outfile);

    //  terminate the program
    return 0;
}

void get_user_input(FILE *infile, int *buy_date, int *buy_rank, double *min_profit,
                    int *print_stock, ROW_DATA *row_data, STOCK *select_data, FILE *outfile, int line) {
    char work = '2';
    int stock_variety = count_variety(infile, row_data, line);
    int month = line/stock_variety;

    while (1) {
        if (work == '1') {
            //  print the data of row_data
            print_row_data(infile, row_data, line);

            //  repeat selecting mode
            printf("what to do?\n");
            printf("1. print row data \n");
            printf("2.buy stock, find momentum that gives minimum profit\n");
            printf("3.print data of specific stock\n F: terminate");
            scanf(" %c", &work);
        } else if (work == '2') {
            printf("enter the day you want to buy stocks(20150514 ~ 20190514)\n");
            scanf("%d", buy_date);

            while ((((*buy_date - 14) % 100) != 0) || (*buy_date < 20150513)
                   || (*buy_date > 20190515)) {
                printf("improper input input date again\n");
                scanf("%d", buy_date);
            }

            //  give warning message if user ask data for last 12 months
            if (*buy_date < 20160514) {
                printf("average momentum, rank is not effective \n");
                printf("because there is no data to calculate momentum\n");
            }

            printf("enter the rank you want to buy.\n");
            scanf("%d", buy_rank);
            printf("enter minimum profit you want\n");
            scanf("%lf", min_profit);

            //  calculate momentum and average momentum
            calculate_momentum(stock_variety, select_data, *buy_date, line, month);

            //  calculate profit and print max profit and the date of max
            calculate_profit(stock_variety, select_data, *buy_rank, *min_profit, *buy_date, work, line);

            //  repeat selecting mode
            printf("what to do?\n");
            printf("1. print row data \n");
            printf("2.buy stock and find momentum that gives minimum profit\n");
            printf("3.print data of specific stock\n F: terminate");
            scanf(" %c", &work);
        } else if (work == '3') {
            //  get stock no. to print
            printf("enter number of stocks that you want to have the data of\n");
            scanf("%d", print_stock);

            //  print the data
            print_the_data(stock_variety, select_data, *print_stock, line, month);

            //  repeat selecting mode
            printf("what to do?\n");
            printf("1. print row data \n");
            printf("2.buy stock and find momentum that gives minimum profit\n");
            printf("3.print data of specific stock\n F: terminate");
            scanf(" %c", &work);
        } else if (work == 'F') {
            //  calculate momentum of all stocks
            calculate_momentum(stock_variety, select_data, *buy_date, line, month);
            //  calculate profit of all stocks and print the data to outfile
            make_output_file(stock_variety, outfile, select_data, line, month);
            printf("\nprogram terminated\n");
            break;
        } else {
            //  get mode input again
            printf("improper input\n\n");
            printf("what to do?\n");
            printf("1. print row data \n");
            printf("2.buy stock and find momentum that gives minimum profit\n");
            printf("3.print data of specific stock\n F: terminate");
            scanf(" %c", &work);
        }
    }
}

int count_line(FILE *infile) {
    int line = 0;
    int ch;  //  to save char gotten by fgetc()

    while (!feof(infile)) {
        ch = fgetc(infile);
        if (ch == '\n') {
            line++;
        }
    }
    rewind(infile);  //  rewind infile to get data from it
    return line;  //  return line
}

int count_variety(FILE *infile, ROW_DATA *row_data, int line) {
    int a_day, a_price, a_stock_no;  //  variables to get data from file
    int variety = 0;  // initialized as 0 since calculation contains plus

    for (int i = 0; i < line; i++) {
        fscanf(infile, "%d,%d,%d\n", &a_stock_no, &a_day, &a_price);

        row_data[i].a_date = a_day;
        row_data[i].a_stock_no = a_stock_no;
        row_data[i].a_price = a_price;
    }

    for (int i = 0; i < line; i++) {
        if ((row_data[i].a_stock_no) != (row_data[i + 1].a_stock_no)) {
                variety++;
            }
    }

    rewind(infile);
    return variety;  //  return variety
}

void data_selection(int stock_variety, ROW_DATA *row_data, STOCK *select_data, int line, int month) {
    for (int i = 0; i < stock_variety; i++) {
        select_data[i].stock_no = row_data[month*i].a_stock_no;

        for (int k = 0; k < month; k++) {
            select_data[i].date[k] = row_data[month*i + k].a_date;
            select_data[i].price[k] = row_data[month*i + k].a_price;
            select_data[i].rank = 1;  //  initialize rank as 1
            select_data[i].momentum[k] = 0;  //  initialize momentum as 0
            select_data[i].profit[k] = 0;  //  initialize profit as 0
        }
    }
}

void calculate_momentum(int stock_variety, STOCK *select_data, int buy_date, int line, int month) {
    int total0 = 0;
    int total1 = 0;  //  initialize total to calculate average

    for (int i = 0; i < stock_variety; i++) {
        for (int j = 0; j < month -12; j++) {
            select_data[i].momentum[j] = select_data[i].price[j] -
                                          select_data[i].price[j + 12];
        }
    }

    for (int i = 0; i < stock_variety; i++) {
        for (int j = 0; j < month; j++) {
            if (select_data[i].date[j] == buy_date) {
                for (int k = 0; k < 12; k+=2) {
                    total0 += select_data[i].momentum[j + k];
                    total1 += select_data[i].momentum[j + k + 1];
                }
            }
        }
        select_data[i].avg_momentum = (double)(total0 + total1)/12;
        total0 = 0;
        total1 = 0;
     }

    for (int i = 0; i < stock_variety; i++) {
        for (int j = 0; j < stock_variety; j++) {
            if (select_data[i].avg_momentum < select_data[j].avg_momentum) {
                select_data[i].rank++;
            }
        }
    }
}

void calculate_profit(int stock_variety, STOCK *select_data, int buy_rank,
                      double min_profit, int buy_date, char work, int line) {
    int month = line/stock_variety;
    //  to get profit for the stock in rank
    for (int k = 1; k <= buy_rank; k++) {
        for (int i = 0; i < stock_variety; i++) {
            //  variables to get max profit
            double max_profit = select_data[i].profit[0];
            //  variable to get max profit's index
            int max_num;

            //  calculate profit for stock whose rank is 1 to buy_rank
            if (select_data[i].rank == k) {
                printf("\nrank %d stock no. %d\n", k, select_data[i].stock_no);
                printf("date : profit\n\n");

                for (int j = 0; j < month; j++) {
                    select_data[i].profit[j] = (select_data[i].price[0] -
                                                select_data[i].price[j])/
                                                (double)select_data[i].price[j];
                    //  print the profit
                    printf("%d : %lf\n", select_data[i].date[j],
                               select_data[i].profit[j]);
                }

                //  find max profit
                for (int j = 0; j < month; j++) {
                        if (max_profit < select_data[i].profit[j]) {
                            max_profit = select_data[i].profit[j];
                            max_num = j;
                        }

                    //  print rank, stock_no, profit of purchasing date
                    if (select_data[i].date[j] == buy_date) {
                        printf("\nbuy_date's profit: %lf\n",
                               select_data[i].profit[j]);
                    }
                }
                // print the maximum profit
                printf("\nrank%d max profit: %lf, date : %d\n\n", k,
                           max_profit, select_data[i].date[max_num]);

            }
        }
    }
    printf("momentum of purchasing date that satisfies minimum\n");
    for (int i = 0; i < stock_variety; i++) {
        for (int j = 0; j < month; j++) {
            if (select_data[i].date[j] == buy_date) {
                if (min_profit <= select_data[i].profit[j]) {
                    printf("profit %d : %d\n", select_data[i].stock_no,
                       select_data[i].momentum[j]);
                }
            }
        }
    }

    //  mark finishing find stocks that satisfy minimum profit
    printf("\nfinished finding stocks\n\n");
}


void print_row_data(FILE *infile, ROW_DATA *row_data, int line) {
    printf("stock\tdate\tprice\n\n");
    for (int i = 0; i < line; i++) {
        printf("%d\t%d\t%d\n", row_data[i].a_stock_no, row_data[i].a_date,
               row_data[i].a_price);
    }
}

void print_the_data(int stock_variety, STOCK *select_data, int print_stock, int line, int month) {
    for (int i = 0; i < stock_variety; i++) {
        if (select_data[i].stock_no == print_stock) {
            //  print title(average momentum, ranking of stock)
            printf("average momentum : %d, rank : %d\n",
                   select_data[i].avg_momentum, select_data[i].rank);
            //  print name of the data
            printf("date\tprice\n");
            for (int j = 0; j < month; j++) {
                //  print date and price of the data
                printf("%d\t%d\n", select_data[i].date[j],
                       select_data[i].price[j]);
            }
        }
    }
}

void make_output_file(int stock_variety, FILE *outfile, STOCK *select_data, int line, int month) {
    //  calculate profit for all stocks
    for (int i = 0; i < stock_variety; i++) {
        for (int j = 0; j < month; j++) {
            select_data[i].profit[j] = (select_data[i].price[0] -
                                        select_data[i].price[j])/
                                        (double)select_data[i].price[0];
        }
    }

    //  loop for a type of stock
    for (int i = 0; i < stock_variety; i++) {
        //  print general information of the stock
        fprintf(outfile, "%d%c%d%c%d\n", select_data[i].stock_no, ',',
                    select_data[i].avg_momentum, ',', select_data[i].rank);

        //  print specific data of the stock
        for (int j = 0; j < month; j++) {
            fprintf(outfile, "%d%c%d%c%d%c%lf\n", select_data[i].date[j], ',',
                    select_data[i].price[j], ',', select_data[i].momentum[j],
                    ',', select_data[i].profit[j]);
        }
    }
}
