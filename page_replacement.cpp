#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#define MAXFRAMES 10
using namespace std;

typedef struct{
	int bit; //reference bit
	int page; //page number to be accessed
} refstring;

void fifo(vector<int>, int);
void lru(vector<int>, int);
void lfu(vector<int>, int);
void sc(vector<int>, int);
void makeGNUPlot(int*,int);

int main() {
	int i,n,d,startLocA,startLocB,startLocC,count=0,last;
	ifstream infile("input");
	infile>>n>>d>>startLocA>>startLocB>>startLocC;
	infile.close();
	int A[n][n],B[n][n],C[n][n];
	last=startLocA;
	for(int i=0; i<n ; ++i){
		for(int j=0; j<n; ++j){
			count++;
			if(count<=d)
				A[i][j]=last;
			else{
				count=1;
				A[i][j]=last+1;
				last++;
			}
		}
	}
	last=startLocB;
	count=0;
	for(int i=0; i<n ; ++i){
		for(int j=0; j<n; ++j){
			count++;
			if(count<=d)
				B[i][j]=last;
			else{
				count=1;
				B[i][j]=last+1;
				last++;
			}
		}
	}
	last=startLocC;
	count=0;
	for(int i=0; i<n ; ++i){
		for(int j=0; j<n; ++j){
			count++;
			if(count<=d)
				C[i][j]=last;
			else{
				count=1;
				C[i][j]=last+1;
				last++;
			}
		}
	}
	cout<<"N= "<<n<<" ,D= "<<d<<endl;
	cout<<"Printing Input Matrix A..."<<endl;
	for(int i=0; i<n ; ++i){
		cout<<"\n";
		for(int j=0; j<n; ++j)
			cout<<A[i][j]<<" ";
	}
	cout<<endl<<endl<<"Printing Input Matrix B..."<<endl;
	for(int i=0; i<n ; ++i){
		cout<<"\n";
		for(int j=0; j<n; ++j)
			cout<<B[i][j]<<" ";
	}
	cout<<endl<<endl<<"Printing Output Matrix C..."<<endl;
	for(int i=0; i<n ; ++i){
		cout<<"\n";
		for(int j=0; j<n; ++j)
			cout<<C[i][j]<<" ";
	}
	vector<int> seq;
	for (int c = 0; c < n; c++) {
      	for (int d = 0; d < n; d++) {
        	for (int k = 0; k < n; k++) {
          		seq.push_back(A[c][k]);
          		seq.push_back(B[k][d]);
        	}
 			seq.push_back(C[c][d]);
        }
    }
	cout<<endl<<endl<<"Printing Reference String..."<<endl;
	for(int i=0; i<seq.size();++i){
		cout<<seq[i]<<" ";
	}
	cout<<endl<<endl<<"Starting Main: "<<endl	;
	int ch=1;
	cout<<endl;
	do{
		int in;
		cout<<"Enter your choice (0- FIFO, 1-LRU, 2-LFU, 3-Second Chance): ";
		cin>>in;
		switch(in){
			case 0:
				fifo(seq,d);
				break;
			case 1:
				lru(seq,d);
				break;
			case 2:
				lfu(seq,d);
				break;
			case 3:
				sc(seq,d);
				break;
		}
		cout<<"You wish to enter again? (1/0): ";
		cin>>ch;
	}while(ch!=0);
	return 0;
}

void fifo(vector<int> seq,int d){
	int faults[MAXFRAMES];
	int k=0;
	for(int numframes=1; numframes<= MAXFRAMES; ++numframes){
		int pagetable[numframes];
		for(int x=0; x< numframes; ++x){
			pagetable[x]= -1;
		}
		int index,flag,j=0,fault_count=0;
		for (int i=0;i< seq.size();i++) {
			flag=0;
			for(index=0;index< numframes; index++) {
				if(seq[i] == pagetable[index]) {
					flag=1;
				}
			}
			if(flag==0) {
				pagetable[j%numframes]= seq[i];
				fault_count++;
				j++;
			}
		}
		cout<<endl<<"Total: "<<fault_count<<" for # of Frames = "<<numframes<<endl;
		faults[k++]= fault_count;
	}
	makeGNUPlot(faults,0);
}



