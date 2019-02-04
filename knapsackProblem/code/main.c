//
//  main.c
//  task1
//
//  Created by Laetitia Britschgi on 11.05.18.
//  Copyright © 2018 Laetitia Britschgi. All rights reserved.
//

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int m[501][51]; // m [capacity+1][n+1]

int knapsackRecursive1(int capacity, int mass[], int profit[], int n) {
    //    break condition knapsack full or all elements tried
    if(capacity < 1 || n == 0) {
        return 0;
    }
    
    //    to get the elements in ascending order
    //    find element with smallest mass and if mutliples: smallest profit of them
    int min_mass = INT_MAX;
    int min_profit = INT_MAX;
    int position = -1;
    for(int i=0; i<n; i++) {
        if(min_mass > mass[i]) {
            min_mass = mass[i];
            min_profit = profit[i];
            position = i;
        }
        else if(min_mass == mass[i] && min_profit > profit[i]) {
            min_mass = mass[i];
            min_profit = profit[i];
            position = i;
        }
    }
    
    //    swap with last position (to "delete" it)
    mass[position] = mass[n-1];
    mass[n-1] = min_mass;
    profit[position] = profit[n-1];
    profit[n-1] = min_profit;
    
    //    the elements mass is too big
    if(min_mass > capacity) {
        return knapsackRecursive1(capacity, mass, profit, n-1);
    }
    
    //    either take the item or leave it
    int leaveItem = knapsackRecursive1(capacity, mass, profit, n-1);
    int takeItem = min_profit + knapsackRecursive1(capacity-min_mass, mass, profit, n-1);
    //    choose option with larger profit
    if(leaveItem > takeItem) {
        return leaveItem;
    }
    else {
        return takeItem;
    }
}

int knapsackRecursive(int capacity, int mass[], int profit[], int n) {
//    break condition knapsack full or all elements tried
    if(capacity < 1 || n == 0) {
        return 0;
    }
    
//    the elements mass is too big
    if(mass[n-1] > capacity) {
        return knapsackRecursive(capacity, mass, profit, n-1);
    }
    
//    either take the item or leave it
    int leaveItem = knapsackRecursive(capacity, mass, profit, n-1);
    int takeItem = profit[n-1] + knapsackRecursive(capacity-mass[n-1], mass, profit, n-1);
//    choose option with larger profit
    if(leaveItem > takeItem) {
        return leaveItem;
    }
    else {
        return takeItem;
    }
}


int knapsackMemoized(int capacity, int mass[], int profit[], int n) {
    //    break condition knapsack full or all elements tried
    if(capacity < 1 || n == 0) {
        return 0;
    }
//    to get the elements in ascending order
    //    find element with smallest mass and if mutliples: smallest profit of them
    int min_mass = INT_MAX;
    int min_profit = INT_MAX;
    int position = -1;
    for(int i=0; i<n; i++) {
        if(min_mass > mass[i]) {
            min_mass = mass[i];
            min_profit = profit[i];
            position = i;
        }
        else if(min_mass == mass[i] && min_profit > profit[i]) {
            min_mass = mass[i];
            min_profit = profit[i];
            position = i;
        }
    }
    
    //    swap with last position (to "delete" it)
    mass[position] = mass[n-1];
    mass[n-1] = min_mass;
    profit[position] = profit[n-1];
    profit[n-1] = min_profit;
    
    //    the elements mass is too big
    if(min_mass > capacity) {
        if(m[capacity][n-1] == 0) {
            m[capacity][n-1] = knapsackMemoized(capacity, mass, profit, n-1);
        }
        return m[capacity][n-1];
    }
    
    //    either take the item or leave it and check whether recursion has been calculated before
    if(m[capacity][n-1] == 0) {
         m[capacity][n-1] = knapsackMemoized(capacity, mass, profit, n-1);
    }
    int leaveItem = m[capacity][n-1];
    
    if(m[capacity-min_mass][n-1] == 0) {
        m[capacity-min_mass][n-1] = knapsackMemoized(capacity-min_mass, mass, profit, n-1);
    }
    int takeItem = min_profit + m[capacity-min_mass][n-1];
    
    if(leaveItem > takeItem) {
        m[capacity][n] = leaveItem;
        return leaveItem;
    }
    else {
        m[capacity][n] = takeItem;
        return takeItem;
    }
}

