#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

struct Movie{
public:
    int movie_ID;
    double totalRate;
    int totalVotes;
    double R;
    double WR;
    Movie(){
        movie_ID=0;
        totalRate=0.0;
        totalVotes=0;
        R=0.0;
        WR=0.0;
    }
};

struct theMovie{
public:
    int movie_ID;
    string movieName;
    int voteCount;
    double average;
    double weightedRate;

    theMovie(){
        movie_ID=0;
        movieName="";
        voteCount=0;
        average=0.0;
        weightedRate=0.0;
    }

};

template<class A>
class arrayListForMovies{
private:
    int maxSize;
    A *list;
    int partialSorting(int l,int u){
        int i,j;
        double v;
        v=list[l].weightedRate;
        i=l;
        j=u+1;
        do{
            do
                i++;
            while(list[i].weightedRate>v && i<=u);
            do
                j--;
            while(v>list[j].weightedRate);
            if(i<j){
                swap(i,j);
            }

        }while(i<j);
        swap(l,j);

        return j;
    }

public:
    arrayListForMovies(int size){
        maxSize=size;
        list=new theMovie[size];
    }
    void printA(){
        for(int i=0;i<maxSize;i++){
            if(list[i].voteCount>40000){
                cout<<list[i].movieName<<endl;
            }
        }
    }

    void orderedInsert(int i,int movieNumber,string name){
        list[i].movie_ID=movieNumber;
        list[i].movieName=name;
    }

    void swap(int first,int second){
        theMovie temp=list[first];
        list[first]=list[second];
        list[second]=temp;
    }
    int search(int movieId)const{
        int first=0;
        int last=maxSize-1;
        int mid;
        bool found=false;
        while(first<=last && !found){
            mid=(first+last)/2;
            if(list[mid].movie_ID==movieId){
                found=true;
            }
            else if(list[mid].movie_ID > movieId){
                last=mid-1;
            }
            else{
                first=mid+1;
            }
        }
        if(found){
            return mid;
        }
        else{
            return -1;
        }

    }

    void organize(int movieId,int vc,double av,double wr){
        int index=search(movieId);
        if(index!=-1){
            list[index].average=av;
            list[index].voteCount=vc;
            list[index].weightedRate=wr;
        }
    }

    int size()const{
        return maxSize;
    }

    void sorting(int l,int u){
        int j;
        if(l<u){
            j=partialSorting(l,u);
            sorting(l,j-1);
            sorting(j+1,u);

        }
    }

    void printMovie_ID(int i)const{
        string s;
        s=to_string(list[i].movie_ID);
        for(int i=s.size();i<10;i++){
            s+=" ";
        }
        cout<<s;
    }

    void printMovieName(int i)const{
        string s=list[i].movieName;
        if(s.size()>45){
            s.erase(45,s.size());
            s+="  ";
        }
        else{
            for(int i=s.size();i<47;i++){
                s+=" ";
                }
        }
        cout<<s;
    }

    void printWeightedRate(int i)const{
        string s;
        s=to_string(list[i].weightedRate);
        s.erase(s.find_last_not_of('0') + 1, string::npos);
        for(int i=s.size();i<18;i++){
            s+=" ";
        }
        cout<<s;
    }

    void printVoteCount(int i)const{
        string s;
        s=to_string(list[i].voteCount);
        for(int i=s.size();i<15;i++){
            s+=" ";
        }
        cout<<s;
    }

    void printTop250()const{
        cout<<"Index     Movie Title                                    Weighted Rate     Vote Count     Average Rate"<<endl;
        int j=250;
        for(int i=0;i<j;i++){
            if(list[i].voteCount<5587){
                j++;
            }
            else{
                printMovie_ID(i);
                printMovieName(i);
                printWeightedRate(i);
                printVoteCount(i);
                cout<<list[i].average<<endl;
            }
        }
    }

    ~arrayListForMovies(){
        delete []list;
    }
};

template<class B>
class arrayListForRatings{
private:
    B *list;
    int allVotes;
    double allRates;
    int maxSize;
    double C;
public:
    arrayListForRatings(int size){
        maxSize=size;
        allVotes=0;
        allRates=0.0;
        list=new Movie[maxSize];
    }

    Movie at(int i)const{
        return list[i];
    }

    void calculateAverageRate(){
        for(int i=0;i<maxSize;i++){
            if(list[i].movie_ID!=0){
                list[i].R=list[i].totalRate/list[i].totalVotes;
            }
        }
    }

    void calculateWeightedRate(){
        C=allRates/allVotes;
        for(int i=0;i<maxSize;i++){
            if(list[i].movie_ID!=0){
                list[i].WR=((list[i].totalVotes/(double)(list[i].totalVotes+5587))*list[i].R)+((5587/(double)(list[i].totalVotes+5587))*C);
            }
        }
    }

    void insert(int Id,double rate){
        if(list[Id].movie_ID==0){
            list[Id].movie_ID=Id;
            list[Id].totalRate=rate;
            list[Id].totalVotes++;
            allVotes++;
            allRates+=rate;
        }
        else{
            list[Id].totalRate+=rate;
            list[Id].totalVotes++;
            allVotes++;
            allRates+=rate;
        }

    }

    int size()const{
        return maxSize;
    }

    ~arrayListForRatings(){
        delete []list;
    }
};

int main(){
    clock_t start;
    double duration;
    start=clock();

    ifstream ratings;
    ifstream movies;
    ratings.open("ratings.csv");
    movies.open("movies.csv");
    int maxMovieID=193886;
    int maxMovieCount=58098;
    arrayListForRatings<Movie> ratingsList(maxMovieID);
    arrayListForMovies<theMovie> moviesList(maxMovieCount);

    string Id,rate,movieTitle;
    string data;
    int temp;
    int i=0;

    getline(movies,data);
    while(getline(movies,data)){///id,movie title,
        temp=data.find(',');
        Id.insert(0,data,0,temp);
        data.erase(0,temp+1);
        temp=data.find(',');
        movieTitle.insert(0,data,0,temp);
        if(movieTitle[0]=='"'){
            movieTitle.erase(movieTitle.begin());
        }
        moviesList.orderedInsert(i,stoi(Id),movieTitle);
        Id.clear();
        movieTitle.clear();
        i++;
    }


    getline(ratings,data);
    while(getline(ratings,data)){
        temp=data.find(',');
        data.erase(0,temp+1);
        temp=data.find(',');
        Id.insert(0,data,0,temp);
        data.erase(0,temp+1);
        temp=data.find(',');
        rate.insert(0,data,0,temp);
        ratingsList.insert(stoi(Id),stod(rate));
        Id.clear();
        rate.clear();
    }

    ratingsList.calculateAverageRate();
    ratingsList.calculateWeightedRate();

    for(int i=0;i<ratingsList.size();i++){
        if(ratingsList.at(i).movie_ID!=0){
            moviesList.organize(ratingsList.at(i).movie_ID,ratingsList.at(i).totalVotes,ratingsList.at(i).R,ratingsList.at(i).WR);
        }
    }

    moviesList.sorting(0,moviesList.size()-1);
    moviesList.printTop250();
    //moviesList.printA();

    ratings.close();
    movies.close();

    duration=(clock()-start)/(double)CLOCKS_PER_SEC;
    cout<<endl<<"DURATION = "<<duration<<" s"<<endl;

    return 0;
}
