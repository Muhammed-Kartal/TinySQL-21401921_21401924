#ifndef TINYSQL_H
#define TINYSQL_H

#define MAX_NAME 64
#define MAX_DEPT 32
#define TABLE_SIZE 20011

typedef struct {
    int id;
    char name[MAX_NAME];
    char dept[MAX_DEPT];
    float gpa;
    int year;
} Record;

#endif