int knapsackDynamic(int capacity, int mass[], int profit[], int n) {
    int temp=n;
//    iterate trough matrix from left->right & top->bottom
    for(int i=1; i<=capacity; i++) {
        n=temp; // reset n to go through all elements again
        for(int j=1; j<=temp;j++) {
            //    find element with smallest mass and if mutliples: smallest profit of them
            int min_mass = INT_MAX;
            int min_profit = INT_MAX;
            int position = -1;
            for(int i=0; i<n; i++) {
                if(min_mass > mass[i]) {
                    min_mass = mass[i];
                    min_profit = profit[i];
                    position = i;
                }
                else if(min_mass == mass[i] && min_profit > profit[i]) {
                    min_mass = mass[i];
                    min_profit = profit[i];
                    position = i;
                }
            }
            
            //    swap with last position
            mass[position] = mass[n-1];
            mass[n-1] = min_mass;
            profit[position] = profit[n-1];
            profit[n-1] = min_profit;

//            mass is bigger than capacity
            if(min_mass > i) {
                m[i][j] = m[i][j-1];
            }
//            decide if take or leave element
            else{
                if(m[i][j-1] > (min_profit + m[i-min_mass][j-1])) {
                    m[i][j] = m[i][j-1];
                }
                else {
                    m[i][j] = min_profit + m[i-min_mass][j-1];
                }
            }
            n--; // ignore element for next smallest search
        }
    }
//    the maximal profit will be on the bottom right corner of the matrix
    return m[capacity][temp];
}

int main(int argc, const char * argv[]) {
//    1
    printf("1\n");
    printf("\n");
    int n = 4;
    int capacity = 10;
    int mass1[4] = {6, 3, 2, 4};
    int profit1[4] = {50, 60, 40, 20};
    clock_t start;
    clock_t end;
    float seconds;
    
    start = clock();
    printf("Recursive: %d\n", knapsackRecursive(capacity, mass1, profit1, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");

    memset(m, INT_MIN, sizeof(m[0][0]) * 51 * 501); // prepare 2d array for memoisation
    start = clock();
    printf("Memoized: %d\n", knapsackMemoized(capacity, mass1, profit1, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    memset(m, 0, sizeof(m[0][0]) * 51 * 501); // reset 2d array for dynamic programming
    start = clock();
    printf("Dynamic: %d\n", knapsackDynamic(capacity, mass1, profit1, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    
//    2
    printf("2\n");
    printf("\n");
    n = 10;
    capacity = 165;
    int mass2[10] = {23, 31, 29, 44, 53, 38, 63, 85, 89, 82};
    int profit2[10] = {92, 57, 49, 68, 60, 43, 67, 84, 87, 72};
    
    start = clock();
    printf("Recursive: %d\n", knapsackRecursive(capacity, mass2, profit2, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    memset(m, INT_MIN, sizeof(m[0][0]) * 51 * 501); // prepare 2d array for memoisation
    start = clock();
    printf("Memoized: %d\n", knapsackMemoized(capacity, mass2, profit2, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    memset(m, 0, sizeof(m[0][0]) * 51 * 501); // reset 2d array for dynamic programming
    start = clock();
    printf("Dynamic: %d\n", knapsackDynamic(capacity, mass2, profit2, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    
//    3
    printf("3\n");
    printf("\n");
    n = 50;
    capacity = 500;
    srand(time(NULL));
    int mass3[50];
    int profit3[50];
    for(int i=0; i<n;i++) {
        mass3[i] = rand() % 100 + 1;
        profit3[i] = rand() % 100 + 1;
    }
    
//    start = clock();
//    printf("Recursive: %d\n", knapsackRecursive1(capacity, mass3, profit3, n));
//    end = clock();
//    seconds = (float)(end - start) / CLOCKS_PER_SEC;
//    printf("secs: %f\n", seconds);
//    printf("\n");

    memset(m, INT_MIN, sizeof(m[0][0]) * 51 * 501); // prepare 2d array for memoisation
    start = clock();
    printf("Memoized: %d\n", knapsackMemoized(capacity, mass3, profit3, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    
    memset(m, 0, sizeof(m[0][0]) * 51 * 501); // reset 2d array for dynamic programming
    start = clock();
    printf("Dynamic: %d\n", knapsackDynamic(capacity, mass3, profit3, n));
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("secs: %f\n", seconds);
    printf("\n");
    return 0;
}
