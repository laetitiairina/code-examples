//
//  main.c
//  task2
//
//  Created by Laetitia Britschgi on 13.05.18.
//  Copyright © 2018 Laetitia Britschgi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

int m[100];
int possible_profits[100];
int count;

// for sorting
int compare(const void* a, const void* b)
{
    int int_a = * ( (int*) a );
    int int_b = * ( (int*) b );
    
    return (int_a > int_b) - (int_a < int_b);
}

int maxProfit(int price[], int n, int k) {
//    reset count
    count = 0;
    for (int i=1; i<n; i++) {
//        possible price added if one waits with selling the product for another day
        int daily_advantage = price[i] - price[i-1];
//        if the good loses value
//        show that there's a break and add the profit of the previous day to the possible profits
        if(daily_advantage < 0) {
            m[i] = -1;
            if(m[i-1] != -1) {
                possible_profits[count] = m[i-1];
                count++;
            }
        }
//        if the price is not falling, then add the new profit to the old one
        else if(m[i-1] > 0) {
            m[i] = daily_advantage + m[i-1];
        }
//        if yesterday there was a break, the profit sum starts anew
        else {
            m[i] = daily_advantage;
        }
//        the profit sum in the last element of the array is an option as well
        if(i == n-1) {
            possible_profits[count] = m[i];
            count++;
        }
    }
//        some kind of sorting
    qsort(possible_profits, count, sizeof(int), compare);
//    get the k highest possible profits
    int max = 0;
    for(int i=k-1; i>=count-k; i--) {
        max = max + possible_profits[i];
    }
    return max;
}

int main(int argc, const char * argv[]) {
    int price [8] = {12,14,17,10,14,13,12,15};
    int k = 3;
    int n = 8;
    printf("Max Profit: %d\n", maxProfit(price, n, k));
    return 0;
}
