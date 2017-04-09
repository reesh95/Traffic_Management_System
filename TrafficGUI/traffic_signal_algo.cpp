#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdio.h>

typedef struct{
	int count_of_cars;
	int incoming_rate;
	int outgoing_rate;
	int green_lit;
	int green_time;
	int wait_time;
	int masked;
	int already_done;
} road_parameter;

road_parameter roads_param[4];

int global_signal_time = 60;

int test_array[4][4]={5,5,5,5,4,6,7,8,3,4,2,7,4,3,5,2};

int i,j,sum_of_elems,count;

int green_calculator(road_parameter *para_road){
	int temp[4];
	int b,c,d,k;
	for(i=0;i<4;i++){
		temp[i]=para_road[i].count_of_cars;
	}

	std::vector<int> myvector (temp, temp+4);
	std::sort (myvector.begin(), myvector.end());
	b=myvector[0];
	for(i=0;i<4;i++){
		if(temp[i]==b){
			c=i;
			break;
		}
	}

	if((para_road[0].masked==1)   \
	   & (para_road[1].masked==1) \
	   & (para_road[2].masked==1) \
	   & (para_road[3].masked==1)){
		for(i=0;i<4;i++){
			para_road[i].masked=0;
		}
	}

	for(i=0;i<4;i++){
		if(c==i){
			if(!para_road[i].masked){
				para_road[i].green_lit = 1;
				para_road[i].masked    = 1;
				sum_of_elems = std::accumulate(myvector.begin(), myvector.end(), 0);
				d = ((float)b/(float)(1+sum_of_elems))*global_signal_time;
				if(d<5){
					d=5;
				}
				para_road[i].wait_time=0;
				para_road[i].green_time=d;
				para_road[i].already_done=1;
			}
			else
			{
				if(b==myvector[0]){
					b=myvector[1];
				}
				else if(b==myvector[1]){
					b=myvector[2];
				}
				else if(b==myvector[2]){
					b=myvector[3];
				}
				else
					break;

				for(i=0;i<4;i++){
					if(temp[i]==b){
						c=i;
						break;
					}
				}

				if(para_road[c].masked){
					for(j=0;j<4;j++){
						if(temp[j]==b && !para_road[j].masked){
							c=j;
							break;
						}
					}

					for(i=0;i<4;i++){
						if(temp[i]==b){
							k=i;
							break;
						}
					}

					if(c == k){
						for(j=0;j<4;j++){
							if(temp[j]!=b && !para_road[j].masked){
								c=j;
								break;
							}
						}
						b=temp[c];
					}
				}

				i=-1;
				continue;
			}
		}
	}

	for(i=0;i<4;i++){
		if(!para_road[i].already_done){
			para_road[i].green_lit = 0;
			para_road[i].wait_time = d;
			para_road[i].green_time= 0;
			para_road[i].already_done=1;
		}
	}

    printf("\nnorth count is %10d and time is %10d",para_road[0].count_of_cars,para_road[0].green_time);
    printf("\neast  count is %10d and time is %10d",para_road[1].count_of_cars,para_road[1].green_time);
    printf("\nsouth count is %10d and time is %10d",para_road[2].count_of_cars,para_road[2].green_time);
    printf("\nwest  count is %10d and time is %10d",para_road[3].count_of_cars,para_road[3].green_time);
    printf("\n\n");
	return 1;
}


int main(void){

	int pass =1;

    for(i=0;i<4;i++){
            roads_param[i].count_of_cars=5;
            roads_param[i].incoming_rate=5;
            roads_param[i].outgoing_rate=6;
            roads_param[i].green_lit=0;
            roads_param[i].green_time=0;
            roads_param[i].wait_time=0;
            roads_param[i].masked=0;
            roads_param[i].already_done=0;
        }

	for(count=0;count<4;count++){
        for(i=0;i<4;i++){
            roads_param[i].count_of_cars=test_array[count][i];
            roads_param[i].incoming_rate=5;
            roads_param[i].outgoing_rate=6;
            roads_param[i].wait_time=0;
            roads_param[i].already_done=0;
        }
        pass = green_calculator(roads_param);
	}

	return 0;

}
