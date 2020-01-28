#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include <unistd.h> 
#include<fstream>
using namespace std;

int read_cnt=0,write_cnt=0, var=0;
sem_t  rd,wrt;
string myText;

void *reader(void *i){
	
	sem_wait(&rd);
	read_cnt++;
	if (read_cnt==1)
		sem_wait(&wrt);
	int x = *((int*)(&i));
	cout<<"\nReader"<< x <<" is reading";		
	sem_post(&rd);
	ifstream file("file1.txt");
	while(getline(file,myText)){
		cout<<endl;
		cout<<myText;
	}
	file.close();
	sem_wait(&rd);
	read_cnt--;
	if(read_cnt==0)
		sem_post(&wrt);
	sem_post(&rd);
}

void *writer(void *i){

	sem_wait(&wrt);
	var+=10;
	int x = *((int*)(&i));
	cout<<"\nwriter"<<x<<" is writing";
	ofstream file("file1.txt");
	file<<"variable:"<<var<<endl;
	file.close();
	sem_post(&wrt);
}

int main(){
	pthread_t r[3],w[3];
	sem_init(&rd,0,1);
	sem_init(&wrt,0,1);
	ofstream file;
	file.open("file1.txt");
	file <<"variable:"<<0<<endl;
	file.close();
	for(int i=0;i<=2;i++)
	  {
	    pthread_create(&w[i],NULL,writer,(void *)i);
	    pthread_create(&r[i],NULL,reader,(void *)i);
	  }
	for(int i=0;i<=2;i++)
	  {
	    pthread_join(w[i],NULL);
	    pthread_join(r[i],NULL);
	  }
}