void lru(vector<int> seq, int d) {
	refstring page[seq.size()];
	int faults[MAXFRAMES];
	int k=0;
	for(int numframes=1; numframes<= MAXFRAMES; ++numframes){
		refstring page_table[numframes];
		int j=0,index,flag,fault_count=0,replace_idx=0;
		for (int i=0;i< seq.size();i++) {
			page[i].page= seq[i];
			page[i].bit=0;  
		}
		for(int i=0;i< numframes;i++) {
		page_table[i].page = -1;
		page_table[i].bit=0;
		}
		for(int i=0;i<seq.size();i++) {
			flag=0;
			j=0; 
			for(index=0;index < numframes;index++) {
				if(page[i].page == page_table[index].page) {
					flag=1;
					page_table[index].bit = 1; 
				}
			}
			while (flag == 0) {
				if(page_table[replace_idx].bit == 0) {
					flag=1;
					page_table[replace_idx] = page[i];
					fault_count++;
					replace_idx= (replace_idx+1) % numframes;	
				}
				else if(page_table[replace_idx].bit ==1) {
					page_table[replace_idx].bit = 0;
					replace_idx= (replace_idx+1) % numframes;
				}
			}
		}
		cout<<endl<<"Total: "<<fault_count<<" for # of Frames = "<<numframes<<endl;
		faults[k++] = fault_count;
	}
	makeGNUPlot(faults,1);
}

void lfu(vector<int> seq, int d){
	int faults[MAXFRAMES];
	int k=0;
	for(int numframes=1; numframes<= MAXFRAMES; ++numframes){
		int frame_array[numframes];
		int fault_count=0;
		int count[seq.size()];
		int freq_array[numframes];
		int f,p,num=0,min=0;
        bool flag = true;
		for(int i=0; i< numframes; i++){
            frame_array[i] = -1;
            freq_array[i] = -1;
        }
        for(int i=0; i< seq.size(); i++){
            flag = true;
            int page = seq[i];
            for(int j=0; j< numframes; j++){
                if(frame_array[j] == page){
                    flag = false;
                    count[page]++;
                    break;
                }
            }
            if(flag){
                fault_count++;
                if(i>=3){
                    for(int j=0; j<numframes; j++){
                        num = frame_array[j];
                        freq_array[j] = count[num];
                    }
                    min = freq_array[0];
                    for(int j=0; j<numframes; j++){
                        if(freq_array[j] < min)
                            min = freq_array[j];
                    }
                    for(int j=0; j<numframes; j++){
                        if(freq_array[j] == min){
                            count[page]++;
                            frame_array[j] = page;
                            break;
                        }
                    }
                }
                else{
                    frame_array[i]=page;
                    count[page]++;
                }
            }
        }
        cout<<endl<<"Total: "<<fault_count<<" for # of Frames = "<<numframes<<endl;
        faults[k++]= fault_count;
    }
   	makeGNUPlot(faults,2);
}

void sc(vector<int> seq, int d){
	int faults[MAXFRAMES];
	int k=0;
	for(int numframes=1; numframes<= MAXFRAMES; ++numframes){
		int fault_count=0;
		int fill_idx=0;
		int ref_array[numframes];
		int frame_array[numframes];
		int replace_idx=0;
		int count=0;
		for(int i=0;i< seq.size();i++){
        	int flag=0;
			if(fill_idx!=0){
    			for(int x=0;x<fill_idx;x++)
    			if(seq[i]==frame_array[x]){   
 					flag=1;
 					ref_array[x]=1;
    				break;
				}
			}
 			if(flag != 1){
            	if(fill_idx!= numframes){
					ref_array[fill_idx]=1;
               		frame_array[fill_idx++]=seq[i];
				}
				else{
					int temp;
 					while(ref_array[replace_idx]!=0){
						ref_array[replace_idx++]=0;
 						if(replace_idx==numframes)
 						replace_idx=0;
					}                     
 					temp=frame_array[replace_idx];
 					frame_array[replace_idx]=seq[i];
 					ref_array[replace_idx]=1;
				}
				fault_count++;
            }       
        }
		cout<<endl<<"Total: "<<fault_count<<" for # of Frames = "<<numframes<<endl;
      	faults[k++]= fault_count;
    }
   	makeGNUPlot(faults,3);
}   

void makeGNUPlot(int* faults, int algo){
	FILE *pipe_gp = popen("gnuplot -persistent", "w");
	char title [100]= "set title \"";
	switch(algo){
		case 0:
			strcat(title,"FIFO Page Replacement Algorithm \"\n");
			break;
		case 1:
			strcat(title,"LRU Page Replacement Algorithm \"\n");
			break;
		case 2:
			strcat(title,"LFU Page Replacement Algorithm \"\n");
			break;
		case 3:
			strcat(title,"Second Chance Page Replacement Algorithm \"\n");
			break;			
	}
	fputs(title,pipe_gp);
	fputs("set autoscale\n", pipe_gp);
	fputs("set xlabel \"No. of Frames\"\n", pipe_gp);
	fputs("set ylabel \"No. of Page Faults\"\n", pipe_gp);
  	fputs("plot '-' u 1:2\n", pipe_gp);
  	for (int i=0; i< MAXFRAMES; ++i) {
    	fprintf(pipe_gp, "%d %d\n",i+1,faults[i]);
  	}
  	fputs("e\n", pipe_gp);
  	//system("./gnuplot-popen > figure.png");
  	pclose(pipe_gp);
}
