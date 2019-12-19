#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<pthread.h>
#include<unistd.h>

void biryani_ready(int,int);
void ready_to_serve(int,int);
void wait_for_slot(int);
void student_in_slot(int);
void allocate(int,int);
int empty();
void init_threads(); 

pthread_t tid1[100],tid2[100],tid3[100];
int rob[100],ser[100],stud[100];
int count = 6;
int n = 2,m = 2,k = 6;
int a = 0;

struct robot{
int w;
int r;
int f;
}*rb[100];

struct serv_table{
int no;
int cont;
int slots;
int ready;
pthread_mutex_t lock;
}*s[100];

struct student{
int alloc;
}*st[100];

void init_fn(){
for(int i = 0;i < k;i++){
st[i] = malloc((sizeof(struct student)));
st[i]->alloc = -1;
}
for(int i = 0;i < n;i++){
s[i] = malloc(sizeof(struct serv_table));
s[i]->no = i;
s[i]->cont = 0;
s[i]->ready = 0;
s[i]->slots = 3;
pthread_mutex_init(&(s[i]->lock),NULL);
}
for(int i = 0;i < m;i++){
rb[i] = malloc(sizeof(struct robot));
rb[i]->w = 2;
rb[i]->r = 1;
rb[i]->f = 2;
}
count = k;
}

int number(int low,int high){
return low + (rand() % (high - low + 1));
}

void* rob_fn(void *arg){
int i = (int *)arg;
while(1){
rb[i]->r = number(2,5);
rb[i]->w = number(1,10);
rb[i]->f = number(25,50);
printf("Robot Chef %d is preparing %d vessels of Biryani\n",i,rb[i]->r);
sleep(1);
sleep(rb[i]->w);
printf("Robot Chef %d has prepared %d vessels of Biryani. Waiting for all the vessels to be emptied to resume cooking\n",i,rb[i]->r);
sleep(1);
biryani_ready(rb[i]->r,rb[i]->f);
printf("All the vessels prepared by Robot Chef %d are emptied. Resuming cooking now.\n",i);
sleep(1);
}
return ((void*)0);
fflush(stdout);
}

void biryani_ready(int i,int k){
int j;
do{
j = empty();
if(j != -1){
pthread_mutex_lock(&(s[j]->lock));
if(s[j]->cont == 0){
s[j]->cont = k;
i--;
printf("Robot Chef %d is refilling Serving Container of Serving Table %d\n",i,j);
sleep(1);
printf("Serving Container of Table %d is refilled by Robot Chef %d\n",j,i);
sleep(1);
}
pthread_mutex_unlock(&(s[j]->lock));
}
}while(i != 0);
return;
}

int empty(){
for(int i = 0;i < n;i++){
if(s[i]->cont == 0)
return i;
}
return -1;
}

void* ser_fn(void *arg){
printf("Table arrived..\n");
sleep(1);
int i = (int *)arg;
pthread_mutex_init(&(s[i]->lock),NULL);
while(1){
while(s[i]->cont == 0);
s[i]->slots = number(1,10);
ready_to_serve(i,s[i]->slots);
if(s[i]->cont == 0)
printf("Serving Container of Table %d is empty, waiting for refill\n",i);
sleep(1);
}
}

void ready_to_serve(int i,int slots){
sleep(1);
allocate(s[i]->no,slots);
printf("Table %d resuming serving now with slots %d\n",i,s[i]->slots);
sleep(1);
pthread_mutex_lock(&(s[i]->lock));
s[i]->ready = 1;
pthread_mutex_unlock(&(s[i]->lock));
while(s[i]->ready == 1);
}

void allocate(int no,int slots){
int i = 0,c = 0;
while(a == 1);
a = 1;
if(count <= slots){
slots = count;
s[i]->slots = count;
}
while(c < slots){
if(st[i]->alloc == -1){
printf("Assigned a slot to student %d on serving table %d,remaining slots is %d\n",i,no,slots - c);
st[i]->alloc = no;
c++;
count--;
}
i = (i + 1)%k;
}
printf("Count: %d",count);
a = 0;
}


void* stud_fn(void *arg){
int i = (int *)arg;
printf("Student %d has arrived\n",i);
sleep(1);
wait_for_slot(i);
student_in_slot(i);
}

void wait_for_slot(int i){
printf("Student %d is waiting to be allocated a slot on the serving table\n",i);
sleep(1);
while(st[i]->alloc == -1 || s[st[i]->alloc]->cont == 0 || s[st[i]->alloc]->ready == 0);
printf("Student %d assigned a slot on the serving table %d and waiting to be served\n",i,st[i]->alloc);
sleep(1);
return;
}

void student_in_slot(int i){
while(s[st[i]->alloc]->ready == 0);
pthread_mutex_lock((&s[st[i]->alloc]->lock));
s[st[i]->alloc]->cont = s[st[i]->alloc]->cont - 1;
s[st[i]->alloc]->slots = s[st[i]->alloc]->slots - 1;
printf("Student %d on serving table %d has been served with remaining servings %d\n",i,st[i]->alloc,s[st[i]->alloc]->cont);
sleep(1);
if(s[st[i]->alloc]->slots <= 0){
s[st[i]->alloc]->ready = 0;
}
pthread_mutex_unlock((&s[st[i]->alloc]->lock));
}

void main(){

printf("No.of robot chefs:");
scanf("%d",&m);

printf("No.of Serving tables:");
scanf("%d",&n);

printf("No.of Students:");
scanf("%d",&k);

init_fn();
int i;
for(i = 0;i < m;i++)
rob[i] = pthread_create(&tid1[i],NULL,rob_fn,(void *)i);
for(i = 0;i < n;i++)
ser[i] = pthread_create(&tid2[i],NULL,ser_fn,(void *)i);

init_threads();
printf("Simulation Over\n");

fflush(stdout);
exit(0);
}

void init_threads(){
for(int i =0;i < k;i++){
stud[i] = pthread_create(&tid3[i],NULL,stud_fn,(void *)i);
pthread_join(tid3[i],NULL);
}
}
